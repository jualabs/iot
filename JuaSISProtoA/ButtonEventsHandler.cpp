#include "ButtonEventsHandler.h"

AdjustableButtonConfig btnConfig;
AceButton redBtn;
AceButton whiteBtn;

ButtonEventsHandler* beh;

void buttonHandlerWrapper(AceButton* button, uint8_t eventType, uint8_t buttonState) {
	beh->mainBtnEventHandler(button, eventType, buttonState);
}

ButtonEventsHandler* ButtonEventsHandler::pInstance = nullptr;

ButtonEventsHandler* ButtonEventsHandler::getInstance() {
	if (!pInstance)
		pInstance = new ButtonEventsHandler();

	return pInstance;
}

ButtonEventsHandler::ButtonEventsHandler() :
														context(Context::getInstance()), actuators(Actuators::getInstance()),
														datalogger(Datalogger::getInstance()), timeEventsHandler(TimeEventsHandler::getInstance()),
														comm(Communication::getInstance()) {}

void ButtonEventsHandler::initButtons() {
	// configure buttons
	beh = this;
	btnConfig.setEventHandler(buttonHandlerWrapper);
	btnConfig.setFeature(ButtonConfig::kFeatureLongPress);
	btnConfig.setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);
	btnConfig.setLongPressDelay(5000);
	pinMode(RED_BTN, INPUT);
	redBtn.setButtonConfig(&btnConfig);
	redBtn.init(RED_BTN, HIGH, RED_BTN_ID);
	pinMode(WHITE_BTN, INPUT);
	whiteBtn.setButtonConfig(&btnConfig);
	whiteBtn.init(WHITE_BTN, HIGH, WHITE_BTN_ID);
}

void ButtonEventsHandler::checkButtonEvents() {
	redBtn.check();
	whiteBtn.check();
}

void ButtonEventsHandler::mainBtnEventHandler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
	int btnId = button->getId();

	switch(context->getCurrentState()) {
		case Context::State::STAND_BY:
			switch (eventType) {
				case AceButton::kEventReleased:
					// do nothing
					break;
				case AceButton::kEventLongPressed:
					switch(btnId) {
						case WHITE_BTN_ID:
							startExperimentBtnEventHandler();
							break;
						case RED_BTN_ID:
							enterGetDataStateBtnEventHandler();
							break;
					}
					break;
			}
			break;
		case Context::State::RUNNING:
			switch (eventType) {
				case AceButton::kEventReleased:
					switch(btnId) {
						case WHITE_BTN_ID:
							if(context->getIsManuallyIrrigating() == false)
								startManualIrrigationBtnEventHandler();
							else
								stopManualIrrigationBtnEventHandler();
							break;
						case RED_BTN_ID:
							suspendAutoIrrigationBtnEventHandler();
							break;
					}
					break;
				case AceButton::kEventLongPressed:
					switch(btnId) {
						case WHITE_BTN_ID:
							dumpExperimentStateBtnEventHandler();
							break;
						case RED_BTN_ID:
							stopExperimentBtnEventHandler();
							break;
					}
					break;
			}
			break;
		case Context::State::GET_DATA:
			switch (eventType) {
				case AceButton::kEventReleased:
					switch(btnId) {
						case WHITE_BTN_ID:
							enableFTPServerBtnEventHandler();
							break;
						case RED_BTN_ID:
							dumpFilesBtnEventHandler();
							break;
					}
					break;
				case AceButton::kEventLongPressed:
					switch(btnId) {
						case WHITE_BTN_ID:
							setTimeBtnEventHandler();
							break;
						case RED_BTN_ID:
							eraseFilesBtnEventHandler();
							break;
					}
					break;
			}
			break;
		case Context::State::FAILED:
			switch (eventType) {
				case AceButton::kEventReleased:
					switch(btnId) {
						case WHITE_BTN_ID:
							enableFTPServerBtnEventHandler();
							break;
						case RED_BTN_ID:
							dumpExperimentStateBtnEventHandler();
							dumpErrorLogBtnEventHandler();
							break;
					}
					break;
				case AceButton::kEventLongPressed:
					switch(btnId) {
						case WHITE_BTN_ID:
							setTimeBtnEventHandler();
							break;
						case RED_BTN_ID:
							eraseFilesBtnEventHandler();
							break;
					}
					break;
			}
			break;
	}
}

void ButtonEventsHandler::startExperimentBtnEventHandler() {
	// #ifdef DEBUG
	Serial.println("experiment started...");
	// #else
	// change the current system state
	context->changeState(Context::State::RUNNING);
	// start the time events
	timeEventsHandler->startTimeEvents();
	// #endif
}

void ButtonEventsHandler::enterGetDataStateBtnEventHandler() {
	// #ifdef DEBUG
	Serial.println("enterGetDataStateBtnEventHandler");
	// #else
	context->changeState(Context::State::GET_DATA);
	// #endif
}

