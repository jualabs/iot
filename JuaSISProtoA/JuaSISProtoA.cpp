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
#include "SystemController.h"

SystemController sc;

void setup()
{
	Serial.begin(9600);
	sc.setup();
}

void loop()
{
	sc.loop();
	delay(1000);
}
