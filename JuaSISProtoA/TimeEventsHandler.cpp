#include "TimeEventsHandler.h"

TimeEventsHandler* teh;

void cfgMinAndHourTimerEventHandlerWrapper() {
	teh->cfgMinAndHourTimerEventHandler();
}
void minTimeEventHandlerWrapper() {
	teh->minTimeEventHandler();
}
void hourTimeEventHandlerWrapper() {
	teh->hourTimeEventHandler();
}
void dailyTimeEventHandlerWrapper() {
	teh->dailyTimeEventHandler();
}
void startAutoIrrigationTimeEventHandlerWrapper() {
	teh->startAutoIrrigationTimeEventHandler();
}
void stopAutoIrrigationTimeEventHandlerWrapper() {
	teh->stopAutoIrrigationTimeEventHandler();
}

TimeEventsHandler* TimeEventsHandler::pInstance = nullptr;

TimeEventsHandler* TimeEventsHandler::getInstance() {
   if (!pInstance)
      pInstance = new TimeEventsHandler();

   return pInstance;
}

TimeEventsHandler::TimeEventsHandler() :
		context(Context::getInstance()), actuators(Actuators::getInstance()), sensors(Sensors::getInstance()), datalogger(Datalogger::getInstance()), rtc(RTC::getInstance()) {
	tmElements_t tm;
	// set alarm timer through rtc
	if (rtc->getTimeElements(&tm) == true) {
		setTime(tm.Hour,tm.Minute,tm.Second,tm.Day,tm.Month,tmYearToCalendar(tm.Year));
	}
	else {
		// Serial.println("ERROR: rtc read");
		// log error
	}
}

void TimeEventsHandler::initTimeEvents() {
	alarmIds[(uint8_t) ALARM_IDS::START_IRRIGATION_EVENT] = Alarm.alarmRepeat(autoIrrigationStartHour,
																	autoIrrigationStartMinute, 0,
																	startAutoIrrigationTimeEventHandlerWrapper);
	Alarm.disable(alarmIds[(uint8_t) ALARM_IDS::START_IRRIGATION_EVENT]);
	alarmIds[(uint8_t) ALARM_IDS::DAILY_EVENT] = Alarm.alarmRepeat(dailyDataProcessStartHour,
														 dailyDataProcessStartMinute, 0,
														 dailyTimeEventHandlerWrapper);
	Alarm.disable(alarmIds[(uint8_t) ALARM_IDS::DAILY_EVENT]);
	alarmIds[(uint8_t) ALARM_IDS::MIN_EVENT] = Alarm.timerRepeat(60, minTimeEventHandlerWrapper);
	Alarm.disable(alarmIds[(uint8_t) ALARM_IDS::MIN_EVENT]);
	alarmIds[(uint8_t) ALARM_IDS::HOUR_EVENT] = Alarm.timerRepeat(60*60, hourTimeEventHandlerWrapper);
	Alarm.disable(alarmIds[(uint8_t) ALARM_IDS::HOUR_EVENT]);
}

void TimeEventsHandler::checkTimeEvents() {
	Alarm.delay(1);
}

void TimeEventsHandler::startTimeEvents() {
	// enable alarms
	// daily irrigation alarm
	Alarm.enable(alarmIds[(uint8_t) ALARM_IDS::START_IRRIGATION_EVENT]);
	// daily consolidation meteorological data alarm
	Alarm.enable(alarmIds[(uint8_t) ALARM_IDS::DAILY_EVENT]);
	unsigned int nextHour = (hour() + 1) % 24;
	// start once the every hour and minute event configuration function
	Alarm.alarmOnce(nextHour, 0, 1, cfgMinAndHourTimerEventHandlerWrapper);
}

void TimeEventsHandler::stopTimeEvents() {
	Alarm.disable(alarmIds[(uint8_t) ALARM_IDS::MIN_EVENT]);
	Alarm.disable(alarmIds[(uint8_t) ALARM_IDS::HOUR_EVENT]);
	Alarm.disable(alarmIds[(uint8_t) ALARM_IDS::DAILY_EVENT]);
	Alarm.disable(alarmIds[(uint8_t) ALARM_IDS::START_IRRIGATION_EVENT]);
}

void TimeEventsHandler::cfgMinAndHourTimerEventHandler() {
#ifdef DEBUG
	Serial.println("cfgMinAndHourTimerEventHandler");
#endif
	Alarm.enable(alarmIds[(uint8_t) ALARM_IDS::MIN_EVENT]);
	Alarm.enable(alarmIds[(uint8_t) ALARM_IDS::HOUR_EVENT]);
}

