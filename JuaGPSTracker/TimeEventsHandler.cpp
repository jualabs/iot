#include "TimeEventsHandler.h"

void periodicTimeEventsHandlerWrapper() {
	TimeEventsHandler::getInstance()->periodicTimeEventsHandler();
}

void sporadicTimeEventsHandlerWrapper() {
	TimeEventsHandler::getInstance()->sporadicTimeEventsHandler();
}

TimeEventsHandler* TimeEventsHandler::pInstance = nullptr;

TimeEventsHandler* TimeEventsHandler::getInstance() {
   if (!pInstance)
      pInstance = new TimeEventsHandler();

   return pInstance;
}

TimeEventsHandler::TimeEventsHandler() {
	for(int i = 0; i < (uint8_t) PERIODIC_TIME_EVENTS::NUM_EVENTS; i++) {
		periodicTimeEvents[i] = false;
		periodicAlarmIds[i] = 0;
	}
	for(int i = 0; i < (uint8_t) SPORADIC_TIME_EVENTS::NUM_EVENTS; i++) {
		sporadicTimeEvents[i] = false;
		sporadicAlarmIds[i] = 0;
	}
}

void TimeEventsHandler::loop() {
	Alarm.delay(10);
}

void TimeEventsHandler::initPeriodicTimeEvents() {
	/* configure the GPS send data alarm */
	periodicAlarmIds[(uint8_t) PERIODIC_TIME_EVENTS::SEND_GPS_DATA] = Alarm.timerRepeat(SEND_GPS_DATA_PERIOD, periodicTimeEventsHandlerWrapper);
	Alarm.disable(periodicAlarmIds[(uint8_t) PERIODIC_TIME_EVENTS::SEND_GPS_DATA]);
}


void TimeEventsHandler::startPeriodicTimeEvents() {
	/* start all alarms */
	for(int i = 0; i < (uint8_t) PERIODIC_TIME_EVENTS::NUM_EVENTS; i++) {
		Alarm.enable(periodicAlarmIds[i]);
	}
}

void TimeEventsHandler::startSporadicTimeEvents(SPORADIC_TIME_EVENTS evt, uint32_t time) {
	/* start all alarms */
	sporadicAlarmIds[(uint8_t) evt] = Alarm.timerOnce(time, sporadicTimeEventsHandlerWrapper);
}

void TimeEventsHandler::stopPeriodicTimeEvents() {
	/* stop all alarms */
	for(int i = 0; i < (uint8_t) PERIODIC_TIME_EVENTS::NUM_EVENTS; i++) {
		Alarm.disable(periodicAlarmIds[i]);
	}
}

bool TimeEventsHandler::getTimeEvent(PERIODIC_TIME_EVENTS evt) {
	return periodicTimeEvents[(uint8_t) evt];
}

void TimeEventsHandler::setTimeEvent(PERIODIC_TIME_EVENTS evt) {
	periodicTimeEvents[(uint8_t) evt] = true;
}

void TimeEventsHandler::clearTimeEvent(PERIODIC_TIME_EVENTS evt) {
	periodicTimeEvents[(uint8_t) evt] = false;
}

bool TimeEventsHandler::getTimeEvent(SPORADIC_TIME_EVENTS evt) {
	return sporadicTimeEvents[(uint8_t) evt];
}

void TimeEventsHandler::setTimeEvent(SPORADIC_TIME_EVENTS evt) {
	sporadicTimeEvents[(uint8_t) evt] = true;
}

void TimeEventsHandler::clearTimeEvent(SPORADIC_TIME_EVENTS evt) {
	sporadicTimeEvents[(uint8_t) evt] = false;
}

void TimeEventsHandler::periodicTimeEventsHandler() {
	AlarmId currentAlarm = Alarm.getTriggeredAlarmId();
	for(int i = 0; i < (uint8_t) PERIODIC_TIME_EVENTS::NUM_EVENTS; i++) {
		if(currentAlarm == periodicAlarmIds[i]) {
			periodicTimeEvents[i] = true;
			break;
		}
	}
}

void TimeEventsHandler::sporadicTimeEventsHandler() {
	AlarmId currentAlarm = Alarm.getTriggeredAlarmId();
	for(int i = 0; i < (uint8_t) SPORADIC_TIME_EVENTS::NUM_EVENTS; i++) {
		if(currentAlarm == sporadicAlarmIds[i]) {
			sporadicTimeEvents[i] = true;
			break;
		}
	}
}
