#ifndef __ACTUATORS_H__
#define __ACTUATORS_H__

#include <Arduino.h>

// relays definitions
#define MAN_PUMP_RELAY  D7
#define AUT_PUMP_RELAY  D8


class Actuators {
	public:
		static Actuators* getInstance();
		void setManPump(bool state);
		void setAutoPump(bool state);
	private:
		Actuators();  // private so that it can  not be called
		Actuators(const Actuators&) = delete;
		Actuators& operator=(const Actuators&) = delete;
		static Actuators* pInstance;

};

#endif
