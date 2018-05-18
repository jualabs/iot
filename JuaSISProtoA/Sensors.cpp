#include "Sensors.h"

Sensors::Sensors() : dht(DHT(D3, DHTTYPE)) {
}

float Sensors::getTemperature() {
  return dht.readTemperature();
}

float Sensors::getHumidity() {
  return dht.readHumidity();
}