void ButtonEventsHandler::startManualIrrigationBtnEventHandler() {
	// #ifdef DEBUG
	Serial.println("start manual irrigation...");
	// #else
	context->setIsManuallyIrrigating(true);
	context->setManIrrigationStartTime(now());
	/* turn manual pump on */
	actuators->setManPump(true);
	// #endif
}

void ButtonEventsHandler::stopManualIrrigationBtnEventHandler() {
	// #ifdef DEBUG
	Serial.println("stop manual irrigation...");
	// #else
	context->setIsManuallyIrrigating(false);
	actuators->setManPump(false);

	uint32_t startTime = context->getManIrrigationStartTime();
	uint32_t stopTime = now();

	char line[120];
	sprintf(line,"%d,%d,%d", startTime, stopTime, (stopTime - startTime));

	datalogger->appendLineInFile("/man-irrig.csv", line);
	// #endif
}

void ButtonEventsHandler::suspendAutoIrrigationBtnEventHandler() {
	// #ifdef DEBUG
	Serial.println("suspend automatic irrigation...");
	// #else
	context->setIsAutoIrrigationSuspended(true);
	// #endif
}

void ButtonEventsHandler::dumpExperimentStateBtnEventHandler() {
	// #ifdef DEBUG
	Serial.println("dumpExperimentStateBtnEventHandler");
	// #else
	Serial.println("********** current experiment state **********");
	Serial.print("state: ");
	Serial.println(context->getCurrentStateString());
	Serial.print("isManuallyIrrigating: ");
	Serial.println(context->getIsManuallyIrrigating());
	Serial.print("isAutoIrrigationSuspended: ");
	Serial.println(context->getIsAutoIrrigationSuspended());
	Serial.print("autoIrrigationDuration: ");
	Serial.println(context->getAutoIrrigationDuration());
	Serial.println("----------------------------------------------");
	Serial.print("currentMinute: ");
	Serial.println(context->getCurrentMinute());
	Serial.print("oneHourMaxTemp: ");
	Serial.println(context->getOneHourMaxTemp());
	Serial.print("oneHourMaxHum: ");
	Serial.println(context->getOneHourMaxHum());
	Serial.print("oneHourMinTemp: ");
	Serial.println(context->getOneHourMinTemp());
	Serial.print("oneHourMinHum: ");
	Serial.println(context->getOneHourMinHum());
	Serial.print("oneHourAvgTemp: ");
	Serial.println(context->getOneHourAvgTemp());
	Serial.print("oneHourAvgHum: ");
	Serial.println(context->getOneHourAvgHum());
	Serial.println("----------------------------------------------");
	Serial.print("currentHour: ");
	Serial.println(context->getCurrentHour());
	Serial.print("oneDayMaxTemp: ");
	Serial.println(context->getOneDayMaxTemp());
	Serial.print("oneDayMaxHum: ");
	Serial.println(context->getOneDayMaxHum());
	Serial.print("oneDayMinTemp: ");
	Serial.println(context->getOneDayMinTemp());
	Serial.print("oneDayMinHum: ");
	Serial.println(context->getOneDayMinHum());
	Serial.print("oneDayAvgTemp: ");
	Serial.println(context->getOneDayAvgTemp());
	Serial.print("oneDayAvgHum: ");
	Serial.println(context->getOneDayAvgHum());
	Serial.println("----------------------------------------------");
	Serial.print("currentDay: ");
	Serial.println(context->getCurrentDay());
	Serial.println("**********************************************");
	// #endif
}

void ButtonEventsHandler::stopExperimentBtnEventHandler() {
	// #ifdef DEBUG
	Serial.println("experiment stopped...");
	context->setIsManuallyIrrigating(false);
	actuators->setManPump(false);
	actuators->setAutoPump(false);
	// #else
	/* change the current system state */
	context->changeState(Context::State::GET_DATA);
	/* stop the time events */
	timeEventsHandler->stopTimeEvents();
	//#endif
}

void ButtonEventsHandler::dumpFilesBtnEventHandler() {
	//#ifdef DEBUG
	Serial.println("dumpFilesBtnEventHandler");
	//#else
	context->changeState(Context::State::STAND_BY);
	datalogger->dumpFiles();
	// #endif
}

void ButtonEventsHandler::eraseFilesBtnEventHandler() {
#ifdef DEBUG
	Serial.println("eraseFilesBtnEventHandler");
#endif
	context->changeState(Context::State::STAND_BY);
	datalogger->formatFS();
}

void ButtonEventsHandler::dumpErrorLogBtnEventHandler() {
#ifdef DEBUG
	Serial.println("dumpErrorLogBtnEventHandler");
#endif
}

void ButtonEventsHandler::setTimeBtnEventHandler() {
#ifdef DEBUG
	context->changeState(Context::State::SET_TIME);
	Serial.println("setTimeBtnEventHandler");
#endif
}

void ButtonEventsHandler::enableFTPServerBtnEventHandler() {
#ifdef DEBUG
	Serial.println("enableFTPBtnEventHandler");
#endif
	if(comm->isEnabled() == true) {
		context->changeState(Context::State::GET_DATA);
		comm->disable();
	}
	else {
		comm->enable();
	}
}
