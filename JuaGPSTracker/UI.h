#ifndef __UI_H__
#define __UI_H__

#include <Arduino.h>
#include <jled.h>

#define STATUS_LED_PIN 13

class UI {
	public:
		static UI* getInstance();
		enum class UI_STATE {IDLE, CONNECTING_TO_GPS, NETWORK_CONNECTION, DATA_SERVER_CONNECTION, NORMAL_OPERATION};
		void loop();
		void setUIState(UI_STATE state);

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
