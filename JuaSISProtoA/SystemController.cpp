#include "SystemController.h"

SystemController::SystemController() : context(Context::getInstance()), rtc(RTC::getInstance()), datalogger(Datalogger::getInstance()),
									   sensors(Sensors::getInstance()), actuators(Actuators::getInstance()),
									   timeEventsHandler(TimeEventsHandler::getInstance()),
									   buttonEventsHandler(ButtonEventsHandler::getInstance()) {};

void SystemController::setup() {
	context->initContext();
	timeEventsHandler->initTimeEvents();
	buttonEventsHandler->initButtons();
}

void SystemController::loop() {
	stateLedUpdate();
	timeEventsHandler->checkTimeEvents();
	buttonEventsHandler->checkButtonEvents();
}

void SystemController::stateLedUpdate() {
	context->getStateLed()->Update();
}

void SystemController::recoverContextFromFile() {

}