void TimeEventsHandler::minTimeEventHandler() {
#ifdef DEBUG
	Serial.println("minTimeEventHandler");
#endif
	// read sensor data
	float currentTmp = sensors->getTemperature();
	float currentHum = sensors->getHumidity();

	uint8_t currentMinute = context->getCurrentMinute();
	// refresh Max and Min values of temperature and humidity
	if(currentTmp > context->getOneHourMaxTemp()) context->setOneHourMaxTemp(currentTmp);
	if(currentTmp < context->getOneHourMinTemp()) context->setOneHourMinTemp(currentTmp);
	if(currentHum > context->getOneHourMaxHum()) context->setOneHourMaxHum(currentHum);
	if(currentHum < context->getOneHourMinHum()) context->setOneHourMinHum(currentHum);
	// refresh temperature and humidity averages
	if(currentMinute > 0) {
		float avgTemp = ((context->getOneHourAvgTemp() * ((float) currentMinute / (currentMinute + 1))) + currentTmp) / (currentMinute + 1);
		context->setOneHourAvgTemp(avgTemp);
		float avgHum = ((context->getOneHourAvgHum() * ((float) currentMinute / (currentMinute + 1))) + currentHum) / (currentMinute + 1);
		context->setOneHourAvgTemp(avgHum);
	}
	else {
		context->setOneHourAvgTemp(currentTmp);
		context->setOneHourAvgHum(currentHum);
	}
	context->setCurrentMinute(currentMinute + 1);
}

void TimeEventsHandler::hourTimeEventHandler() {

#ifdef DEBUG
	Serial.println("hourTimeEventHandler");
#endif
	uint8_t currentHour = context->getCurrentHour();

	datalogger->appendLineInFile("/hour.csv", context->getCurrentContextString(rtc->getTimeStamp()));
	// refresh MAX and MIN values of temperature and humidity
	if(context->getOneHourMaxTemp() > context->getOneDayMaxTemp()) context->setOneDayMaxTemp(context->getOneHourMaxTemp());
	if(context->getOneHourMinTemp() < context->getOneDayMinTemp()) context->setOneDayMinTemp(context->getOneHourMinTemp());
	if(context->getOneHourMaxHum() > context->getOneDayMaxHum()) context->setOneDayMaxHum(context->getOneHourMaxHum());
	if(context->getOneHourMinHum() < context->getOneDayMinHum()) context->setOneDayMinHum(context->getOneHourMinHum());
	// refresh temperature and humidity averages
	if(currentHour > 0) {
		float avgTemp = ((context->getOneDayAvgTemp() * ((float) currentHour / (currentHour + 1))) + context->getOneHourAvgTemp()) / (currentHour + 1);
		context->setOneDayAvgTemp(avgTemp);
		float avgHum = ((context->getOneDayAvgHum() * ((float) currentHour / (currentHour + 1))) + context->getOneHourAvgHum()) / (currentHour + 1);
		context->setOneDayAvgHum(avgHum);
	}
	else {
		context->setOneDayAvgTemp(context->getOneHourAvgTemp());
		context->setOneDayAvgHum(context->getOneHourAvgHum());
	}
	context->resetHourContext();
	context->setCurrentHour(currentHour + 1);
}

void TimeEventsHandler::dailyTimeEventHandler() {
#ifdef DEBUG
	Serial.println("dailyTimeEventHandler");
#endif

	uint16_t currentDay = context->getCurrentDay();
	float avgTemp = context->getOneDayAvgTemp();
	float avgHum = context->getOneDayAvgHum();
	// program for auto irrigation
	context->setIsAutoIrrigationSuspended(false);
	// calculates irrigation parameters
	float es = 0.6108 * exp((double) (17.27 * avgTemp)/(avgTemp + 237.3));
	float ea = (es * avgHum) / 100;
	float eto = (2.5982 * pow((1 + (avgTemp / 25)), 2) * (1 - (ea/es))) + 0.7972;
	float etc = eto * kc[currentDay];
	float ll = etc * 0.1 * sqrt(35);
	float lb = ll / efc;
	// calculate irrigation time in seconds (hour . seconds)
	uint16_t duration = ((lb / ia) * 3600);
	context->setAutoIrrigationDuration(duration);

	// write current hour values to file
	char line[120];
	char kcStr[10];
	char etoStr[10];
	dtostrf(kc[currentDay], 9, 3, kcStr);
	dtostrf(eto, 9, 3, etoStr);
	sprintf(line,"%l,%s,%s,%d", rtc->getTimeStamp(), kcStr, etoStr, context->getAutoIrrigationDuration());
	datalogger->appendLineInFile("/day.csv", line);

	// context.setCurrentDay(currentDay + 1);
	context->resetDayContext();
}

void TimeEventsHandler::startAutoIrrigationTimeEventHandler() {
#ifdef DEBUG
	Serial.println("startAutoIrrigationTimeEventHandler");
#endif
	if(context->getIsAutoIrrigationSuspended() == false) {
		actuators->setAutoPump(true);
		Alarm.timerOnce(context->getAutoIrrigationDuration(), stopAutoIrrigationTimeEventHandlerWrapper);
		context->setAutoIrrigationStartTime(rtc->getTimeStamp());
	}
}

void TimeEventsHandler::stopAutoIrrigationTimeEventHandler() {
#ifdef DEBUG
	Serial.println("stopAutoIrrigationTimeEventHandler");
#endif
	actuators->setAutoPump(false);
	uint32_t stopTime = rtc->getTimeStamp();
	uint32_t startTime = context->getAutoIrrigationStartTime();
	char line[120];
	sprintf(line,"%l,%l,%l", startTime, stopTime, (stopTime - startTime));
	// write current hour values to file
	datalogger->appendLineInFile("/aut-irrig.csv", line);
}
