#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include <Arduino.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>

#define AP_MODE 1
#define MAX_SRV_CLIENTS 1


class Communication {
	public:
		static Communication* getInstance();
		void initCommunication();
		void checkCommunication();

	private:
		Communication();  // private so that it can  not be called
		Communication(const Communication&) = delete;
		Communication& operator=(const Communication&) = delete;
		static Communication* pInstance;
		const char *ssid = "siri_cascudo";
		const char *password = "B3rl1n3rM@u3r";
		// WiFiServer server(23);
		// WiFiClient serverClients[MAX_SRV_CLIENTS];
};

#endif
