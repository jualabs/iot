#ifndef __SYSTEM_STATE_H__
#define __SYSTEM_STATE_H__

#define STATE_LED D4

// led library
#include <Arduino.h>
#include <jled.h>
#include "Context.h"

class SystemState {
	public:
		SystemState();
		enum state {STAND_BY, RUNNING, GET_DATA, FAILED};
		void stateLedUpdate();
		void changeState(state toState);
		void currentStateToString(char *str);

	private:
		void recoverContextFromFile();
		JLed stateLed;
		Context ctx;
		state currentState;
};

#endif

