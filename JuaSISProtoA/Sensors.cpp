#include "Sensors.h"

Sensors::Sensors() {
	dht = DHT(D3, DHT22);
}

float Sensors::getTemperature() {
  return dht.readTemperature();
}

float Sensors::getHumidity() {
  return dht.readHumidity();
}

