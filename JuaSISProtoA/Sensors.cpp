#include "Sensors.h"

Sensors* Sensors::pInstance = nullptr;

Sensors* Sensors::getInstance() {
   if (!pInstance)
      pInstance = new Sensors();

   return pInstance;
}

Sensors::Sensors() : dht(DHT(D3, DHTTYPE)) {
}

float Sensors::getTemperature() {
  return dht.readTemperature();
}

float Sensors::getHumidity() {
  return dht.readHumidity();
}

