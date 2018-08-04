#ifndef __TIME_EVENTS_HANDLER_H__
#define __TIME_EVENTS_HANDLER_H__

#include <Arduino.h>
#include <Time.h>
#include <TimeLib.h>
#include <TimeAlarms.h>

#define SEND_GPS_DATA_PERIOD 10

class TimeEventsHandler {
	public:
		static TimeEventsHandler* getInstance();
		enum class TIME_EVENTS {SEND_GPS_DATA, NUM_EVENTS};
		void loop();
		void initTimeEvents();
		void startTimeEvents();
		void stopTimeEvents();
		bool getTimeEvent(TIME_EVENTS evt);
		void setTimeEvent(TIME_EVENTS evt);
		void clearTimeEvent(TIME_EVENTS evt);
		void timeEventsHandler();
	private:
		TimeEventsHandler();  // private so that it can  not be called
		TimeEventsHandler(const TimeEventsHandler&) = delete;
		TimeEventsHandler& operator=(const TimeEventsHandler&) = delete;
		static TimeEventsHandler* pInstance;
		bool timeEvents[(uint8_t) TIME_EVENTS::NUM_EVENTS];
		AlarmId alarmIds[(uint8_t) TIME_EVENTS::NUM_EVENTS];
};

#endif
