#include "TimeEventsHandler.h"

void TimeEventsHandler::TimeEventsHandler(SystemController* sc) {
	this->sc = sc;
	tmElements_t tm;
	// set alarm timer through rtc
	if (sc->getRTC()->getTimeElements(&tm) == true) {
		setTime(tm.Hour,tm.Minute,tm.Second,tm.Day,tm.Month,tmYearToCalendar(tm.Year));
	}
	else {
		// logError("ERROR: rtc read");
	}
}

void TimeEventsHandler::initAlarms() {
	alarmIds[ALARM_IDS::START_IRRIGATION_EVENT] = Alarm.alarmRepeat(autoIrrigationStartHour,
																	autoIrrigationStartMinute, 0,
																	startAutoIrrigationTimeEventHandler);
	Alarm.disable(alarmIds[ALARM_IDS::START_IRRIGATION_EVENT]);
	alarmIds[ALARM_IDS::DAILY_EVENT] = Alarm.alarmRepeat(dailyDataProcessStartHour,
														 dailyDataProcessStartMinute, 0,
														 dailyTimeEventHandler);
	Alarm.disable(alarmIds[ALARM_IDS::DAILY_EVENT]);
	alarmIds[ALARM_IDS::MIN_EVENT] = Alarm.timerRepeat(60, minTimeEventHandler);
	Alarm.disable(alarmIds[ALARM_IDS::MIN_EVENT]);
	alarmIds[ALARM_IDS::HOUR_EVENT] = Alarm.timerRepeat(60*60, hourTimeEventHandler);
	Alarm.disable(alarmIds[ALARM_IDS::HOUR_EVENT]);
}

void TimeEventsHandler::checkTimeEvents() {
  Alarm.delay(1);
}

void TimeEventsHandler::cfgMinAndHourTimerEventHandler() {
#ifdef DEBUG
	Serial.println("cfgMinAndHourTimerEventHandler");
#endif
	Alarm.enable(alarmIds[ALARM_IDS::MIN_EVENT]);
	Alarm.enable(alarmIds[ALARM_IDS::HOUR_EVENT]);
}

void TimeEventsHandler::minTimeEventHandler() {
#ifdef DEBUG
	Serial.println("minTimeEventHandler");
#endif
	// read sensor data
	float currentTmp = sc->getSensors()->getTemperature();
	float currentHum = sc->getSensors()->getHumidity();
	Context* ctx = sc->getContext();
	uint8_t currentMinute = ctx->getCurrentMinute();
	// refresh Max and Min values of temperature and humidity
	if(currentTmp > ctx->getOneHourMaxTemp()) ctx->setOneHourMaxTemp(currentTmp);
	if(currentTmp < ctx->getOneHourMinTemp()) ctx->setOneHourMinTemp(currentTmp);
	if(currentHum > ctx->getOneHourMaxHum()) ctx->setOneHourMaxHum(currentHum);
	if(currentHum < ctx->getOneHourMinHum()) ctx->setOneHourMinHum(currentHum);
	// refresh temperature and humidity averages
	if(currentMinute > 0) {
		float avgTemp = ((ctx->getOneHourAvgTemp() * ((float) currentMinute / (currentMinute + 1))) + currentTmp) / (currentMinute + 1);
		ctx->setOneHourAvgTemp(avgTemp);
		float avgHum = ((ctx->getOneHourAvgHum() * ((float) currentMinute / (currentMinute + 1))) + currentHum) / (currentMinute + 1);
		ctx->setOneHourAvgTemp(avgHum);
	}
	else {
		ctx->setOneHourAvgTemp(currentTmp);
		ctx->setOneHourAvgHum(currentHum);
	}
	ctx->setCurrentMinute(currentMinute + 1);
}

