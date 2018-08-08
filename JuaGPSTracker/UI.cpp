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
	/* set a invalid value for currentUIState to force state changing on first run */
	currentUIState = UI::UI_STATE::INIT;
	lastUIState = UI::UI_STATE::INIT;
}

void UI::loop() {
	statusLED.Update();
}

void UI::setUIState(UI::UI_STATE state) {
	currentUIState = state;
	if (currentUIState != lastUIState) {
		lastUIState = currentUIState;
		switch(currentUIState) {
			case UI::UI_STATE::INIT:
			case UI::UI_STATE::CONNECTING_TO_NETWORK:
			case UI::UI_STATE::CONNECTED_TO_NETWORK:
				statusLED.Blink(150, 150).Forever();
				break;
			case UI::UI_STATE::CONNECTING_TO_DATA_SERVER:
			case UI::UI_STATE::CONNECTED_TO_DATA_SERVER:
				statusLED.Blink(1000, 1000).Forever();
				break;
			case UI::UI_STATE::GPS_FIXED:
				statusLED.Blink(150, 19850).Forever();
				break;
			case UI::UI_STATE::GPS_NOT_FIXED:
				statusLED.Blink(2000, 2000).Forever();
				break;
		}
	}
	statusLED.Update();
}
