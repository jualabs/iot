#include "SystemController.h"

extern void vm_reboot_normal_start(void);

void vm_reset(void) {
	delay(500);
	vm_reboot_normal_start();
}

boolean vm_reset_wrap(void* userData) {
	vm_reset();
	return true;
}

void reset(void) {
	LTask.remoteCall(vm_reset_wrap, NULL);
}

SystemController::SystemController() : ui(UI::getInstance()), gps(GPSInterface::getInstance()),
									   comm(Communication::getInstance()), teh(TimeEventsHandler::getInstance()) {
	currentSystemState = SYSTEM_STATE::INIT;
	lastAttempt = 0;
	numDataServerRetries = 0;
};

void SystemController::setup() {
	// rfComm->init();
	/* configure serial port */
	Serial.begin(115200);
	/* turn GPS on */
	gps->init();
	comm->init();
	teh->initTimeEvents();
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
		case SYSTEM_STATE::INIT:
			ui->setUIState(UI::UI_STATE::INIT);
			lastAttempt = 0;
			numDataServerRetries = 0;
			numGPRSRetries = 0;
			Serial.print("connecting to network...");
			currentSystemState = SYSTEM_STATE::CONNECTING_TO_NETWORK;
			break;
		case SYSTEM_STATE::CONNECTING_TO_NETWORK:
			ui->setUIState(UI::UI_STATE::CONNECTING_TO_NETWORK);
			if((millis() - lastAttempt) > 500) {
				lastAttempt = millis();
				if(comm->connectToNetwork() == true) {
					numGPRSRetries = 0;
					Serial.println("[OK]");
					Serial.print("waiting signal establishment...");
					/* change state */
					currentSystemState = SYSTEM_STATE::CONNECTED_TO_NETWORK;
				}
				else {
					Serial.print(".");
					numGPRSRetries++;
					if(numGPRSRetries > MAX_GPRS_CONNECTION_RETRIES) {
						reset();
					}
				}
			}
			break;
		case SYSTEM_STATE::CONNECTED_TO_NETWORK:
			ui->setUIState(UI::UI_STATE::CONNECTED_TO_NETWORK);
			/* wait for connection establishment */
			if((millis() - lastAttempt) > 10000) {
				lastAttempt = 0;
				Serial.println("[OK]");
#ifdef MQTT
				currentSystemState = SYSTEM_STATE::CONNECTING_TO_DATA_SERVER;
				Serial.print("connecting to data server...");
#else
				teh->startTimeEvents();
				currentSystemState = SYSTEM_STATE::GPS_NOT_FIXED;
#endif
			}
			break;
		case SYSTEM_STATE::CONNECTING_TO_DATA_SERVER:
			ui->setUIState(UI::UI_STATE::CONNECTING_TO_DATA_SERVER);
			if((millis() - lastAttempt) > 10000) {
				lastAttempt = millis();
				if(comm->connectToDataServer() == true) {
					numDataServerRetries = 0;
					Serial.println("[OK]");
					/* change state */
					currentSystemState = SYSTEM_STATE::CONNECTED_TO_DATA_SERVER;
				}
				else {
				    if(numDataServerRetries < MAX_DATA_SERVER_CONNECTION_RETRIES ) {
						Serial.print("[FAILED] [rc = " );
					    Serial.print(comm->getDataServerCommState());
					    Serial.println( " : retrying in 10 seconds]" );
					    numDataServerRetries++;
				    }
				    else {
				    	Serial.println("reached maximal retries. restarting GPRS connection...");
				    	numDataServerRetries = 0;
				    	currentSystemState = SYSTEM_STATE::INIT;
				    }
				}
			}
			break;
		case SYSTEM_STATE::CONNECTED_TO_DATA_SERVER:
			ui->setUIState(UI::UI_STATE::CONNECTED_TO_DATA_SERVER);
			/* change state */
			lastAttempt = 0;
			teh->startTimeEvents();
			currentSystemState = SYSTEM_STATE::GPS_NOT_FIXED;
			break;
		case SYSTEM_STATE::GPS_NOT_FIXED:
			ui->setUIState(UI::UI_STATE::GPS_NOT_FIXED);
		case SYSTEM_STATE::GPS_FIXED:
			/* verify if it is running on battery */
//			if(LBattery.isCharging() == false) {
//				Serial.println("entering on low power operation mode...");
//				currentSystemState = SYSTEM_STATE::LOW_POWER_OPERATION;
//				teh->clearTimeEvent(TimeEventsHandler::TIME_EVENTS::SEND_GPS_DATA);
//				teh->stopTimeEvents();
//				teh->setTimeEventPeriod(TimeEventsHandler::TIME_EVENTS::SEND_GPS_DATA, 300);
//				teh->startTimeEvents();
//				gps->powerOff();
//				comm->disconnectFromDataServer();
//			}
			if(teh->getTimeEvent(TimeEventsHandler::TIME_EVENTS::SEND_GPS_DATA) == true) {
				/* clear time event */
				teh->clearTimeEvent(TimeEventsHandler::TIME_EVENTS::SEND_GPS_DATA);
#ifdef MQTT
				/* if connection lost with mqtt server */
				if(comm->sendDataMQTT(gps->parseDataJSON()) == false) {
					lastAttempt = 0;
					currentSystemState = SYSTEM_STATE::CONNECTING_TO_DATA_SERVER;
					Serial.print("connecting to data server...");
				}
				else {
					/* verify if it already fixed */
					if(gps->isFixed()) {
						currentSystemState = SYSTEM_STATE::GPS_FIXED;
						ui->setUIState(UI::UI_STATE::GPS_FIXED);
					}
					else {
						currentSystemState = SYSTEM_STATE::GPS_NOT_FIXED;
						ui->setUIState(UI::UI_STATE::GPS_NOT_FIXED);
					}

				}
#else
				while(true) {
					if(millis() - lastAttempt > 1000) {
						lastAttempt = millis();
						if(comm->sendDataHTML(gps->parseDataJSON()) == true) {
							Serial.println("Data sent...");
							/* verify if it already fixed */
							if(gps->isFixed()) {
								currentSystemState = SYSTEM_STATE::GPS_FIXED;
								ui->setUIState(UI::UI_STATE::GPS_FIXED);
							}
							else {
								currentSystemState = SYSTEM_STATE::GPS_NOT_FIXED;
								ui->setUIState(UI::UI_STATE::GPS_NOT_FIXED);
							}
							break;
						}
						else {
							if(numDataServerRetries < MAX_DATA_SERVER_CONNECTION_RETRIES ) {
								Serial.print("[FAILED]");
								Serial.println( " : retrying in 1 seconds]" );
								numDataServerRetries++;
							}
							else {
								Serial.println("reached maximal retries. restarting GPRS connection...");
								numDataServerRetries = 0;
								currentSystemState = SYSTEM_STATE::INIT;
								break;
							}
						}
					}
				}
#endif
			}
			break;
	}
}