void TimeEventsHandler::hourTimeEventHandler() {

#ifdef DEBUG
	Serial.println("hourTimeEventHandler");
#endif
	Datalogger* dl = sc->getDatalogger();
	Context* ctx = sc->getContext();
	uint8_t currentHour = ctx->getCurrentHour();

	dl->appendLineInFile("/hour.csv", ctx->getCurrentContextString(sc->getRTC()->getTimeStamp()));
	// refresh MAX and MIN values of temperature and humidity
	if(ctx->getOneHourMaxTemp() > ctx->getOneDayMaxTemp()) ctx->setOneDayMaxTemp(ctx->getOneHourMaxTemp());
	if(ctx->getOneHourMinTemp() < ctx->getOneDayMinTemp()) ctx->setOneDayMinTemp(ctx->getOneHourMinTemp());
	if(ctx->getOneHourMaxHum() > ctx->getOneDayMaxHum()) ctx->setOneDayMaxHum(ctx->getOneHourMaxHum());
	if(ctx->getOneHourMinHum() < ctx->getOneDayMinHum()) ctx->setOneDayMinHum(ctx->getOneHourMinHum());
	// refresh temperature and humidity averages
	if(currentHour > 0) {
		float avgTemp = ((ctx->getOneDayAvgTemp() * ((float) currentHour / (currentHour + 1))) + ctx->getOneHourAvgTemp()) / (currentHour + 1);
		ctx->setOneDayAvgTemp(avgTemp);
		float avgHum = ((ctx->getOneDayAvgHum() * ((float) currentHour / (currentHour + 1))) + ctx->getOneHourAvgHum()) / (currentHour + 1);
		ctx->setOneDayAvgHum(avgHum);
	}
	else {
		ctx->setOneDayAvgTemp(ctx->getOneHourAvgTemp());
		ctx->setOneDayAvgHum(ctx->getOneHourAvgHum());
	}
	ctx->resetHourContext();
	ctx->setCurrentHour(currentHour + 1);
}

void TimeEventsHandler::dailyTimeEventHandler() {
#ifdef DEBUG
	Serial.println("dailyTimeEventHandler");
#endif
	Datalogger *dl = sc->getDatalogger();
	Context *ctx = sc->getContext();
	uint16_t currentDay = ctx->getCurrentDay();
	float avgTemp = ctx->getOneDayAvgTemp();
	float avgHum = ctx->getOneDayAvgHum();
	// program for auto irrigation
	ctx->setIsAutoIrrigationSuspended(false);
	// calculates irrigation parameters
	float es = 0.6108 * exp((double) (17.27 * avgTemp)/(avgTemp + 237.3));
	float ea = (es * avgHum) / 100;
	float eto = (2.5982 * pow((1 + (avgTemp / 25)), 2) * (1 - (ea/es))) + 0.7972;
	float etc = eto * kc[currentDay];
	float ll = etc * 0.1 * sqrt(35);
	float lb = ll / efc;
	// calculate irrigation time in seconds (hour -> seconds)
	uint16_t duration = ((lb / ia) * 3600);
	ctx->setAutoIrrigationDuration(duration);

	// write current hour values to file
	char line[120];
	char kcStr[10];
	char etoStr[10];
	dtostrf(kc[currentDay], 9, 3, kcStr);
	dtostrf(eto, 9, 3, etoStr);
	sprintf(line,"%l,%s,%s,%d", sc->getRTC()->getTimeStamp(), kcStr, etoStr, ctx->getAutoIrrigationDuration());
	dl->appendLineInFile("/day.csv", line);

	ctx->setCurrentDay(currentDay + 1);
	ctx->resetDayContext();
}

void TimeEventsHandler::startAutoIrrigationTimeEventHandler() {
#ifdef DEBUG
	Serial.println("startAutoIrrigationTimeEventHandler");
#endif
	Context* ctx = sc->getContext();
	Actuators* actuators = sc->getActuators();

	if(ctx->getIsAutoIrrigationSuspended() == false) {
		actuators->setAutoPump(true);
		Alarm.timerOnce(ctx->getAutoIrrigationDuration(), stopAutoIrrigationTimeEventHandler);
		ctx->setAutoIrrigationStartTime(sc->getRTC()->getTimeStamp());
	}
}

void TimeEventsHandler::stopAutoIrrigationTimeEventHandler() {
#ifdef DEBUG
	Serial.println("stopAutoIrrigationTimeEventHandler");
#endif
	Context* ctx = sc->getContext();
	Actuators* actuators = sc->getActuators();
	Datalogger* dl = sc->getDatalogger();

	actuators->setAutoPump(false);
	uint32_t stopTime = sc->getRTC()->getTimeStamp();
	uint32_t startTime = ctx->getAutoIrrigationStartTime();
	char line[120];
	sprintf(line,"%l,%l,%l", startTime, stopTime, (stopTime - startTime));
	// write current hour values to file
	dl->appendLineInFile("/aut-irrig.csv", line);
}
