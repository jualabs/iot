#include "ButtonEventsHandler.h"

AdjustableButtonConfig btnConfig;
AceButton redBtn(RED_BTN);
AceButton whiteBtn(WHITE_BTN);

ButtonEventsHandler::ButtonEventsHandler(SystemController *sc) {
	this->sc = sc;
	// configure buttons
	btnConfig.setEventHandler(mainBtnEventHandler);
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

  switch(sc->getContext()->getCurrentState()) {
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
              if(this->sc->getContext()->getIsManuallyIrrigating() == false)
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
              dumpFilesBtnEventHandler();
              break;
            case RED_BTN_ID:
              // do nothing
              break;
          }
          break;
        case AceButton::kEventLongPressed:
          switch(btnId) {
            case WHITE_BTN_ID:
              // do nothing
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
              dumpExperimentStateBtnEventHandler();
              break;
            case RED_BTN_ID:
              dumpErrorLogBtnEventHandler();
              break;
          }
          break;
        case AceButton::kEventLongPressed:
          // do nothing
          break;
      }
      break;
  }
}

void ButtonEventsHandler::startExperimentBtnEventHandler() {
  this->sc->changeState(Context::State::RUNNING);
  // enable alarms
  // daily irrigation alarm
  Alarm.enable(startIrrigationEventAlarmId);
  // daily consolidation meteorological data alarm
  Alarm.enable(dailyEventAlarmId);
  unsigned int nextHour = (hour() + 1) % 24;
  // start once the every hour and minute event configuration function
  Alarm.alarmOnce(nextHour, 0, 1, cfgMinAndHourEvent);
#ifdef DEBUG
  Serial.print("experiment started...\n");
#endif
}

void ButtonEventsHandler::enterGetDataStateBtnEventHandler() {
#ifdef DEBUG
  Serial.println("enterGetDataStateBtnEventHandler");
#endif
}

void ButtonEventsHandler::startManualIrrigationBtnEventHandler() {
#ifdef DEBUG
  Serial.print("start manual irrigation...\n");
#endif

  sc->getContext()->setIsManuallyIrrigating(true);
  sc->getContext()->setManIrrigationStartTime(sc->getCurrentTimeStamp());
  /* turn manual pump on */
  sc->getActuators()->setManPump(true);
  sc->get
  experimentState.autoIrrigationStartTime = RTC.get();
}

void ButtonEventsHandler::stopManualIrrigationBtnEventHandler() {
#ifdef DEBUG
  Serial.print("stop manual irrigation...\n");
#endif
  experimentState.isManuallyIrrigating = false;
  digitalWrite(MAN_PUMP_RELAY, HIGH);
  unsigned long stopTime = RTC.get();
  File file;  
  // write csv file
  file = SPIFFS.open("/man-irrig.csv", "a");
  if(file) {
    char line[120];
    sprintf(line,"%ld,%ld,%ld", experimentState.manIrrigationStartTime, stopTime, (stopTime-experimentState.manIrrigationStartTime));
    file.println(line);
    file.close();
  }
else {
#ifdef DEBUG
  Serial.println("ERROR: opening '/man-irrig.csv'...");
#endif
  }
}

void ButtonEventsHandler::suspendAutoIrrigationBtnEventHandler() {
  experimentState.isAutoIrrigationSuspended = true;
#ifdef DEBUG
  Serial.print("suspend automatic irrigation...\n");
#endif
}

