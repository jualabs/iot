#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#define BLYNK_PRINT Serial

#include <Arduino.h>
#include <ESP8266WiFi.h>
// #include <BlynkSimpleEsp8266.h>


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
		const char *pass = "B3rl1n3rM@u3r";
		const char *auth = "e312ac8c612b427084066af01f4ec6b4";
};

#endif
