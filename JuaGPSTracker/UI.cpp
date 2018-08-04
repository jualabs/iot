#include "UI.h"


UI* ui;

/* global static pointer used to ensure a single instance of the class. */
UI* UI::pInstance = nullptr;

/*
 * 	This function is called to create an instance of the class.
 *  Calling the constructor publicly is not allowed. The constructor
 *  is private and is only called by this getInstance() function.
 */
UI* UI::getInstance() {
	if (!pInstance)   /* Only allow one instance of class to be generated. */
		pInstance = new UI();

	return pInstance;
}

UI::UI() : statusLED(JLed(STATUS_LED_PIN)) {
	currentUIState = UI_STATE::NETWORK_CONNECTION;
	/* set a invalid value for lastUIState to force state changing on first run */
	lastUIState = UI_STATE::IDLE;
}

void UI::loop() {
	if (lastUIState != currentUIState) {
		lastUIState = currentUIState;
		switch(currentUIState) {
			case UI_STATE::NETWORK_CONNECTION:
				statusLED.Blink(150, 150).Forever();
				break;
			case UI_STATE::DATA_SERVER_CONNECTION:
				statusLED.Blink(1000, 1000).Forever();
				break;
			case UI_STATE::GPS_NOT_FIXED:
				statusLED.Blink(3000, 3000).Forever();
				break;
			case UI_STATE::NORMAL_OPERATION:
				statusLED.Blink(150, 9850).Forever();
				break;
		}
	}
	statusLED.Update();
}

void UI::setUIState(UI_STATE state) {
	currentUIState = state;
}