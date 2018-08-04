#include "TimeEventsHandler.h"

void timeEventsHandlerWrapper() {
	TimeEventsHandler::getInstance()->timeEventsHandler();
}
TimeEventsHandler* TimeEventsHandler::pInstance = nullptr;

TimeEventsHandler* TimeEventsHandler::getInstance() {
   if (!pInstance)
      pInstance = new TimeEventsHandler();

   return pInstance;
}

TimeEventsHandler::TimeEventsHandler() {
	for(int i = 0; i < (uint8_t) TIME_EVENTS::NUM_EVENTS; i++) {
		timeEvents[i] = false;
		alarmIds[i] = 0;
	}
}

void TimeEventsHandler::loop() {
	Alarm.delay(10);
}

void TimeEventsHandler::initTimeEvents() {
	/* configure the GPS send data alarm */
	alarmIds[(uint8_t) TIME_EVENTS::SEND_GPS_DATA] = Alarm.timerRepeat(SEND_GPS_DATA_PERIOD, timeEventsHandlerWrapper);
	Alarm.disable(alarmIds[(uint8_t) TIME_EVENTS::SEND_GPS_DATA]);
}

void TimeEventsHandler::startTimeEvents() {
	/* start all alarms */
	for(int i = 0; i < (uint8_t) TIME_EVENTS::NUM_EVENTS; i++) {
		Alarm.enable(alarmIds[i]);
	}
}

void TimeEventsHandler::stopTimeEvents() {
	/* stop all alarms */
	for(int i = 0; i < (uint8_t) TIME_EVENTS::NUM_EVENTS; i++) {
		Alarm.disable(alarmIds[i]);
	}
}

bool TimeEventsHandler::getTimeEvent(TIME_EVENTS evt) {
	return timeEvents[(uint8_t) evt];
}

void TimeEventsHandler::setTimeEvent(TIME_EVENTS evt) {
	timeEvents[(uint8_t) evt] = true;
}

void TimeEventsHandler::clearTimeEvent(TIME_EVENTS evt) {
	timeEvents[(uint8_t) evt] = false;
}

void TimeEventsHandler::timeEventsHandler() {
	AlarmId currentAlarm = Alarm.getTriggeredAlarmId();
	for(int i = 0; i < (uint8_t) TIME_EVENTS::NUM_EVENTS; i++) {
		if(currentAlarm == alarmIds[i]) {
			timeEvents[i] = true;
			break;
		}
	}
}
