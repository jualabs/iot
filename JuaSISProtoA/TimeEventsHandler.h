#ifndef __TIME_EVENTS_HANDLER_H__
#define __TIME_EVENTS_HANDLER_H__

#include <Arduino.h>
#include <Time.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <math.h>
#include "SystemController.h"
#include "Parameters.h"

class TimeEventsHandler {
	public:
		TimeEventsHandler(SystemController* sc);
		void checkTimeEvents();
	private:
		SystemController* sc;
		enum class ALARM_IDS {MIN_EVENT, HOUR_EVENT, DAILY_EVENT, START_IRRIGATION_EVENT, SIZE};
		AlarmId alarmIds[ALARM_IDS::SIZE];
		void initAlarms();
		void cfgMinAndHourTimerEventHandler();
		void minTimeEventHandler();
		void hourTimeEventHandler();
		void dailyTimeEventHandler();
		void startAutoIrrigationTimeEventHandler();
		void stopAutoIrrigationTimeEventHandler();
};

#endif
