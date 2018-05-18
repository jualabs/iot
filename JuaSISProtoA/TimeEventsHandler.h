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
		TimeEventsHandler(Context ctx, Actuators act, Sensors sens, Datalogger dl, RTC rtc);
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
		Context context;
		Actuators actuators;
		Sensors sensors;
		Datalogger datalogger;
		RTC rtc;
		enum class ALARM_IDS {MIN_EVENT, HOUR_EVENT, DAILY_EVENT, START_IRRIGATION_EVENT, SIZE};
		AlarmId alarmIds[(uint8_t) ALARM_IDS::SIZE];
		void initAlarms();
};

#endif
