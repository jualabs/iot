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

}

void Communication::init() {
	mqttClient.setServer(mqttServer, mqttPort);
}

void Communication::loop() {
	mqttClient.loop();
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


/* UBIDOTS MQTT SEND DATA */
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
		Serial.println(gprsClient.readStringUntil('\n'));
		gprsClient.stop();
		return true;
	}
	else {
		Serial.println(F("Connection failed"));
		gprsClient.stop();
		return false;
	}
}
