#ifndef __TIME_EVENTS_HANDLER_H__
#define __TIME_EVENTS_HANDLER_H__

#include <Arduino.h>
#include <Time.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include "Context.h"
#include "Actuators.h"
#include "Sensors.h"
#include "Datalogger.h"
#include "RTC.h"
#include "Parameters.h"

class SystemController;

class TimeEventsHandler {
	public:
		static TimeEventsHandler* getInstance();
		void initTimeEvents();
		void checkTimeEvents();
		void startTimeEvents();
		void stopTimeEvents();
		// time events handlers
		void cfgMinAndHourTimerEventHandler();
		void minTimeEventHandler();
		void hourTimeEventHandler();
		void dailyTimeEventHandler();
		void startAutoIrrigationTimeEventHandler();
		void stopAutoIrrigationTimeEventHandler();
	private:
		TimeEventsHandler();  // private so that it can  not be called
		TimeEventsHandler(const TimeEventsHandler&) = delete;
		TimeEventsHandler& operator=(const TimeEventsHandler&) = delete;
		static TimeEventsHandler* pInstance;
		Context* context;
		RTC* rtc;
		Actuators* actuators;
		Sensors* sensors;
		Datalogger* datalogger;
		enum class ALARM_IDS {MIN_EVENT, HOUR_EVENT, DAILY_EVENT, START_IRRIGATION_EVENT, SIZE};
		AlarmId alarmIds[(uint8_t) ALARM_IDS::SIZE];
};

#endif
