#ifndef __BUTTON_EVENTS_HANDLER_H__
#define __BUTTON_EVENTS_HANDLER_H__

#include <Arduino.h>
#include <AceButton.h>
using namespace ace_button;
#include "Context.h"
#include "Actuators.h"
#include "Datalogger.h"
#include "RTC.h"
#include "TimeEventsHandler.h"

// buttons definitions
#define WHITE_BTN       D5
#define WHITE_BTN_ID    0
#define RED_BTN         D6
#define RED_BTN_ID      1

class ButtonEventsHandler {
	public:
		ButtonEventsHandler(Actuators act, Datalogger dl, RTC rtc, TimeEventsHandler teh);
		// main button handler
		void mainBtnEventHandler(AceButton* button, uint8_t eventType, uint8_t buttonState);
		void checkButtonEvents();
		void initButtons();
		static void btnHandler(AceButton* button, uint8_t eventType, uint8_t buttonState);
	private:
		Context* context;
		Actuators actuators;
		Datalogger datalogger;
		RTC rtc;
		TimeEventsHandler timeEventsHandler;
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

