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

Communication::Communication() {
}

void Communication::initCommunication() {
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
}

void Communication::checkCommunication() {

}
