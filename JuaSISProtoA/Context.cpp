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
	lastValidAutoIrrigationDuration = 0;
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

bool Context::recoverContext() {
	bool isContextRecovered = false;
	/* if there is a saved context ... */
	if(SPIFFS.exists("/context.txt")) {
#ifdef DEBUG
		Serial.println("recovering context...");
#endif
		/* open context file */
		File file = SPIFFS.open("/context.txt", "r");
		if (file) {
			/* get the current time stamp */
			time_t currentTime = now();
			/* get from file the last context checkpoint time stamp */
			time_t contextCheckpointTS = (uint32_t) file.parseInt();
#ifdef DEBUG
			Serial.print("last checkpoint ts: ");
			Serial.println(contextCheckpointTS);
#endif
			/* verify if the current time stamp is valid related to the stored context, that is, bigger than */
			if((currentTime - contextCheckpointTS) < 0) {
				LogError::getInstance()->insertError("ERROR: current time stamp lower than the last context checkpoint time stamp! Entered in set time mode!");
#ifdef DEBUG
				Serial.println("ERROR: current time stamp lower than the last context checkpoint time stamp! Entering set time mode!");
#endif
				file.close();
				/* enter fail state */
				changeState(Context::State::FAILED);
			}
			/* verify if it is recovering from an acceptable time period */
			else if((currentTime - contextCheckpointTS)/86400 > MAX_RECOVER_DAYS) {
				LogError::getInstance()->insertError("ERROR: Reached MAX_RECOVER_DAYS! Will not recover! Erasing current recover context!");
#ifdef DEBUG
				Serial.println("ERROR: Reached MAX_RECOVER_DAYS! Will not recover! Erasing current recover context!");
#endif
				file.close();
				/* enter on get data state, where the user can download or erase the data to restart the experiment */
				changeState(Context::State::GET_DATA);
			}
			else {
				currentState = (Context::State) file.parseInt();
			    changeState(currentState);
				/* verify if it is recovering from a more than one day period */
			    if((currentTime - contextCheckpointTS)/86400 > 0) {
			    	uint16_t checkpointDay = (uint16_t) file.parseInt();
			    	currentDay = (checkpointDay + (currentTime - contextCheckpointTS)/86400);
			    }
			    /* recovering on the same day */
				else {
					currentDay = (uint16_t) file.parseInt();
					currentHour = (uint8_t) file.parseInt();
					currentMinute = (uint8_t) file.parseInt();
					isAutoIrrigationSuspended = (bool) file.parseInt();
					autoIrrigationStartTime = (uint32_t) file.parseInt();
					autoIrrigationDuration = (uint16_t) file.parseInt();
					lastValidAutoIrrigationDuration = (uint16_t) file.parseInt();
					manIrrigationStartTime = (uint32_t) file.parseInt();
					oneDayMaxTemp = file.parseFloat();
					oneDayMaxHum = file.parseFloat();
					oneDayMinTemp = file.parseFloat();
					oneDayMinHum = file.parseFloat();
					oneDayAvgTemp = file.parseFloat();
					oneDayAvgHum = file.parseFloat();
				    /* recovering from the same day and hour */
				    if((currentTime - contextCheckpointTS)/3600 == 0) {
				    	oneHourMaxTemp = file.parseFloat();
						oneHourMaxHum = file.parseFloat();
						oneHourMinTemp = file.parseFloat();
						oneHourMinHum = file.parseFloat();
						oneHourAvgTemp = file.parseFloat();
						oneHourAvgHum = file.parseFloat();
				    }
				}
			    isContextRecovered = true;
#ifdef DEBUG
			    Serial.println("**** restored context ****");
			    printContextSerial();
			    Serial.println("**************************");
#endif
			}
			file.close();
		}
	    else {
	    	LogError::getInstance()->insertError("ERROR: opening context file!");
	    }
	}
	return isContextRecovered;
}

