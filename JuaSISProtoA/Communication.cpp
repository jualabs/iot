#include "Communication.h"

Communication* comm;

void dataCallbackWrapper(uint32_t *client, const char* topic, uint32_t topic_len, const char *data, uint32_t lenght) {
	comm->dataCallback(client, topic, topic_len, data, lenght);
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

Communication::Communication() {
}

void Communication::dataCallback(uint32_t *client, const char* topic, uint32_t topic_len, const char *data, uint32_t lenght) {
  char topic_str[topic_len + 1];
  os_memcpy(topic_str, topic, topic_len);
  topic_str[topic_len] = '\0';

  char data_str[lenght + 1];
  os_memcpy(data_str, data, lenght);
  data_str[lenght] = '\0';

  Serial.print("received topic '");
  Serial.print(topic_str);
  Serial.print("' with data '");
  Serial.print(data_str);
  Serial.println("'");
}

void Communication::init() {
#ifdef AP_MODE
	Serial.print("\n\nconfiguring AP...");
	WiFi.softAP(ssid, password);
	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
#else
	Serial.print("\n\nconnecting to \"");
	Serial.print(ssid);
	Serial.println("\"...");
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print("#");
	}
	Serial.println(" connected!");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
#endif
	/* register the callback */
	MQTT_server_onData(dataCallbackWrapper);
	/* start the broker */
	Serial.println("Starting MQTT broker");
	MQTT_server_start(mqttPort, maxSubscriptions, maxRetainedTopics);
	MQTT_local_subscribe((unsigned char *)"#", 0);
}

void Communication::publish(char *topic, char *msg) {
	MQTT_local_publish((unsigned char*) topic, (unsigned char*) msg, strlen(msg), 0, 0);
}
