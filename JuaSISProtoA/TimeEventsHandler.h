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
#include "LogError.h"
#include "Parameters.h"

#define SIMULATION

extern "C" {
#include "user_interface.h"
}

class TimeEventsHandler {
	public:
		static TimeEventsHandler* getInstance();
		void initTimeEvents();
		void checkTimeEvents();
		void startTimeEvents();
		void stopTimeEvents();
		// time events handlers
		void minTimeEventHandler();
		void hourTimeEventHandler();
		void dailyTimeEventHandler();
		void startAutoIrrigationTimeEventHandler();
		void stopAutoIrrigationTimeEventHandler();
		void printTime();
	private:
		TimeEventsHandler();  // private so that it can  not be called
		TimeEventsHandler(const TimeEventsHandler&) = delete;
		TimeEventsHandler& operator=(const TimeEventsHandler&) = delete;
		static TimeEventsHandler* pInstance;
		Context* context;
		Actuators* actuators;
		Sensors* sensors;
		Datalogger* datalogger;
		LogError* logError;
		AlarmId minEventAlarmId;
		AlarmId hourEventAlarmId;
		AlarmId dailyEventAlarmId;
		AlarmId startIrrigationEventAlarmId;
};

#endif
