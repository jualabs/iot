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
		context(Context::getInstance()), actuators(Actuators::getInstance()), sensors(Sensors::getInstance()),
		datalogger(Datalogger::getInstance()), logError(LogError::getInstance()),
		minEventAlarmId(0), hourEventAlarmId(0), dailyEventAlarmId(0), startIrrigationEventAlarmId(0) {
}

void TimeEventsHandler::initTimeEvents() {
	startIrrigationEventAlarmId = Alarm.alarmRepeat(autoIrrigationStartHour,
													autoIrrigationStartMinute, 0,
													startAutoIrrigationTimeEventHandlerWrapper);
	Alarm.disable(startIrrigationEventAlarmId);
	dailyEventAlarmId = Alarm.alarmRepeat(0, 0, 30, dailyTimeEventHandlerWrapper);
	Alarm.disable(dailyEventAlarmId);
}

void TimeEventsHandler::checkTimeEvents() {
	Alarm.delay(1);
}

void TimeEventsHandler::startTimeEvents() {
	/* enable alarms */
	/* daily irrigation alarm */
	Alarm.enable(startIrrigationEventAlarmId);
	/* daily consolidation meteorological data alarm*/
	Alarm.enable(dailyEventAlarmId);
	uint8_t nextHour = (hour() + 1) % 24;
	uint8_t hourAfterNext = (hour() + 2) % 24;
	// start once the every hour and minute event configuration function
	minEventAlarmId = Alarm.alarmOnce(nextHour, 1, 10, minTimeEventHandlerWrapper);
	hourEventAlarmId = Alarm.alarmOnce(hourAfterNext, 0, 20, hourTimeEventHandlerWrapper);
}

void TimeEventsHandler::stopTimeEvents() {
	Alarm.disable(minEventAlarmId);
	Alarm.disable(hourEventAlarmId);
	Alarm.disable(dailyEventAlarmId);
	Alarm.disable(startIrrigationEventAlarmId);
}

void TimeEventsHandler::minTimeEventHandler() {
#ifdef DEBUG
	/* Serial.println("minTimeEventHandler"); */
	// printTime();
#endif
	/* read sensor data */
	float currentTmp = sensors->getTemperature();
	float currentHum = sensors->getHumidity();
	uint8_t currentMinute = context->getCurrentMinute();

	/* verify temperature data */
	if(currentTmp < MIN_ACCEPTABLE_TMP || currentTmp > MAX_ACCEPTABLE_TMP || isnan(currentTmp)) {
		/* log the invalid value */
		char currentTmpStr[10];
		char str[80];
		dtostrf(currentTmp, 6, 2, currentTmpStr);
		sprintf(str, "ts: %d, error: invalid temperature, value: %s", now(), currentTmpStr);
		logError->insertError(str);
		free(currentTmpStr);
		free(str);
	}
	else {
		/* refresh max and min values of temperature */
		if(currentTmp > context->getOneHourMaxTemp()) context->setOneHourMaxTemp(currentTmp);
		if(currentTmp < context->getOneHourMinTemp()) context->setOneHourMinTemp(currentTmp);
		/* refresh temperature averages */
		float avgTemp = (context->getOneHourAvgTemp() * ((float) currentMinute / (currentMinute + 1))) + (currentTmp / (currentMinute + 1));
		context->setOneHourAvgTemp(avgTemp);
	}
	/* verify humidity data */
	if(currentHum < MIN_ACCEPTABLE_HUM || currentHum > MAX_ACCEPTABLE_HUM || isnan(currentHum)) {
		/* log the invalid value */
		char currentHumStr[10];
		char str[80];
		dtostrf(currentHum, 6, 2, currentHumStr);
		sprintf(str, "ts: %d, error: invalid humidity, value: %s", now(), currentHumStr);
		logError->insertError(str);
		free(currentHumStr);
		free(str);
	}
	else {
		/* refresh max and min values of humidity */
		if(currentHum > context->getOneHourMaxHum()) context->setOneHourMaxHum(currentHum);
		if(currentHum < context->getOneHourMinHum()) context->setOneHourMinHum(currentHum);
		/* refresh humidity averages */
		float avgHum = (context->getOneHourAvgHum() * ((float) currentMinute / (currentMinute + 1))) + (currentHum / (currentMinute + 1));
		context->setOneHourAvgHum(avgHum);
	}
	context->setCurrentMinute(currentMinute + 1);
	/* set next minute time event */
	uint8_t nextMinute = (minute() + 1) % 60;
	uint8_t eventHour = hour();
	if(nextMinute == 0) {
		eventHour = (eventHour + 1) % 24;
	}
	/* reprogram next event */
	minEventAlarmId = Alarm.alarmOnce(eventHour, nextMinute, 10, minTimeEventHandlerWrapper);
	/* save current context */
	// context->saveContext();
#ifdef SIMULATION
	adjustTime(60);
#endif
}

