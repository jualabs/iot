#include "Context.h"

// global static pointer used to ensure a single instance of the class.
Context* Context::pInstance = nullptr;

/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this getInstance() function.
*/

Context* Context::getInstance() {
   if (!pInstance)   // Only allow one instance of class to be generated.
      pInstance = new Context();

   return pInstance;
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
	/* if there is a saved context ... */
	if(SPIFFS.exists("/context.dat")) {
		/* loads it to the ContextRecover struct */
		File file = SPIFFS.open("/context.dat", "rb");
	    struct ContextRecover ctxRecover;
	    if (file) {
	    	uint8_t *buffer = (uint8_t*) malloc(sizeof(struct ContextRecover));
	    	file.read(buffer, sizeof(struct ContextRecover));
	    	buildContextRecover(buffer, &ctxRecover);
	    	free(buffer);
	    	file.close();
	    }
	    else {
	    	LogError::getInstance()->insertError("ERROR: opening context file!");
	    }
	    time_t currentTime = now();
	    /* verify if it is recovering from an acceptable period */
	    if((currentTime - ctxRecover.lastContextUpdateTS)/86400 > MAX_RECOVER_DAYS) {
	    	LogError::getInstance()->insertError("ERROR: Reached MAX_RECOVER_DAYS! Will not recover! Erasing current recover context!");
	    	SPIFFS.remove("/context.dat");
	    }
	    else {
    		currentState = State::RUNNING;
    		changeState(currentState);
	    	/* verify if it is recovering from a more than one day period */
	    	if((currentTime - ctxRecover.lastContextUpdateTS)/86400 > 0) {
	    		currentDay = (ctxRecover.currentDay + (currentTime - ctxRecover.lastContextUpdateTS)/86400);
	    	}
	    	else {
				isAutoIrrigationSuspended = ctxRecover.isAutoIrrigationSuspended;
				autoIrrigationStartTime = ctxRecover.autoIrrigationStartTime;
				autoIrrigationDuration = ctxRecover.autoIrrigationDuration;
				manIrrigationStartTime = ctxRecover.manIrrigationStartTime;
				currentDay = ctxRecover.currentDay;
				currentHour = ctxRecover.currentHour;
				currentMinute = ctxRecover.currentMinute;
				oneDayMaxTemp= ctxRecover.oneDayMaxTemp;
				oneDayMaxHum = ctxRecover.oneDayMaxHum;
				oneDayMinTemp = ctxRecover.oneDayMinTemp;
				oneDayMinHum = ctxRecover.oneDayMinHum;
				oneDayAvgTemp = ctxRecover.oneDayAvgTemp;
				oneDayAvgHum = ctxRecover.oneDayAvgHum;
	    		/* verify if it is recovering from the same hour period */
	    		if((currentTime - ctxRecover.lastContextUpdateTS)/3600 == 0) {
					oneHourMaxTemp = ctxRecover.oneDayMaxTemp;
					oneHourMaxHum = ctxRecover.oneDayMaxTemp;
					oneHourMinTemp = ctxRecover.oneDayMaxTemp;
					oneHourMinHum = ctxRecover.oneDayMaxTemp;
					oneHourAvgTemp = ctxRecover.oneDayMaxTemp;
					oneHourAvgHum = ctxRecover.oneDayMaxTemp;
	    		}
	    	}
	    }
	}
}

