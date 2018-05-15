#ifndef __BUTTON_EVENTS_HANDLER_H__
#define __BUTTON_EVENTS_HANDLER_H__

#include <Arduino.h>
#include <AceButton.h>
using namespace ace_button;
#include "SystemController.h"

// buttons definitions
#define WHITE_BTN       D5
#define WHITE_BTN_ID    0
#define RED_BTN         D6
#define RED_BTN_ID      1

class ButtonEventsHandler {
	public:
		ButtonEventsHandler(SystemController* sc);
		void checkButtonEvents();
	private:
		SystemController* sc;
		// main handler
		void mainBtnEventHandler(AceButton* button, uint8_t eventType, uint8_t buttonState);
		// derivated handlers
		void startExperimentBtnEventHandler();
		void enterGetDataStateBtnEventHandler();
		void startManualIrrigationBtnEventHandler();
		void stopManualIrrigationBtnEventHandler();
		void suspendAutoIrrigationBtnEventHandler();
		void dumpExperimentStateBtnEventHandler();
		void stopExperimentBtnEventHandler();
		void dumpFilesBtnEventHandler();
		void eraseFilesBtnEventHandler();
		void dumpErrorLogBtnEventHandler();
};

#endif

