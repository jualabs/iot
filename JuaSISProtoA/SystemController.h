#ifndef __SYSTEM_STATE_H__
#define __SYSTEM_STATE_H__

// led library
#include <Arduino.h>
#include "Context.h"
#include "RTC.h"
#include "Datalogger.h"
#include "Sensors.h"
#include "Actuators.h"
#include "TimeEventsHandler.h"
#include "ButtonEventsHandler.h"

class SystemController {
	public:
		SystemController();
		void setup();
		void loop();
		void stateLedUpdate();

	private:
		void recoverContextFromFile();
		std::unique_ptr<Context> context;
		RTC rtc;
		Datalogger datalogger;
		Sensors sensors;
		Actuators actuators;
		TimeEventsHandler timeEventsHandler;
		ButtonEventsHandler buttonEventsHandler;
};

#endif