void Context::buildContextRecover(uint8_t *src, struct ContextRecover *ctxRecover) {
    uint16_t i = 0;
    memcpy(&ctxRecover->currentState, &src[i], sizeof(ctxRecover->currentState));
    i += sizeof(ctxRecover->currentState);
    memcpy(&ctxRecover->lastContextUpdateTS, &src[i], sizeof(ctxRecover->lastContextUpdateTS));
    i += sizeof(ctxRecover->lastContextUpdateTS);
    memcpy(&ctxRecover->isManuallyIrrigating, &src[i], sizeof(ctxRecover->isManuallyIrrigating));
    i += sizeof(ctxRecover->isManuallyIrrigating);
    memcpy(&ctxRecover->isAutoIrrigationSuspended, &src[i], sizeof(ctxRecover->isAutoIrrigationSuspended));
    i += sizeof(ctxRecover->isAutoIrrigationSuspended);
    memcpy(&ctxRecover->autoIrrigationStartTime, &src[i], sizeof(ctxRecover->autoIrrigationStartTime));
    i += sizeof(ctxRecover->autoIrrigationStartTime);
    memcpy(&ctxRecover->autoIrrigationDuration, &src[i], sizeof(ctxRecover->autoIrrigationDuration));
    i += sizeof(ctxRecover->autoIrrigationDuration);
    memcpy(&ctxRecover->manIrrigationStartTime, &src[i], sizeof(ctxRecover->manIrrigationStartTime));
    i += sizeof(ctxRecover->manIrrigationStartTime);
    memcpy(&ctxRecover->currentMinute, &src[i], sizeof(ctxRecover->currentMinute));
    i += sizeof(ctxRecover->currentMinute);
    memcpy(&ctxRecover->currentHour, &src[i], sizeof(ctxRecover->currentHour));
    i += sizeof(ctxRecover->currentHour);
    memcpy(&ctxRecover->currentDay, &src[i], sizeof(ctxRecover->currentDay));
    i += sizeof(ctxRecover->currentDay);
    memcpy(&ctxRecover->oneHourMaxTemp, &src[i], sizeof(ctxRecover->oneHourMaxTemp));
    i += sizeof(ctxRecover->oneHourMaxTemp);
    memcpy(&ctxRecover->oneHourMaxHum, &src[i], sizeof(ctxRecover->oneHourMaxHum));
    i += sizeof(ctxRecover->oneHourMaxHum);
    memcpy(&ctxRecover->oneHourMinTemp, &src[i], sizeof(ctxRecover->oneHourMinTemp));
    i += sizeof(ctxRecover->oneHourMinTemp);
    memcpy(&ctxRecover->oneHourMinHum, &src[i], sizeof(ctxRecover->oneHourMinHum));
    i += sizeof(ctxRecover->oneHourMinHum);
    memcpy(&ctxRecover->oneHourAvgTemp, &src[i], sizeof(ctxRecover->oneHourAvgTemp));
    i += sizeof(ctxRecover->oneHourAvgTemp);
    memcpy(&ctxRecover->oneHourAvgHum, &src[i], sizeof(ctxRecover->oneHourAvgHum));
    i += sizeof(ctxRecover->oneHourAvgHum);
    memcpy(&ctxRecover->oneDayMaxTemp, &src[i], sizeof(ctxRecover->oneDayMaxTemp));
    i += sizeof(ctxRecover->oneDayMaxTemp);
    memcpy(&ctxRecover->oneDayMaxHum, &src[i], sizeof(ctxRecover->oneDayMaxHum));
    i += sizeof(ctxRecover->oneDayMaxHum);
    memcpy(&ctxRecover->oneDayMinTemp, &src[i], sizeof(ctxRecover->oneDayMinTemp));
    i += sizeof(ctxRecover->oneDayMinTemp);
    memcpy(&ctxRecover->oneDayMinHum, &src[i], sizeof(ctxRecover->oneDayMinHum));
    i += sizeof(ctxRecover->oneDayMinHum);
    memcpy(&ctxRecover->oneDayAvgTemp, &src[i], sizeof(ctxRecover->oneDayAvgTemp));
    i += sizeof(ctxRecover->oneDayAvgTemp);
    memcpy(&ctxRecover->oneDayAvgHum, &src[i], sizeof(ctxRecover->oneDayAvgHum));
    i += sizeof(ctxRecover->oneDayAvgHum);
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

uint16_t Context::getAutoIrrigationDuration()  {
	return autoIrrigationDuration;
}

void Context::setAutoIrrigationDuration(uint16_t duration) {
	autoIrrigationDuration = duration;
}

uint32_t Context::getAutoIrrigationStartTime()  {
	return autoIrrigationStartTime;
}

void Context::setAutoIrrigationStartTime(
		uint32_t startTime) {
	autoIrrigationStartTime = startTime;
}

uint16_t Context::getCurrentDay()  {
	return currentDay;
}

void Context::setCurrentDay(uint16_t curDay) {
	currentDay = curDay;
}

uint8_t Context::getCurrentHour()  {
	return currentHour;
}

void Context::setCurrentHour(uint8_t curHour) {
	currentHour = curHour;
}

uint8_t Context::getCurrentMinute()  {
	return currentMinute;
}

void Context::setCurrentMinute(uint8_t curMinute) {
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

uint32_t Context::getManIrrigationStartTime()  {
	return manIrrigationStartTime;
}

void Context::setManIrrigationStartTime(uint32_t startTime) {
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
		case State::SET_TIME:
			sprintf(returnStr,"FAILED");
			break;
	}
	return returnStr;
}

void Context::setCurrentState(State curState) {
	currentState = curState;
}

char* Context::getCurrentContextString() {
	char minTmpStr[10];
	char maxTmpStr[10];
	char avgTmpStr[10];
	char minHumStr[10];
	char maxHumStr[10];
	char avgHumStr[10];

	dtostrf(oneHourMinTemp, 6, 2, minTmpStr);
	dtostrf(oneHourMaxTemp, 6, 2, maxTmpStr);
	dtostrf(oneHourAvgTemp, 6, 2, avgTmpStr);

	dtostrf(oneHourMinHum, 6, 2, minHumStr);
	dtostrf(oneHourMaxHum, 6, 2, maxHumStr);
	dtostrf(oneHourAvgHum, 6, 2, avgHumStr);

	char *str = (char *) malloc(sizeof(char) * 100);
	sprintf(str, "%d,%s,%s,%s,%s,%s,%s", now(), minTmpStr, maxTmpStr, avgTmpStr, minHumStr, maxHumStr, avgHumStr);
#ifdef DEBUG
	Serial.println(str);
#endif
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
		case State::SET_TIME:
			stateLed.Blink(150, 850).Forever();
			break;
	}
	stateLed.Update();
}

JLed* Context::getStateLed() {
	return &stateLed;
}

