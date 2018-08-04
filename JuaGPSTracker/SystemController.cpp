#include "SystemController.h"

SystemController::SystemController() : ui(UI::getInstance()), rfComm(RFCommunication::getInstance()), gps(GPSInterface::getInstance()),
									   comm(Communication::getInstance()), teh(TimeEventsHandler::getInstance()) {
	currentSystemState = SYSTEM_STATE::INIT;
	lastAttempt = 0;
};

void SystemController::setup() {
	/* configure serial port */
	Serial.begin(115200);
	/* turn GPS on */
	gps->init();
	comm->init();
	teh->initTimeEvents();
	rfComm->init();
}

void SystemController::loop() {
	/* loop for time events */
	teh->loop();
	/* loop for communication */
	comm->loop();
	/* loop for user interface */
	ui->loop();
	/* loop for rf interface */
	rfComm->loop();
	/* main state machine loop */
	switch(currentSystemState) {
		case SYSTEM_STATE::INIT:
			lastAttempt = 0;
			Serial.print("connecting to network...");
			ui->setUIState(UI::UI_STATE::NETWORK_CONNECTION);
			currentSystemState = SYSTEM_STATE::CONNECTING_TO_NETWORK;
			break;
		case SYSTEM_STATE::CONNECTING_TO_NETWORK:
			if((millis() - lastAttempt) > 500) {
				lastAttempt = millis();
				if(comm->connectToNetwork() == true) {
					Serial.println("[OK]");
					Serial.print("waiting signal establishment...");
					/* change state */
					currentSystemState = SYSTEM_STATE::CONNECTED_TO_NETWORK;
				}
				else {
					Serial.print(".");
				}
			}
			break;
		case SYSTEM_STATE::CONNECTED_TO_NETWORK:
			/* wait for connection establishment */
			if((millis() - lastAttempt) > 10000) {
				lastAttempt = 0;
				Serial.println("[OK]");
				currentSystemState = SYSTEM_STATE::CONNECTING_TO_DATA_SERVER;
				Serial.print("connecting to data server...");
				ui->setUIState(UI::UI_STATE::DATA_SERVER_CONNECTION);
			}
			break;
		case SYSTEM_STATE::CONNECTING_TO_DATA_SERVER:
			if((millis() - lastAttempt) > 5000) {
				lastAttempt = millis();
				if(comm->connectToDataServer() == true) {
					Serial.println("[OK]");
					/* change state */
					currentSystemState = SYSTEM_STATE::CONNECTED_TO_DATA_SERVER;
				}
				else {
					Serial.print("[FAILED] [rc = " );
				    Serial.print(comm->getDataServerCommState());
				    Serial.println( " : retrying in 5 seconds]" );
				}
			}
			break;
		case SYSTEM_STATE::CONNECTED_TO_DATA_SERVER:
			lastAttempt = 0;
			/* change state */
			teh->startTimeEvents();
			currentSystemState = SYSTEM_STATE::NORMAL_OPERATION;
			break;
		case SYSTEM_STATE::NORMAL_OPERATION:
			if(teh->getTimeEvent(TimeEventsHandler::TIME_EVENTS::SEND_GPS_DATA) == true) {
				/* clear time event */
				teh->clearTimeEvent(TimeEventsHandler::TIME_EVENTS::SEND_GPS_DATA);
				comm->sendData(gps->getData());
				/* verify if it already fixed */
				if(gps->isFixed()) {
					ui->setUIState(UI::UI_STATE::NORMAL_OPERATION);
				}
				else {
					ui->setUIState(UI::UI_STATE::GPS_NOT_FIXED);
				}
			}
			break;
	}
}