void ButtonEventsHandler::dumpExperimentStateBtnEventHandler() {
#ifdef DEBUG
  Serial.println("dumpExperimentStateBtnEventHandler");
#endif
  Serial.println("********** current experiment state **********");
  Serial.print("started: ");
  Serial.println(experimentState.started);
  Serial.print("isManuallyIrrigating: ");
  Serial.println(experimentState.isManuallyIrrigating);
  Serial.print("isAutoIrrigationSuspended: ");
  Serial.println(experimentState.isAutoIrrigationSuspended);
  Serial.print("autoIrrigationDuration: ");
  Serial.println(experimentState.autoIrrigationDuration);
  Serial.println("----------------------------------------------");
  Serial.print("currentMinute: ");
  Serial.println(experimentState.currentMinute);
  Serial.print("oneHourMaxTemp: ");
  Serial.println(experimentState.oneHourMaxTemp);
  Serial.print("oneHourMaxHum: ");
  Serial.println(experimentState.oneHourMaxHum);
  Serial.print("oneHourMinTemp: ");
  Serial.println(experimentState.oneHourMinTemp);
  Serial.print("oneHourMinHum: ");
  Serial.println(experimentState.oneHourMinHum);
  Serial.print("oneHourAvgTemp: ");
  Serial.println(experimentState.oneHourAvgTemp);
  Serial.print("oneHourAvgHum: ");
  Serial.println(experimentState.oneHourAvgHum);
  Serial.println("----------------------------------------------");
  Serial.print("currentHour: ");
  Serial.println(experimentState.currentHour);
  Serial.print("oneDayMaxTemp: ");
  Serial.println(experimentState.oneDayMaxTemp);
  Serial.print("oneDayMaxHum: ");
  Serial.println(experimentState.oneDayMaxHum);
  Serial.print("oneDayMinTemp: ");
  Serial.println(experimentState.oneDayMinTemp);
  Serial.print("oneDayMinHum: ");
  Serial.println(experimentState.oneDayMinHum);
  Serial.print("oneDayAvgTemp: ");
  Serial.println(experimentState.oneDayAvgTemp);
  Serial.print("oneDayAvgHum: ");
  Serial.println(experimentState.oneDayAvgHum);
  Serial.println("----------------------------------------------");
  Serial.print("currentDay: ");
  Serial.println(experimentState.currentDay);
  Serial.println("**********************************************");
}

void ButtonEventsHandler::stopExperimentBtnEventHandler() {
#ifdef DEBUG
  Serial.print("experiment stoped...\n");
#endif
  statusLed = JLed(STATUS_LED).Blink(500, 500).Forever();
  experimentState.started = false;
  Alarm.disable(minEventAlarmId);
  Alarm.disable(hourEventAlarmId);
  Alarm.disable(dailyEventAlarmId);
  Alarm.disable(startIrrigationEventAlarmId);
}

void ButtonEventsHandler::dumpFilesBtnEventHandler() {
#ifdef DEBUG
  Serial.println("dumpFilesBtnEventHandler");
#endif
  File file;
  // dump csv files
  for(int i = 0; i < NUM_CSV_FILES; i++) {
    file = SPIFFS.open(csv_files[i], "r");
    if(file) {
      Serial.print("********** ");
      Serial.print(csv_files[i]);
      Serial.println(" **********");
      while(file.available())
        Serial.println(file.readStringUntil('\n'));
      file.close();
    }
    else {
#ifdef DEBUG
      Serial.print("ERROR: opening file -> ");
      Serial.println(csv_files[i]);
#endif
    }
  }
  // dump json files
  for(int i = 0; i < NUM_JSON_FILES; i++) {
    file = SPIFFS.open(json_files[i], "r");
    if(file) {
      Serial.print("********** ");
      Serial.print(json_files[i]);
      Serial.println(" **********");
      while(file.available())
        Serial.println(file.readStringUntil('\n'));
      file.close();
    }
    else {
#ifdef DEBUG
      Serial.print("ERROR: opening file -> ");
      Serial.println(json_files[i]);
#endif
    }
  }
  // dump error log
  file = SPIFFS.open("/log_error.txt", "w");
  if(file) {
    Serial.println("********** /log_error.txt **********");
    while(file.available())
      Serial.println(file.readStringUntil('\n'));
    file.close();
  }
  else {
#ifdef DEBUG
    Serial.print("ERROR: opening file -> ");
    Serial.println("/log_error.txt");
#endif
  }
}

void ButtonEventsHandler::eraseFilesBtnEventHandler() {
#ifdef DEBUG
  Serial.println("eraseFilesBtnEventHandler");
#endif  
}

void ButtonEventsHandler::dumpErrorLogBtnEventHandler() {
#ifdef DEBUG
  Serial.println("dumpErrorLogBtnEventHandler");
#endif   
}



