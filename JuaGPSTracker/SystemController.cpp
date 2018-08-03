#include "SystemController.h"

SystemController::SystemController() : ui(UI::getInstance()), rfComm(RFCommunication::getInstance()), gps(GPSInterface::getInstance()),
									   comm(Communication::getInstance()), teh(TimeEventsHandler::getInstance()) {
	currentSystemState = SYSTEM_STATE::CONNECTING_TO_GPS;
	lastAttempt = 0;
};

void SystemController::setup() {
	/* configure serial port */
	Serial.begin(115200);
	comm->init();
	teh->initPeriodicTimeEvents();
	// rfComm->init();
}

void SystemController::loop() {
	/* loop for time events */
	teh->loop();
	/* loop for communication */
	comm->loop();
	/* loop for user interface */
	ui->loop();
	/* loop for rf interface */
	// rfComm->loop();
	/* main state machine loop */
	switch(currentSystemState) {
		case SYSTEM_STATE::CONNECTING_TO_GPS:
			/* turn GPS on */
			Serial.print("connecting to GPS...");
			ui->setUIState(UI::UI_STATE::CONNECTING_TO_GPS);
			gps->init();
			Serial.println("[OK]");
			/* change state */
			currentSystemState = SYSTEM_STATE::CONNECTING_TO_NETWORK;
			Serial.print("connecting to network...");
			ui->setUIState(UI::UI_STATE::NETWORK_CONNECTION);
			lastAttempt = 0;
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
			/* change state */
			teh->startPeriodicTimeEvents();
			ui->setUIState(UI::UI_STATE::NORMAL_OPERATION);
			currentSystemState = SYSTEM_STATE::NORMAL_OPERATION;
			break;
		case SYSTEM_STATE::NORMAL_OPERATION:
			if(rfComm->getRFEvent(RFCommunication::RF_EVENTS::BTN_0) == true) {
				rfComm->clearRFEvent(RFCommunication::RF_EVENTS::BTN_0);
				comm->sendData("{\"btn0\":1}");
				/* clear rf button signal in 10 seconds */
				teh->startSporadicTimeEvents(TimeEventsHandler::SPORADIC_TIME_EVENTS::CLEAR_BTN_0, CLEAR_BTN_PERIOD);
			}
			if(rfComm->getRFEvent(RFCommunication::RF_EVENTS::BTN_1) == true) {
				rfComm->clearRFEvent(RFCommunication::RF_EVENTS::BTN_1);
				comm->sendData("{\"btn1\":1}");
				/* clear rf button signal in 10 seconds */
				teh->startSporadicTimeEvents(TimeEventsHandler::SPORADIC_TIME_EVENTS::CLEAR_BTN_1, CLEAR_BTN_PERIOD);
			}
			if(teh->getTimeEvent(TimeEventsHandler::SPORADIC_TIME_EVENTS::CLEAR_BTN_0) == true) {
				teh->clearTimeEvent(TimeEventsHandler::SPORADIC_TIME_EVENTS::CLEAR_BTN_0);
				comm->sendData("{\"btn0\":0}");
			}
			if(teh->getTimeEvent(TimeEventsHandler::SPORADIC_TIME_EVENTS::CLEAR_BTN_1) == true) {
				teh->clearTimeEvent(TimeEventsHandler::SPORADIC_TIME_EVENTS::CLEAR_BTN_1);
				comm->sendData("{\"btn1\":0}");
			}
			if(teh->getTimeEvent(TimeEventsHandler::PERIODIC_TIME_EVENTS::SEND_GPS_DATA) == true) {
				teh->clearTimeEvent(TimeEventsHandler::PERIODIC_TIME_EVENTS::SEND_GPS_DATA);
				comm->sendData(gps->getData());
			}
			break;
	}
}
