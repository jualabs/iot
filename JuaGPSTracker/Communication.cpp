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

bool Communication::isConnectedToDataServer() {
	return mqttClient.connected();
}

int8_t Communication::getDataServerCommState() {
	return (int8_t) mqttClient.state();
}


/* UBIDOTS MQTT SEND DATA */
void Communication::sendData(String payload) {
	char payloadChar[200];
	payload.toCharArray(payloadChar, 200);
	mqttClient.publish(ubidotsTopic, payloadChar);
	Serial.print("Published (");
	Serial.print(strlen(payloadChar));
	Serial.print(" bytes): ");
	Serial.println(payloadChar);
}

#if 0
/* UBIDOTS HTTP SEND DATA */
void Communication::sendData(String payload) {
	char payloadChar[200];
	char payloadLength[4];
	payload.toCharArray(payloadChar, 200);
	Serial.println(payloadChar);
	/* get length of the entire payload */
	sprintf(payloadLength,"%d", strlen(payloadChar));

	Serial.println("Sending: ");
	Serial.println(payload);

	/* connect and send data to Ubidots... */

	if (gprsClient.connect(UBIDOTS_SERVER, 80)){
		Serial.println("Connected!");  // Console monitoring
		gprsClient.print(F("POST "));
		gprsClient.print(PATH);
		gprsClient.print(F(" HTTP/1.1\r\n"));
		gprsClient.print(F("Content-Type: application/json\r\n"));
		// gprsClientnt.print(F("Connection: close\r\n"));
		gprsClient.print(F("Host: "));
		gprsClient.print(PATH);
		gprsClient.print(F("\r\n"));
		gprsClient.print(F("Content-Length: "));
		gprsClient.print(payloadLength);
		gprsClient.print(F("\r\n"));
		gprsClient.print(F("\r\n"));
		gprsClient.print(payloadChar);
		gprsClient.println();

	}
	else {
		Serial.println("Connection failed");
	}

	Serial.println("passou aqui!");

	gprsClient.stop();
}
#endif
