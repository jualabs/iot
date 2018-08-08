#include "RFCommunication.h"


RFCommunication* comm;

/* global static pointer used to ensure a single instance of the class. */
RFCommunication* RFCommunication::pInstance = nullptr;

/*
 * 	This function is called to create an instance of the class.
 *  Calling the constructor publicly is not allowed. The constructor
 *  is private and is only called by this getInstance() function.
 */
RFCommunication* RFCommunication::getInstance() {
	if (!pInstance)   /* Only allow one instance of class to be generated. */
		pInstance = new RFCommunication();

	return pInstance;
}

RFCommunication::RFCommunication() : rfController(RCSwitch()) {
	/* TODO: guardar informacoes dos codigos dos controles RF no sistema de arquivos e criar um mecanismo de
	 * 		 configuracao de novos controles
	 */
	rfCommData[0].code = 146289317;
	rfCommData[1].code = 146289301;
	for(int i = 0; i < NUM_RF_EVENTS; i++) {
		rfCommData[i].eventFlag = false;
	}
}

void RFCommunication::init() {
	rfController.enableReceive(0);
}

void RFCommunication::loop() {
	if (rfController.available()) {
		int value = rfController.getReceivedValue();
		/* if 0 is a invalid encoding */
		if (value == 0) {
			Serial.print("Unknown encoding");
		}
		/* valid value */
		else {
			Serial.print("Received ");
			Serial.print(rfController.getReceivedValue());
			Serial.print(" / ");
			Serial.print(rfController.getReceivedBitlength());
			Serial.print("bit ");
			Serial.print("Protocol: ");
			Serial.println(rfController.getReceivedProtocol());
			for (int i = 0; i < NUM_RF_EVENTS; i++) {
				/* verify if it is a valid known value */
				if(value == rfCommData[i].code) {
					Serial.println("known value");
					/* set alert */
					rfCommData[i].eventFlag = true;
				}
			}
		}
		rfController.resetAvailable();
	}
}

uint8_t RFCommunication::getRFEventState(uint8_t eventId) {
	return rfCommData[eventId].eventFlag == true ? 1 : 0;
}

void RFCommunication::clearRFEventState(uint8_t eventId) {
	rfCommData[eventId].eventFlag = false;
}
