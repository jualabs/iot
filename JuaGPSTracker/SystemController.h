#ifndef __SYSTEM_CONTROLLER_H__
#define __SYSTEM_CONTROLLER_H__

#include <Arduino.h>
#include <Time.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include "UI.h"
#include "GPSInterface.h"
#include "Communication.h"
#include "TimeEventsHandler.h"

#define MAX_DATA_SERVER_CONNECTION_RETRIES 5
#define MAX_GPRS_CONNECTION_RETRIES 10

class SystemController {
	public:
		SystemController();
		void setup();
		void loop();
		enum class SYSTEM_STATE {INIT, CONNECTING_TO_NETWORK, CONNECTED_TO_NETWORK, CONNECTING_TO_DATA_SERVER, CONNECTED_TO_DATA_SERVER, GPS_NOT_FIXED, GPS_FIXED};
	private:
		UI* ui;
		GPSInterface* gps;
		Communication* comm;
		TimeEventsHandler* teh;
		SYSTEM_STATE currentSystemState;
		uint32_t lastAttempt;
		uint8_t numDataServerRetries;
		uint8_t numGPRSRetries;
};

#endif

