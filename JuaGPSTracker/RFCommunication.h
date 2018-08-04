#ifndef __RFCOMMUNICATION_H__
#define __RFCOMMUNICATION_H__

#include <Arduino.h>
#include <RCSwitch.h>

#define NUM_RF_EVENTS 2

typedef struct RFCommData_t {
	uint32_t code;
	bool eventFlag;
} RFCommData;

class RFCommunication {
	public:
		static RFCommunication* getInstance();
		void init();
		void loop();
		uint8_t getRFEventState(uint8_t eventId);
		void clearRFEventState(uint8_t eventId);
	private:
		RFCommunication();  /* private so that it can  not be called */
		RFCommunication(const RFCommunication&) = delete;
		RFCommunication& operator=(const RFCommunication&) = delete;
		static RFCommunication* pInstance;
		RCSwitch rfController;
		RFCommData rfCommData[NUM_RF_EVENTS];
};

#endif
