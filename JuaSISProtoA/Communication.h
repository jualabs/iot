#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include <Arduino.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <uMQTTBroker.h>

// #define AP_MODE 1

class Communication {
	public:
		static Communication* getInstance();
		void init();
		void publish(char *topic, char *msg);
		void dataCallback(uint32_t *client, const char* topic, uint32_t topic_len, const char *data, uint32_t lenght);
	private:
		Communication();  // private so that it can  not be called
		Communication(const Communication&) = delete;
		Communication& operator=(const Communication&) = delete;
		static Communication* pInstance;
		const char *ssid = "SSID";
		const char *password = "PASSWORD";
		const uint16_t mqttPort = 1883;
		const uint16_t maxSubscriptions = 30;
		const uint16_t maxRetainedTopics = 30;
};

#endif
