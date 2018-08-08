#ifndef __UI_H__
#define __UI_H__

#include <Arduino.h>
#include <jled.h>
//#include "SystemController.h"

#define STATUS_LED_PIN 11

class UI {
	public:
		static UI* getInstance();
		enum class UI_STATE {INIT, CONNECTING_TO_NETWORK, CONNECTED_TO_NETWORK, CONNECTING_TO_DATA_SERVER, CONNECTED_TO_DATA_SERVER, GPS_NOT_FIXED, GPS_FIXED};
		void loop();
		void setUIState(UI::UI_STATE state);

	private:
		UI();  /* private so that it can  not be called */
		UI(const UI&) = delete;
		UI& operator=(const UI&) = delete;
		static UI* pInstance;
		JLed statusLED;
		UI_STATE currentUIState;
		UI_STATE lastUIState;
};

#endif
