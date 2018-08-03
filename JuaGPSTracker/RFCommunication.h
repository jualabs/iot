#ifndef __RFCOMMUNICATION_H__
#define __RFCOMMUNICATION_H__

#include <Arduino.h>
#include <RCSwitch.h>

class RFCommunication {
	public:
		static RFCommunication* getInstance();
		void init();
		void loop();
		enum class RF_EVENTS {BTN_0, BTN_1, NUM_EVENTS};
		bool getRFEvent(RF_EVENTS evt);
		void setRFEvent(RF_EVENTS evt);
		void clearRFEvent(RF_EVENTS evt);

	private:
		RFCommunication();  /* private so that it can  not be called */
		RFCommunication(const RFCommunication&) = delete;
		RFCommunication& operator=(const RFCommunication&) = delete;
		static RFCommunication* pInstance;
		RCSwitch rfController;
		const uint32_t code[(uint8_t) RF_EVENTS::NUM_EVENTS] = {146289317, 146289301};
		uint32_t lastPressTS[(uint8_t) RF_EVENTS::NUM_EVENTS];
		bool rfEvents[(uint8_t) RF_EVENTS::NUM_EVENTS];
};

#endif
