#include "SystemController.h"


SystemController::SystemController() {
	/* init led */
	stateLed = JLed(STATE_LED).Off();
	ctx = Context();
	dl = Datalogger();
	sensors = Sensors();
	actuators = Actuators();
	beh = ButtonEventsHandler();
	teh = TimeEventsHandler();
	rtc = RTC();
}

void SystemController::loop() {
	this->stateLedUpdate();
	this->beh.checkButtonEvents();
	this->teh.checkTimeEvents();
}

void SystemController::stateLedUpdate() {
	stateLed.Update();
}

void SystemController::changeState(Context::State toState) {
	/* change current state */
	ctx.setCurrentState(toState);
	// update led ctx
	switch(toState) {
		case Context::State::STAND_BY:
			stateLed.Breathe(3000).Forever();
			break;
		case Context::State::RUNNING:
			stateLed.On();
			break;
		case Context::State::GET_DATA:
			stateLed.Blink(500, 500).Forever();
			break;
		case Context::State::FAILED:
			stateLed.Blink(150, 150).Forever();
			break;
	}
}

const Context* SystemController::getContext() const {
	return &ctx;
}

Actuators* SystemController::getActuators() const {
	return &actuators;
}

Sensors* SystemController::getSensors() const {
	return &sensors;
}


RTC* SystemController::getRTC() const {
	return &rtc;
}

Datalogger* SystemController::getDatalogger() const {
	return &dl;
}

void SystemController::recoverContextFromFile() {

}
