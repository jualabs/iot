#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <Arduino.h>
// sensors libraries
#include "DHT.h"

// sensor definitions
#define DHTPIN D3         // what digital pin the DHT22 is connected to
#define DHTTYPE DHT22     // there are multiple kinds of DHT sensors

class Sensors {
	public:
		Sensors();
		float getTemperature();
		float getHumidity();
	private:
		DHT dht;
};

#endif
