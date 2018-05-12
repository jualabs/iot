#ifndef __ACTUATORS_H__
#define __ACTUATORS_H__

#include <Arduino.h>

// relays definitions
#define MAN_PUMP_RELAY  D7
#define AUT_PUMP_RELAY  D8


class Actuators {
	public:
		Actuators();
		void setManPump(bool state);
		void setAutoPump(bool state);
	private:

};

#endif
