#include "Communication.h"

/* global static pointer used to ensure a single instance of the class. */
Communication* Communication::pInstance = nullptr;

/*
 * 	This function is called to create an instance of the class.
 *  Calling the constructor publicly is not allowed. The constructor
 *  is private and is only called by this getInstance() function.
 */
Communication* Communication::getInstance() {
	if (!pInstance)   /* Only allow one instance of class to be generated. */
		pInstance = new Communication();

	return pInstance;
}

Communication::Communication() : gprsClient(), mqttClient(gprsClient) {
	gprsClient.setTimeout(GPRS_CONNECTION_TIMEOUT);
}

void Communication::init() {
	// mqttClient.setServer(mqttServer, mqttPort);

}

void Communication::loop() {
	// mqttClient.loop();
}

bool Communication::connectToNetwork() {
	return LGPRS.attachGPRS(APN, APN_USER, APN_PASS);
}

bool Communication::connectToDataServer() {
	return mqttClient.connect(mqttClientId, ubidotsToken, "");
}

void Communication::disconnectFromDataServer() {
	return mqttClient.disconnect();
}

bool Communication::isConnectedToDataServer() {
	return mqttClient.connected();
}

int8_t Communication::getDataServerCommState() {
	return (int8_t) mqttClient.state();
}

#if 0

bool Communication::sendDataMQTT(String payload) {
	if(mqttClient.connected()) {
		char payloadChar[256];
		payload.toCharArray(payloadChar, 256);
		mqttClient.publish(ubidotsTopic, payloadChar);
		Serial.print("Published (");
		Serial.print(strlen(payloadChar));
		Serial.print(" bytes): ");
		Serial.println(payloadChar);
		return true;
	}
	else {
		return false;
	}
}
#endif

bool Communication::sendDataHTML(String payload) {

	Serial.print("Sending JSON through HTML: ");
	Serial.println(payload);

	if (gprsClient.connect(UBIDOTS_SERVER, 80)) {
		Serial.println("Connected!");  // Console monitoring
		gprsClient.print(F("POST "));
		gprsClient.print(PATH);
		gprsClient.print(F(" HTTP/1.1\r\n"));
		gprsClient.print(F("Content-Type: application/json\r\n"));
		gprsClient.print(F("Host: "));
		gprsClient.print(UBIDOTS_SERVER);
		gprsClient.print(F("\r\n"));
		gprsClient.print(F("Content-Length: "));
		gprsClient.print(payload.length());
		gprsClient.print(F("\r\n"));
		gprsClient.print(F("\r\n"));
		gprsClient.print(payload);
		gprsClient.println();
#if 1
		uint32_t responseTime = millis();
	    while (true) {
	        if (gprsClient.available()) {
	            char c = gprsClient.read();
	            if (c < 0) {
	                break;
	            }
	            else {
	                Serial.print(c);
	            }
	        }
	        else {
	            if ((millis() - responseTime) > MAX_RESPONSE_TIME){
	                Serial.println("stop Client");
	                gprsClient.stop();
	                break;
	            }
	        }
	    }
		gprsClient.stop();
		return true;
#endif
	}
	else {
		Serial.println(F("Connection failed"));
		gprsClient.stop();
		return false;
	}
}

#if 0
bool Communication::sendDataUDP(String payload) {

	payload = "LinkitONE/1.0|POST|A1E-cl4w5Oltm5tBxLbsY9ucNn3xA1Dsyu|calango-viajante=>udp-teste:30|end\r\n";
	uint16_t payloadLength = payload.length() + 1;

	char* udpBuffer = (char *) malloc((payloadLength) * sizeof(char));
	udpBuffer[payloadLength-1] = '\0';
	payload.toCharArray(udpBuffer, payloadLength, 0);

	Serial.print("Sending JSON through UDP: ");
	Serial.println(udpBuffer);

	udpClient.beginPacket("translate.ubidots.com", 9012);
	udpClient.write(udpBuffer, payloadLength);
	udpClient.endPacket();

	free(udpBuffer);

	Serial.println("Sent UDP datagram...");

	return true;
}
#endif
