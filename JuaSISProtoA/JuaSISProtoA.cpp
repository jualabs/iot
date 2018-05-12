/*
	Sketch title

	Describe what it does in layman's terms.  Refer to the components
	attached to the various pins.

	The circuit:
	* list the components attached to each input
	* list the components attached to each output

	Created day month year
	By author's name
	Modified day month year
	By author's name

	http://url/of/online/tutorial.cc

*/
#include <Arduino.h>

#include "Actuators.h"
#include "ButtonEventsHandler.h"
#include "Datalogger.h"
#include "SystemState.h"
#include "Sensors.h"
//#include "time_events_handler.h"

Datalogger dl;
SystemState ss;
Sensors sensors;
Actuators actuators;

void setup()
{
	//initBtnEventsHandler();
	//initTimeEventsHandler();
	delay(5000);
}

void loop()
{
	ss.stateLedUpdate();
	//checkBtnEvents();
	//checkTimeEvents();
}
