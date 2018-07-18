#include "Communication.h"


Communication* comm;

void dataCallbackWrapper(uint32_t *client, const char* topic, uint32_t topic_len, const char *data, uint32_t lenght) {
}

// global static pointer used to ensure a single instance of the class.
Communication* Communication::pInstance = nullptr;

/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this getInstance() function.
 */

Communication* Communication::getInstance() {
	if (!pInstance)   // Only allow one instance of class to be generated.
		pInstance = new Communication();

	return pInstance;
}

Communication::Communication() : ftpSrv() {
	commState = CommunicationState::DISCONNECTED;
	enableCommFlag = false;
	lastVerificationTS = 0;
}

void Communication::enable() {
	enableCommFlag = true;
}

void Communication::disable() {
	enableCommFlag = false;
}

bool Communication::isEnabled() {
	return enableCommFlag;
}

void Communication::checkCommunication() {
	switch (commState) {
	case CommunicationState::DISCONNECTED:
		if(enableCommFlag == true) {
			WiFi.begin(ssid, pass);
			commState = CommunicationState::CONNECTING;
		}
		break;
	case CommunicationState::CONNECTING:
		if(enableCommFlag == false) {
			Serial.println("\r\naborting connection...");
			WiFi.disconnect(true);
			commState = CommunicationState::DISCONNECTED;
		}
		else if(WiFi.status() != WL_CONNECTED) {
			if((millis() - lastVerificationTS) > 500) {
				Serial.print(".");
				lastVerificationTS = millis();
			}
		}
		else {
			/* print ip on serial interface */
			Serial.print("\r\nconnected - ip address: ");
			Serial.println(WiFi.localIP());
			/* open file system */
			if (SPIFFS.begin()) {
				Serial.println("SPIFFS opened!");
				/* username, password for ftp. set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV) */
				ftpSrv.begin(ftpUser, ftpPass);
				commState = CommunicationState::CONNECTED;
			}
			else {
				Serial.println("ERROR: unable to open file system!");
				Serial.println("disconnecting...");
				WiFi.disconnect(true);
				commState = CommunicationState::DISCONNECTED;
			}
		}
		break;
	case CommunicationState::CONNECTED:
		ftpSrv.handleFTP();
		if(enableCommFlag == false) {
			Serial.println("\r\ndisconnecting...");
			WiFi.disconnect(true);
			commState = CommunicationState::DISCONNECTED;
		}
		break;
	}

}
