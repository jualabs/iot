#include "Context.h"

// global static pointer used to ensure a single instance of the class.
Context* Context::m_pInstance = nullptr;

/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this getInstance() function.
*/

Context* Context::getInstance() {
   if (!m_pInstance)   // Only allow one instance of class to be generated.
      m_pInstance = new Context();

   return m_pInstance;
}

Context::Context() : stateLed(JLed(STATE_LED_PIN)) {
	stateLed.Off();
	currentState = State::STAND_BY;
	changeState(currentState);
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

void Context::initContext() {
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

unsigned int Context::getAutoIrrigationDuration()  {
	return autoIrrigationDuration;
}

void Context::setAutoIrrigationDuration(unsigned int duration) {
	autoIrrigationDuration = duration;
}

unsigned long Context::getAutoIrrigationStartTime()  {
	return autoIrrigationStartTime;
}

void Context::setAutoIrrigationStartTime(
		unsigned long startTime) {
	autoIrrigationStartTime = startTime;
}

unsigned int Context::getCurrentDay()  {
	return currentDay;
}

void Context::setCurrentDay(unsigned int curDay) {
	currentDay = curDay;
}

unsigned int Context::getCurrentHour()  {
	return currentHour;
}

void Context::setCurrentHour(unsigned int curHour) {
	currentHour = curHour;
}

unsigned int Context::getCurrentMinute()  {
	return currentMinute;
}

void Context::setCurrentMinute(unsigned int curMinute) {
	currentMinute = curMinute;
}

bool Context::getIsAutoIrrigationSuspended()  {
	return isAutoIrrigationSuspended;
}

void Context::setIsAutoIrrigationSuspended(bool suspendedFlag) {
	isAutoIrrigationSuspended = suspendedFlag;
}

bool Context::getIsManuallyIrrigating()  {
	return isManuallyIrrigating;
}

void Context::setIsManuallyIrrigating(bool manuallyIrrigatingFlag) {
	isManuallyIrrigating = manuallyIrrigatingFlag;
}

unsigned long Context::getManIrrigationStartTime()  {
	return manIrrigationStartTime;
}

void Context::setManIrrigationStartTime(unsigned long startTime) {
	manIrrigationStartTime = startTime;
}

float Context::getOneDayAvgHum()  {
	return oneDayAvgHum;
}

void Context::setOneDayAvgHum(float avgHum) {
	oneDayAvgHum = avgHum;
}

float Context::getOneDayAvgTemp()  {
	return oneDayAvgTemp;
}

void Context::setOneDayAvgTemp(float avgTemp) {
	oneDayAvgTemp = avgTemp;
}

float Context::getOneDayMaxHum()  {
	return oneDayMaxHum;
}

void Context::setOneDayMaxHum(float maxHum) {
	oneDayMaxHum = maxHum;
}

float Context::getOneDayMaxTemp()  {
	return oneDayMaxTemp;
}

void Context::setOneDayMaxTemp(float maxTemp) {
	oneDayMaxTemp = maxTemp;
}

float Context::getOneDayMinHum()  {
	return oneDayMinHum;
}

void Context::setOneDayMinHum(float minHum) {
	oneDayMinHum = minHum;
}

float Context::getOneDayMinTemp()  {
	return oneDayMinTemp;
}

void Context::setOneDayMinTemp(float minTemp) {
	oneDayMinTemp = minTemp;
}

float Context::getOneHourAvgHum()  {
	return oneHourAvgHum;
}

void Context::setOneHourAvgHum(float avgHum) {
	oneHourAvgHum = avgHum;
}

float Context::getOneHourAvgTemp()  {
	return oneHourAvgTemp;
}

void Context::setOneHourAvgTemp(float avgTemp) {
	oneHourAvgTemp = avgTemp;
}

float Context::getOneHourMaxHum()  {
	return oneHourMaxHum;
}

void Context::setOneHourMaxHum(float maxHum) {
	oneHourMaxHum = maxHum;
}

float Context::getOneHourMaxTemp()  {
	return oneHourMaxTemp;
}

void Context::setOneHourMaxTemp(float maxTemp) {
	oneHourMaxTemp = maxTemp;
}

float Context::getOneHourMinHum()  {
	return oneHourMinHum;
}

void Context::setOneHourMinHum(float minHum) {
	oneHourMinHum = minHum;
}

float Context::getOneHourMinTemp()  {
	return oneHourMinTemp;
}

void Context::setOneHourMinTemp(float minTemp) {
	oneHourMinTemp = minTemp;
}

Context::State Context::getCurrentState()  {
	return currentState;
}

char* Context::getCurrentStateString()  {
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

void Context::setCurrentState(State curState) {
	currentState = curState;
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
	stateLed.Update();
}

JLed* Context::getStateLed() {
	return &stateLed;
}

