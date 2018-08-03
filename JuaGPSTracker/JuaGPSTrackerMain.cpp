#include <Arduino.h>
#include "SystemController.h"

SystemController sc;

void setup() {
	sc.setup();
}

void loop() {
	sc.loop();
}
