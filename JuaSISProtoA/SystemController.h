#ifndef __SYSTEM_STATE_H__
#define __SYSTEM_STATE_H__

#define STATE_LED D4

// led library
#include <Arduino.h>
#include <jled.h>
#include "Context.h"
#include "Actuators.h"
#include "ButtonEventsHandler.h"
#include "Datalogger.h"
#include "Sensors.h"
#include "SystemController.h"
#include "TimeEventsHandler.h"
#include "RTC.h"

class SystemController {
	public:
		SystemController();
		void loop();
		void stateLedUpdate();
		void changeState(Context::State toState);
		char* getCurrentContextString();
		const Context* getContext() const;
		Actuators* getActuators() const;
		Sensors* getSensors() const;
		RTC* getRTC() const;
		Datalogger* getDatalogger() const;

	private:
		void recoverContextFromFile();
		JLed stateLed;
		Context ctx;
		Datalogger dl;
		Sensors sensors;
		Actuators actuators;
		ButtonEventsHandler beh;
		TimeEventsHandler teh;
		RTC rtc;
};

#endif

