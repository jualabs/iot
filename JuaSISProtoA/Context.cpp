#include "Context.h"


Context::Context() : stateLed(JLed(STATE_LED_PIN)) {
	/* init led */
	stateLed.Off();
	currentState = State::STAND_BY;
	isManuallyIrrigating = false;
	isAutoIrrigationSuspended = false;
	autoIrrigationStartTime = 0;
	autoIrrigationDuration = 0;
	manIrrigationStartTime = 0;
	currentMinute = 0;
	currentHour = 0;
	currentDay = 0;
	oneHourMaxTemp = -50.0;
	oneHourMaxHum = 0.0;
	oneHourMinTemp = 50.0;
	oneHourMinHum = 100.0;
	oneHourAvgTemp = 0.0;
	oneHourAvgHum = 0.0;
	oneDayMaxTemp= -50.0;
	oneDayMaxHum = 0.0;
	oneDayMinTemp = 50.0;
	oneDayMinHum = 100.0;
	oneDayAvgTemp = 0.0;
	oneDayAvgHum = 0.0;
}

void Context::resetHourContext() {
	currentMinute = 0;
	oneHourMaxTemp = -50.0;
	oneHourMaxHum = 0.0;
	oneHourMinTemp = 50.0;
	oneHourMinHum = 100.0;
	oneHourAvgTemp = 0.0;
	oneHourAvgHum = 0.0;
}

void Context::resetDayContext() {
	currentHour = 0;
	oneDayMaxTemp = -50.0;
	oneDayMaxHum = 0.0;
	oneDayMinTemp = 50.0;
	oneDayMinHum = 100.0;
	oneDayAvgTemp = 0.0;
	oneDayAvgHum = 0.0;
}

unsigned int Context::getAutoIrrigationDuration() const {
	return autoIrrigationDuration;
}

void Context::setAutoIrrigationDuration(unsigned int autoIrrigationDuration) {
	this->autoIrrigationDuration = autoIrrigationDuration;
}

unsigned long Context::getAutoIrrigationStartTime() const {
	return autoIrrigationStartTime;
}

void Context::setAutoIrrigationStartTime(
		unsigned long autoIrrigationStartTime) {
	this->autoIrrigationStartTime = autoIrrigationStartTime;
}

unsigned int Context::getCurrentDay() const {
	return currentDay;
}

void Context::setCurrentDay(unsigned int currentDay) {
	this->currentDay = currentDay;
}

unsigned int Context::getCurrentHour() const {
	return currentHour;
}

void Context::setCurrentHour(unsigned int currentHour) {
	this->currentHour = currentHour;
}

unsigned int Context::getCurrentMinute() const {
	return currentMinute;
}

void Context::setCurrentMinute(unsigned int currentMinute) {
	this->currentMinute = currentMinute;
}

bool Context::getIsAutoIrrigationSuspended() const {
	return isAutoIrrigationSuspended;
}

void Context::setIsAutoIrrigationSuspended(bool isAutoIrrigationSuspended) {
	this->isAutoIrrigationSuspended = isAutoIrrigationSuspended;
}

bool Context::getIsManuallyIrrigating() const {
	return isManuallyIrrigating;
}

void Context::setIsManuallyIrrigating(bool isManuallyIrrigating) {
	this->isManuallyIrrigating = isManuallyIrrigating;
}

unsigned long Context::getManIrrigationStartTime() const {
	return manIrrigationStartTime;
}

void Context::setManIrrigationStartTime(unsigned long manIrrigationStartTime) {
	this->manIrrigationStartTime = manIrrigationStartTime;
}

float Context::getOneDayAvgHum() const {
	return oneDayAvgHum;
}

void Context::setOneDayAvgHum(float oneDayAvgHum) {
	this->oneDayAvgHum = oneDayAvgHum;
}

float Context::getOneDayAvgTemp() const {
	return oneDayAvgTemp;
}

void Context::setOneDayAvgTemp(float oneDayAvgTemp) {
	this->oneDayAvgTemp = oneDayAvgTemp;
}

float Context::getOneDayMaxHum() const {
	return oneDayMaxHum;
}

void Context::setOneDayMaxHum(float oneDayMaxHum) {
	this->oneDayMaxHum = oneDayMaxHum;
}

