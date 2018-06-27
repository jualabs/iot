#include "SystemController.h"

SystemController::SystemController() : context(Context::getInstance()), rtc(DS1302RTC(D0, D2, D1)), datalogger(Datalogger::getInstance()),
									   sensors(Sensors::getInstance()), actuators(Actuators::getInstance()),
									   timeEventsHandler(TimeEventsHandler::getInstance()), comm(Communication::getInstance()),
									   buttonEventsHandler(ButtonEventsHandler::getInstance()) {};

void SystemController::setup() {
	/* initialize communication */
	// comm->init();
	setupTime();
	// sensors->verifySensors();
	/* initialize contexts */
	context->initContext();
	/* initialize time events */
	timeEventsHandler->initTimeEvents();
	/* initialize the buttons events */
	buttonEventsHandler->initButtons();

	String str = "";
	Dir dir = SPIFFS.openDir("/");
	while (dir.next()) {
	    str += dir.fileName();
	    str += " / ";
	    str += dir.fileSize();
	    str += "\r\n";
	}
	Serial.print(str);
}

void SystemController::loop() {
	stateLedUpdate();
	timeEventsHandler->checkTimeEvents();
	buttonEventsHandler->checkButtonEvents();
}

void SystemController::stateLedUpdate() {
	context->getStateLed()->Update();
}

void SystemController::setupTime() {
#ifndef SIMULATION
	setSyncProvider(rtc.get);
	if(timeStatus() != timeSet) {
		Serial.println("Unable to sync with the RTC");
	}
	else {
		Serial.println("RTC has set the system time");
	}
#else
	setTime(22, 59, 0, 31, 12, 2017);
	// setTime(12, 59, 0, 9, 2, 2018);
	timeEventsHandler->printTime();
#endif
}

