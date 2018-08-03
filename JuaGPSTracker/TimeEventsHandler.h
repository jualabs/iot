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
		enum class PERIODIC_TIME_EVENTS {SEND_GPS_DATA, NUM_EVENTS};
		enum class SPORADIC_TIME_EVENTS {CLEAR_BTN_0, CLEAR_BTN_1, NUM_EVENTS};
		void loop();
		void initPeriodicTimeEvents();
		void startPeriodicTimeEvents();
		void startSporadicTimeEvents(SPORADIC_TIME_EVENTS evt, uint32_t time);
		void stopPeriodicTimeEvents();
		bool getTimeEvent(PERIODIC_TIME_EVENTS evt);
		bool getTimeEvent(SPORADIC_TIME_EVENTS evt);
		void setTimeEvent(PERIODIC_TIME_EVENTS evt);
		void setTimeEvent(SPORADIC_TIME_EVENTS evt);
		void clearTimeEvent(PERIODIC_TIME_EVENTS evt);
		void clearTimeEvent(SPORADIC_TIME_EVENTS evt);
		void periodicTimeEventsHandler();
		void sporadicTimeEventsHandler();
	private:
		TimeEventsHandler();  // private so that it can  not be called
		TimeEventsHandler(const TimeEventsHandler&) = delete;
		TimeEventsHandler& operator=(const TimeEventsHandler&) = delete;
		static TimeEventsHandler* pInstance;
		bool periodicTimeEvents[(uint8_t) PERIODIC_TIME_EVENTS::NUM_EVENTS];
		AlarmId periodicAlarmIds[(uint8_t) PERIODIC_TIME_EVENTS::NUM_EVENTS];
		bool sporadicTimeEvents[(uint8_t) SPORADIC_TIME_EVENTS::NUM_EVENTS];
		AlarmId sporadicAlarmIds[(uint8_t) SPORADIC_TIME_EVENTS::NUM_EVENTS];
};

#endif