void TimeEventsHandler::hourTimeEventHandler() {

#ifdef DEBUG
	// Serial.println("\r\nhourTimeEventHandler");
	// printTime();
#endif
	uint8_t currentHour = context->getCurrentHour();

	datalogger->appendLineInFile("/hour.csv", context->getCurrentContextString());

	/* verify temperature data */
	if(context->getOneHourMinTemp() < MIN_ACCEPTABLE_TMP || context->getOneHourMaxTemp() > MAX_ACCEPTABLE_TMP ||
	   context->getOneHourAvgTemp()	< MIN_ACCEPTABLE_TMP || context->getOneHourAvgTemp() > MAX_ACCEPTABLE_TMP) {
		/* log the invalid value */
		char minTmpStr[10];
		char maxTmpStr[10];
		char avgTmpStr[10];
		char str[120];
		dtostrf(context->getOneHourMinTemp(), 6, 2, minTmpStr);
		dtostrf(context->getOneHourMaxTemp(), 6, 2, maxTmpStr);
		dtostrf(context->getOneHourAvgTemp(), 6, 2, avgTmpStr);
		sprintf(str, "ts: %d, error: invalid hourly temperature, values:(min: %s | max: %s | avg: %s)", now(), minTmpStr, maxTmpStr, avgTmpStr);
		logError->insertError(str);
	}
	else {
		/* refresh max and min values of temperature */
		if(context->getOneHourMaxTemp() > context->getOneDayMaxTemp()) context->setOneDayMaxTemp(context->getOneHourMaxTemp());
		if(context->getOneHourMinTemp() < context->getOneDayMinTemp()) context->setOneDayMinTemp(context->getOneHourMinTemp());
		/* refresh temperature averages */
		float avgTemp = (context->getOneDayAvgTemp() * ((float) currentHour / (currentHour + 1))) + (context->getOneHourAvgTemp() / (currentHour + 1));
		context->setOneDayAvgTemp(avgTemp);
	}

	/* verify humidity data */
	if(context->getOneHourMinHum() < MIN_ACCEPTABLE_HUM || context->getOneHourMaxHum() > MAX_ACCEPTABLE_HUM ||
	   context->getOneHourAvgHum() < MIN_ACCEPTABLE_HUM || context->getOneHourAvgHum() > MAX_ACCEPTABLE_HUM) {
		/* log the invalid value */
		char minHumStr[10];
		char maxHumStr[10];
		char avgHumStr[10];
		char str[120];
		dtostrf(context->getOneHourMinHum(), 6, 2, minHumStr);
		dtostrf(context->getOneHourMaxHum(), 6, 2, maxHumStr);
		dtostrf(context->getOneHourAvgHum(), 6, 2, avgHumStr);
		sprintf(str, "ts: %d, error: invalid hourly humidity, values:(min: %s | max: %s | avg: %s)", now(), minHumStr, maxHumStr, avgHumStr);
		logError->insertError(str);
	}
	else {
		/* refresh max and min values of humidity */
		if(context->getOneHourMaxHum() > context->getOneDayMaxHum()) context->setOneDayMaxHum(context->getOneHourMaxHum());
		if(context->getOneHourMinHum() < context->getOneDayMinHum()) context->setOneDayMinHum(context->getOneHourMinHum());
		/* refresh humidity averages */
		float avgHum = (context->getOneDayAvgHum() * ((float) currentHour / (currentHour + 1))) + (context->getOneHourAvgHum() / (currentHour + 1));
		context->setOneDayAvgHum(avgHum);
	}
	context->resetHourContext();
	context->setCurrentHour(currentHour + 1);
	/* set next hour time event */
	uint8_t nextHour = (hour() + 1) % 24;
	/* reprogram next event */
	hourEventAlarmId = Alarm.alarmOnce(nextHour, 0, 20, hourTimeEventHandlerWrapper);
}