void Context::saveContext() {
#ifdef DEBUG
	Serial.println("saving context...");
#endif
	/* open context file */
	File file = SPIFFS.open("/context.txt", "w");
	if (file) {
		/* get current time stamp */
		time_t currentCheckpointTS = now();
		file.println(currentCheckpointTS);
		file.println((uint32_t) currentState);
		file.println(currentDay);
		file.println(currentHour);
		file.println(currentMinute);
		file.println(isAutoIrrigationSuspended);
		file.println(autoIrrigationStartTime);
		file.println(autoIrrigationDuration);
		file.println(lastValidAutoIrrigationDuration);
		file.println(manIrrigationStartTime);
		file.println(oneDayMaxTemp, 2);
		file.println(oneDayMaxHum, 2);
		file.println(oneDayMinTemp, 2);
		file.println(oneDayMinHum, 2);
		file.println(oneDayAvgTemp, 2);
		file.println(oneDayAvgHum, 2);
		file.println(oneHourMaxTemp, 2);
		file.println(oneHourMaxHum, 2);
		file.println(oneHourMinTemp, 2);
		file.println(oneHourMinHum, 2);
		file.println(oneHourAvgTemp, 2);
		file.println(oneHourAvgHum, 2);
		file.close();
	}
	else {
		LogError::getInstance()->insertError("ERROR: creating context file!");
	}
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

uint16_t Context::getLastValidAutoIrrigationDuration()  {
	return lastValidAutoIrrigationDuration;
}

void Context::setLastValidAutoIrrigationDuration(uint16_t duration) {
	lastValidAutoIrrigationDuration = duration;
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

char * Context::getCurrentStateString()  {
	static char returnStr[10];
	switch(currentState) {
		case State::STAND_BY:
			strcpy(returnStr,"STAND_BY");
			break;
		case State::RUNNING:
			strcpy(returnStr,"RUNNING");
			break;
		case State::GET_DATA:
			strcpy(returnStr,"GET_DATA");
			break;
		case State::FAILED:
			strcpy(returnStr,"FAILED");
			break;
		case State::SET_TIME:
			strcpy(returnStr,"SET_TIME");
			break;
		case State::STOPPED:
			strcpy(returnStr,"STOPPED");
			break;
	}
	return returnStr;
}

void Context::setCurrentState(State curState) {
	currentState = curState;
}

char * Context::getCurrentContextString() {
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

	static char str[100];
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
		case State::STOPPED:
			stateLed.Blink(2000, 2000).Forever();
			break;
	}
	stateLed.Update();
}

JLed* Context::getStateLed() {
	return &stateLed;
}

void Context::printContextSerial() {
	Serial.print("current state: ");
	Serial.println((uint32_t) getCurrentState());
	Serial.print("currentDay: ");
	Serial.println(getCurrentDay());
	Serial.print("currentHour: ");
	Serial.println(getCurrentHour());
	Serial.print("currentMinute: ");
	Serial.println(getCurrentMinute());
	Serial.print("isAutoIrrigationSuspended: ");
	Serial.println(getIsAutoIrrigationSuspended());
	Serial.print("autoIrrigationStartTime: ");
	Serial.println(getAutoIrrigationStartTime());
	Serial.print("autoIrrigationDuration: ");
	Serial.println(getAutoIrrigationDuration());
	Serial.print("lastValidAutoIrrigationDuration: ");
	Serial.println(getLastValidAutoIrrigationDuration());
	Serial.print("manIrrigationStartTime: ");
	Serial.println(getManIrrigationStartTime());
	Serial.print("oneDayMaxTemp: ");
	Serial.println(getOneDayMaxTemp(), 2);
	Serial.print("oneDayMaxHum: ");
	Serial.println(getOneDayMaxHum(), 2);
	Serial.print("oneDayMinTemp: ");
	Serial.println(getOneDayMinTemp(), 2);
	Serial.print("oneDayMinHum: ");
	Serial.println(getOneDayMinHum(), 2);
	Serial.print("oneDayAvgTemp: ");
	Serial.println(getOneDayAvgTemp(), 2);
	Serial.print("oneDayAvgHum: ");
	Serial.println(getOneDayAvgHum(), 2);
	Serial.print("oneHourMaxTemp: ");
	Serial.println(getOneHourMaxTemp(), 2);
	Serial.print("oneHourMaxHum: ");
	Serial.println(getOneHourMaxHum(), 2);
	Serial.print("oneHourMinTemp: ");
	Serial.println(getOneHourMinTemp(), 2);
	Serial.print("oneHourMinHum: ");
	Serial.println(getOneHourMinHum(), 2);
	Serial.print("oneHourAvgTemp: ");
	Serial.println(getOneHourAvgTemp(), 2);
	Serial.print("oneHourAvgHum: ");
	Serial.println(getOneHourAvgHum(), 2);
}
