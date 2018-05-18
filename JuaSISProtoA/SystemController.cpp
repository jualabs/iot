#include "SystemController.h"

SystemController::SystemController() : context(Context()), rtc(RTC()), datalogger(Datalogger()),
									   sensors(Sensors()), actuators(Actuators()),
									   timeEventsHandler(TimeEventsHandler(context, actuators, sensors, datalogger, rtc)),
									   buttonEventsHandler(ButtonEventsHandler(context, actuators, datalogger, rtc, timeEventsHandler)) {};

void SystemController::loop() {
	stateLedUpdate();
	timeEventsHandler.checkTimeEvents();
	buttonEventsHandler.checkButtonEvents();
}

void SystemController::stateLedUpdate() {
	context.getStateLed()->Update();
}

void SystemController::recoverContextFromFile() {

}