void TimeEventsHandler::dailyTimeEventHandler() {
#ifdef DEBUG
	Serial.println("dailyTimeEventHandler");
	printTime();
	if(now() > 1518788760) {
		Serial.println("simulating power failure!");
		stopTimeEvents();
		/* print current context */
		Serial.println("**** current context ****");
		context->printContextSerial();
		Serial.println("*************************");
		/* print stored context */
		File file = SPIFFS.open("/context.txt", "r");
		if(file) {
			Serial.println("**** stored context ****");
			while(file.available())
				Serial.println(file.readStringUntil('\n'));
			Serial.println("************************");
			file.close();
		}
		/* change device state */
		context->changeState(Context::State::FAILED);
		return;
	}
#endif
	uint16_t currentDay = context->getCurrentDay();
	/* verifies if it is the last experiment day */
	if(currentDay >= EXPERIMENT_DURATION) {
		/* change device state */
		context->changeState(Context::State::STOPPED);
		/* stop all time events */
		stopTimeEvents();
	}
	else {
		uint16_t duration = 0;
		float avgTemp = context->getOneDayAvgTemp();
		float avgHum = context->getOneDayAvgHum();
		// program for auto irrigation
		context->setIsAutoIrrigationSuspended(false);
		/* verify temperature and humidity values */
		if(avgTemp < MIN_ACCEPTABLE_TMP || avgTemp > MAX_ACCEPTABLE_TMP ||
		   avgHum < MIN_ACCEPTABLE_HUM || avgHum > MAX_ACCEPTABLE_HUM) {
			duration = context->getLastValidAutoIrrigationDuration();
			char line[200];
			sprintf(line,"ts: %d, error: invalid average temperature and/or humidity values, using last valid irrigation duration: %d s", now(), duration);
			/* log the invalid value */
			datalogger->appendLineInFile("/day.csv", line);
			logError->insertError(line);

		}
		else {
			/* calculates irrigation parameters */
			float es = 0.6108 * exp((double) (17.27 * avgTemp)/(avgTemp + 237.3));
			float ea = (es * avgHum) / 100;
			float eto = (2.5982 * pow((1 + (avgTemp / 25)), 2) * (1 - (ea/es))) + 0.7972;
			float etc = eto * kc[currentDay];
			float ll = etc * 0.1 * sqrt(35);
			float lb = ll / efc;
			/* calculate irrigation time in seconds (hour . seconds) */
			duration = ((lb / ia) * 3600);
			context->setLastValidAutoIrrigationDuration(duration);
			/* write current hour values to file */
			char line[120];
			char kcStr[10];
			char etoStr[10];
			char tmpAvgStr[10];
			char humAvgStr[10];
			dtostrf(kc[currentDay], 6, 3, kcStr);
			dtostrf(eto, 6, 3, etoStr);
			dtostrf(avgTemp, 6, 2, tmpAvgStr);
			dtostrf(avgHum, 6, 2, humAvgStr);
			sprintf(line,"%d,%s,%s,%s,%s,%d", now(), tmpAvgStr, humAvgStr, kcStr, etoStr, duration);
			datalogger->appendLineInFile("/day.csv", line);
		}
		context->setAutoIrrigationDuration(duration);
		context->setCurrentDay(currentDay + 1);
		context->resetDayContext();
		context->saveContext();
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
	sprintf(line,"%d,%d,%d", startTime, stopTime, (stopTime - startTime));
	// write current hour values to file
	datalogger->appendLineInFile("/aut-irrig.csv", line);
	// free(line);
}

void TimeEventsHandler::printTime() {
	char time[] = "00:00";
	Serial.print(day());
	Serial.write('/');
	Serial.print(month());
	Serial.write('/');
	Serial.print(year());
	Serial.print(" | ");
	sprintf(time, "%02i:%02i", hour(), minute());
	Serial.print(time);
	uint32_t free = system_get_free_heap_size();
	Serial.print(" | available mem: ");
	Serial.print(free);
	Serial.println();
}

