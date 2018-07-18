#ifndef __BUTTON_EVENTS_HANDLER_H__
#define __BUTTON_EVENTS_HANDLER_H__

#include <Arduino.h>
#include <AceButton.h>
using namespace ace_button;
#include <Time.h>
#include <TimeLib.h>
#include "Context.h"
#include "Actuators.h"
#include "Datalogger.h"
#include "TimeEventsHandler.h"
#include "Communication.h"

// buttons definitions
#define WHITE_BTN       D5
#define WHITE_BTN_ID    0
#define RED_BTN         D6
#define RED_BTN_ID      1

class ButtonEventsHandler {
	public:
		static ButtonEventsHandler* getInstance();
		/* main function for button handling */
		void mainBtnEventHandler(AceButton* button, uint8_t eventType, uint8_t buttonState);
		void checkButtonEvents();
		void initButtons();

	private:
		ButtonEventsHandler();  // private so that it can  not be called
		ButtonEventsHandler(const ButtonEventsHandler&) = delete;
		ButtonEventsHandler& operator=(const ButtonEventsHandler&) = delete;
		static ButtonEventsHandler* pInstance;
		Context* context;
		Actuators* actuators;
		Datalogger* datalogger;
		TimeEventsHandler* timeEventsHandler;
		Communication* comm;
		/* derivated handlers */
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
		void setTimeBtnEventHandler();
		void enableFTPServerBtnEventHandler();
};

#endif