float Context::getOneDayMaxTemp() const {
	return oneDayMaxTemp;
}

void Context::setOneDayMaxTemp(float oneDayMaxTemp) {
	this->oneDayMaxTemp = oneDayMaxTemp;
}

float Context::getOneDayMinHum() const {
	return oneDayMinHum;
}

void Context::setOneDayMinHum(float oneDayMinHum) {
	this->oneDayMinHum = oneDayMinHum;
}

float Context::getOneDayMinTemp() const {
	return oneDayMinTemp;
}

void Context::setOneDayMinTemp(float oneDayMinTemp) {
	this->oneDayMinTemp = oneDayMinTemp;
}

float Context::getOneHourAvgHum() const {
	return oneHourAvgHum;
}

void Context::setOneHourAvgHum(float oneHourAvgHum) {
	this->oneHourAvgHum = oneHourAvgHum;
}

float Context::getOneHourAvgTemp() const {
	return oneHourAvgTemp;
}

void Context::setOneHourAvgTemp(float oneHourAvgTemp) {
	this->oneHourAvgTemp = oneHourAvgTemp;
}

float Context::getOneHourMaxHum() const {
	return oneHourMaxHum;
}

void Context::setOneHourMaxHum(float oneHourMaxHum) {
	this->oneHourMaxHum = oneHourMaxHum;
}

float Context::getOneHourMaxTemp() const {
	return oneHourMaxTemp;
}

void Context::setOneHourMaxTemp(float oneHourMaxTemp) {
	this->oneHourMaxTemp = oneHourMaxTemp;
}

float Context::getOneHourMinHum() const {
	return oneHourMinHum;
}

void Context::setOneHourMinHum(float oneHourMinHum) {
	this->oneHourMinHum = oneHourMinHum;
}

float Context::getOneHourMinTemp() const {
	return oneHourMinTemp;
}

void Context::setOneHourMinTemp(float oneHourMinTemp) {
	this->oneHourMinTemp = oneHourMinTemp;
}

Context::State Context::getCurrentState() const {
	return currentState;
}

char* Context::getCurrentStateString() const {
	char * returnStr = (char *) malloc(sizeof(char) * 10);
	switch(currentState) {
		case State::STAND_BY:
			sprintf(returnStr,"STAND_BY");
			break;
		case State::RUNNING:
			sprintf(returnStr,"RUNNING");
			break;
		case State::GET_DATA:
			sprintf(returnStr,"GET_DATA");
			break;
		case State::FAILED:
			sprintf(returnStr,"FAILED");
			break;
	}
	return returnStr;
}

void Context::setCurrentState(State currentState) {
	this->currentState = currentState;
}

char* Context::getCurrentContextString(uint32_t ts) {
	char minTmpStr[10];
	char maxTmpStr[10];
	char avgTmpStr[10];
	char minHumStr[10];
	char maxHumStr[10];
	char avgHumStr[10];

	dtostrf(oneHourMinTemp, 9, 2, minTmpStr);
	dtostrf(oneHourMaxTemp, 9, 2, maxTmpStr);
	dtostrf(oneHourAvgTemp, 9, 2, avgTmpStr);

	dtostrf(oneHourMinHum, 9, 2, minHumStr);
	dtostrf(oneHourMaxHum, 9, 2, maxHumStr);
	dtostrf(oneHourAvgHum, 9, 2, avgHumStr);

	char *str = (char *) malloc(sizeof(char) * 100);
	sprintf(str, "%l,%s,%s,%s,%s,%s,%s", ts, minTmpStr, maxTmpStr, avgTmpStr, minHumStr, maxHumStr, avgHumStr);

	return str;
}

void Context::changeState(State toState) {
	/* change current state */
	setCurrentState(toState);
	// update led ctx

	switch(toState) {
		case State::STAND_BY:
			stateLed.Breathe(3000).Forever();
			break;
		case State::RUNNING:
			stateLed.On();
			break;
		case State::GET_DATA:
			stateLed.Blink(500, 500).Forever();
			break;
		case State::FAILED:
			stateLed.Blink(150, 150).Forever();
			break;
	}
}

JLed* Context::getStateLed() {
	return &stateLed;
}
