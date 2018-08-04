#ifndef __SYSTEM_CONTROLLER_H__
#define __SYSTEM_CONTROLLER_H__

#include <Arduino.h>
#include <Time.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include "UI.h"
#include "RFCommunication.h"
#include "GPSInterface.h"
#include "Communication.h"
#include "TimeEventsHandler.h"

#define CLEAR_BTN_PERIOD 10

class SystemController {
	public:
		SystemController();
		enum class SYSTEM_STATE {INIT, CONNECTING_TO_NETWORK, CONNECTED_TO_NETWORK, CONNECTING_TO_DATA_SERVER, CONNECTED_TO_DATA_SERVER, NORMAL_OPERATION};
		void setup();
		void loop();
	private:
		UI* ui;
		RFCommunication* rfComm;
		GPSInterface* gps;
		Communication* comm;
		TimeEventsHandler* teh;
		SYSTEM_STATE currentSystemState;
		uint32_t lastAttempt;
};

#endif

