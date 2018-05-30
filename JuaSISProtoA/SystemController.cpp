#include "SystemController.h"

SystemController::SystemController() : context(Context::getInstance()), rtc(DS1302RTC(D0, D2, D1)), datalogger(Datalogger::getInstance()),
									   sensors(Sensors::getInstance()), actuators(Actuators::getInstance()),
									   timeEventsHandler(TimeEventsHandler::getInstance()), comm(Communication::getInstance()),
									   buttonEventsHandler(ButtonEventsHandler::getInstance()) {};

void SystemController::setup() {
	// RTC::getInstance()->verifyRTC();
	setSyncProvider(rtc.get);
	// sensors->verifySensors();
	/* initialize contexts */
	// context->initContext();
	/* initialize time events */
	// timeEventsHandler->initTimeEvents();
	/* initialize the buttons events */
	// buttonEventsHandler->initButtons();
	/* initialize communication */
	comm->init();
}

void SystemController::loop() {
	// stateLedUpdate();
	// timeEventsHandler->checkTimeEvents();
	// buttonEventsHandler->checkButtonEvents();
	Serial.println(now());
}

void SystemController::stateLedUpdate() {
	context->getStateLed()->Update();
}

void SystemController::recoverContextFromFile() {

}

