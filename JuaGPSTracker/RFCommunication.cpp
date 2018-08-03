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
	for(int i = 0; i < (uint8_t) RF_EVENTS::NUM_EVENTS; i++) {
		lastPressTS[i] = 0;
		rfEvents[i] = false;
	}
}

void RFCommunication::init() {
	rfController.enableReceive(0);
}

void RFCommunication::loop() {
	  if (rfController.available()) {
	    int value = rfController.getReceivedValue();
	    if (value == 0) {
	      Serial.print("Unknown encoding");
	    }/*
	    else if (value == code[(uint8_t) RF_EVENTS::BTN_0]) {
	    	if(millis() - lastPressTS[(uint8_t) RF_EVENTS::BTN_0] > 5000) {
	    		Serial.print("BTN_0");
	    		lastPressTS[(uint8_t) RF_EVENTS::BTN_0] = millis();
	    		rfEvents[(uint8_t) RF_EVENTS::BTN_0] = true;
	    	}
	    }
	    else if (value == code[(uint8_t) RF_EVENTS::BTN_1]) {
	    	if(millis() - lastPressTS[(uint8_t) RF_EVENTS::BTN_1] > 5000) {
	    		Serial.print("BTN_1");
	    		lastPressTS[(uint8_t) RF_EVENTS::BTN_1] = millis();
	    		rfEvents[(uint8_t) RF_EVENTS::BTN_1] = true;
	    	}
	    }*/
	    else {
	      Serial.print("Received ");
	      Serial.print( rfController.getReceivedValue() );
	      Serial.print(" / ");
	      Serial.print( rfController.getReceivedBitlength() );
	      Serial.print("bit ");
	      Serial.print("Protocol: ");
	      Serial.println( rfController.getReceivedProtocol() );
	    }
	    rfController.resetAvailable();
	  }
}

bool RFCommunication::getRFEvent(RF_EVENTS evt) {
	return rfEvents[(uint8_t) evt];
}

void RFCommunication::setRFEvent(RF_EVENTS evt) {
	rfEvents[(uint8_t) evt] = true;
}

void RFCommunication::clearRFEvent(RF_EVENTS evt) {
	rfEvents[(uint8_t) evt] = false;
}
