#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include <Arduino.h>
#include <PubSubClient.h>
#include <LGPRS.h>
#include <LGPRSClient.h>

#define UBIDOTS_SERVER "things.ubidots.com"
#define PATH "/api/v1.6/devices/calango-viajante?token=A1E-cl4w5Oltm5tBxLbsY9ucNn3xA1Dsyu"

/* APN of your cellular provider */
#define APN "claro.com.br"
#define APN_USER "claro"
#define APN_PASS "claro"

class Communication {
	public:
		static Communication* getInstance();
		void init();
		void loop();
		bool connectToNetwork();
		bool connectToDataServer();
		void disconnectFromDataServer();
		bool isConnectedToDataServer();
		int8_t getDataServerCommState();
		bool sendDataMQTT(String payload);
		bool sendDataHTML(String payload);

	private:
		Communication();  /* private so that it can  not be called */
		Communication(const Communication&) = delete;
		Communication& operator=(const Communication&) = delete;
		static Communication* pInstance;
		LGPRSClient gprsClient;
		PubSubClient mqttClient;
		const char* mqttServer = "things.ubidots.com";
		uint16_t mqttPort= 1883;
		const char* mqttClientId = "";
		const char* mqttUsername = "";
		const char* mqttPassword = "";
		const char* ubidotsToken = "A1E-cl4w5Oltm5tBxLbsY9ucNn3xA1Dsyu";
		const char* ubidotsTopic = "/v1.6/devices/calango-viajante";
};

#endif
