#include "TimeEventsHandler.h"

void minTimeEventHandlerWrapper() {
	TimeEventsHandler::getInstance()->minTimeEventHandler();
}
void hourTimeEventHandlerWrapper() {
	TimeEventsHandler::getInstance()->hourTimeEventHandler();
}
void dailyTimeEventHandlerWrapper() {
	TimeEventsHandler::getInstance()->dailyTimeEventHandler();
}
void startAutoIrrigationTimeEventHandlerWrapper() {
	TimeEventsHandler::getInstance()->startAutoIrrigationTimeEventHandler();
}
void stopAutoIrrigationTimeEventHandlerWrapper() {
	TimeEventsHandler::getInstance()->stopAutoIrrigationTimeEventHandler();
}

TimeEventsHandler* TimeEventsHandler::pInstance = nullptr;

TimeEventsHandler* TimeEventsHandler::getInstance() {
   if (!pInstance)
      pInstance = new TimeEventsHandler();

   return pInstance;
}

TimeEventsHandler::TimeEventsHandler() :
		context(Context::getInstance()), actuators(Actuators::getInstance()), sensors(Sensors::getInstance()), datalogger(Datalogger::getInstance()),
		minEventAlarmId(0), hourEventAlarmId(0), dailyEventAlarmId(0), startIrrigationEventAlarmId(0) {
}

void TimeEventsHandler::initTimeEvents() {
	startIrrigationEventAlarmId = Alarm.alarmRepeat(autoIrrigationStartHour,
													autoIrrigationStartMinute, 0,
													startAutoIrrigationTimeEventHandlerWrapper);
	Alarm.disable(startIrrigationEventAlarmId);
	dailyEventAlarmId = Alarm.alarmRepeat(dailyDataProcessStartHour,
										  dailyDataProcessStartMinute, 0,
										  dailyTimeEventHandlerWrapper);
	Alarm.disable(dailyEventAlarmId);
}

void TimeEventsHandler::checkTimeEvents() {
	Alarm.delay(1);
}

void TimeEventsHandler::startTimeEvents() {
	// enable alarms
	// daily irrigation alarm
	Alarm.enable(startIrrigationEventAlarmId);
	// daily consolidation meteorological data alarm
	Alarm.enable(dailyEventAlarmId);
	uint8_t nextHour = (hour() + 1) % 24;
	uint8_t hourAfterNext = (hour() + 2) % 24;
	// start once the every hour and minute event configuration function
	minEventAlarmId = Alarm.alarmOnce(nextHour, 1, 0, minTimeEventHandlerWrapper);
	hourEventAlarmId = Alarm.alarmOnce(hourAfterNext, 0, 1, hourTimeEventHandlerWrapper);

}

void TimeEventsHandler::stopTimeEvents() {
	Alarm.disable(minEventAlarmId);
	Alarm.disable(hourEventAlarmId);
	Alarm.disable(dailyEventAlarmId);
	Alarm.disable(startIrrigationEventAlarmId);
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
	float avgTemp = (context->getOneHourAvgTemp() * ((float) currentMinute / (currentMinute + 1))) + (currentTmp / (currentMinute + 1));
	context->setOneHourAvgTemp(avgTemp);
	float avgHum = (context->getOneHourAvgHum() * ((float) currentMinute / (currentMinute + 1))) + (currentHum / (currentMinute + 1));
	context->setOneHourAvgHum(avgHum);

	context->setCurrentMinute(currentMinute + 1);
	/* set next minute time event */
	uint8_t nextMinute = (minute() + 1) % 60;
	uint8_t eventHour = hour();
	if(nextMinute == 0) {
		eventHour = (eventHour + 1) % 24;
	}
	minEventAlarmId = Alarm.alarmOnce(eventHour, nextMinute, 0, minTimeEventHandlerWrapper);
#ifdef SIMULATION
	adjustTime(60);
#endif
}

void TimeEventsHandler::hourTimeEventHandler() {

#ifdef DEBUG
	Serial.println("hourTimeEventHandler");
#endif
	uint8_t currentHour = context->getCurrentHour();

	datalogger->appendLineInFile("/hour.csv", context->getCurrentContextString());
	// refresh MAX and MIN values of temperature and humidity
	if(context->getOneHourMaxTemp() > context->getOneDayMaxTemp()) context->setOneDayMaxTemp(context->getOneHourMaxTemp());
	if(context->getOneHourMinTemp() < context->getOneDayMinTemp()) context->setOneDayMinTemp(context->getOneHourMinTemp());
	if(context->getOneHourMaxHum() > context->getOneDayMaxHum()) context->setOneDayMaxHum(context->getOneHourMaxHum());
	if(context->getOneHourMinHum() < context->getOneDayMinHum()) context->setOneDayMinHum(context->getOneHourMinHum());
	// refresh temperature and humidity averages
	float avgTemp = (context->getOneDayAvgTemp() * ((float) currentHour / (currentHour + 1))) + (context->getOneHourAvgTemp() / (currentHour + 1));
	context->setOneDayAvgTemp(avgTemp);
	float avgHum = (context->getOneDayAvgHum() * ((float) currentHour / (currentHour + 1))) + (context->getOneHourAvgHum() / (currentHour + 1));
	context->setOneDayAvgHum(avgHum);

	context->resetHourContext();
	context->setCurrentHour(currentHour + 1);
	/* set next hour time event */
	uint8_t nextHour = (hour() + 1) % 24;
	minEventAlarmId = Alarm.alarmOnce(nextHour, 0, 1, hourTimeEventHandlerWrapper);
#ifdef DEBUG
	printTime();
#endif
}

void TimeEventsHandler::dailyTimeEventHandler() {
	if(context->getCurrentDay() > 0) {
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
		sprintf(line,"%l,%s,%s,%d", now(), kcStr, etoStr, context->getAutoIrrigationDuration());
		datalogger->appendLineInFile("/day.csv", line);

		// context.setCurrentDay(currentDay + 1);
		context->resetDayContext();
	}
}

void TimeEventsHandler::startAutoIrrigationTimeEventHandler() {
#ifdef DEBUG
	Serial.println("startAutoIrrigationTimeEventHandler");
#endif
	if(context->getIsAutoIrrigationSuspended() == false) {
		actuators->setAutoPump(true);
		Alarm.timerOnce(context->getAutoIrrigationDuration(), stopAutoIrrigationTimeEventHandlerWrapper);
		context->setAutoIrrigationStartTime(now());
	}
}

void TimeEventsHandler::stopAutoIrrigationTimeEventHandler() {
#ifdef DEBUG
	Serial.println("stopAutoIrrigationTimeEventHandler");
#endif
	actuators->setAutoPump(false);
	uint32_t stopTime = now();
	uint32_t startTime = context->getAutoIrrigationStartTime();
	char line[120];
	sprintf(line,"%l,%l,%l", startTime, stopTime, (stopTime - startTime));
	// write current hour values to file
	datalogger->appendLineInFile("/aut-irrig.csv", line);
}

void TimeEventsHandler::printTime() {
	Serial.print(now());
	Serial.print("|");
	Serial.print(day());
	Serial.write('/');
	Serial.print(month());
	Serial.write('/');
	Serial.print(year());
	Serial.print("|");
	Serial.print(hour());
	Serial.write(':');
	Serial.print(minute());
	Serial.println();
}

