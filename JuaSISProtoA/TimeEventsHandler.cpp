#include "TimeEventsHandler.h"

DS1302RTC rtc(D0, D2, D1);

AlarmId minEventAlarmId;
AlarmId hourEventAlarmId;
AlarmId dailyEventAlarmId;
AlarmId startIrrigationEventAlarmId;

void initTimeEventsHandler() {
  tmElements_t tm;
  // check RTC 
  if (rtc.haltRTC()) {
    logError("ERROR: rtc read");
  }
  // set alarm timer through rtc
  if (!RTC.read(tm))
    setTime(tm.Hour,tm.Minute,tm.Second,tm.Day,tm.Month,tmYearToCalendar(tm.Year));  
  else
    logError("ERROR: rtc read");
}

void initAlarms() {
  startIrrigationEventAlarmId = Alarm.alarmRepeat(0,2,0, startAutoIrrigationTimeEventHandler);
  Alarm.disable(startIrrigationEventAlarmId);
  dailyEventAlarmId = Alarm.alarmRepeat(0,1,0, dailyTimeEventHandler);
  Alarm.disable(dailyEventAlarmId);
  minEventAlarmId = Alarm.timerRepeat(60, minTimeEventHandler);
  Alarm.disable(minEventAlarmId);
  hourEventAlarmId = Alarm.timerRepeat(60*60, hourTimeEventHandler);
  Alarm.disable(hourEventAlarmId);
}

void checkTimeEvents() {
  Alarm.delay(1);
}

void cfgMinAndHourTimerEventHandler() {
#ifdef DEBUG
  Serial.println("cfgMinAndHourTimerEventHandler");
#endif
  Alarm.enable(minEventAlarmId);
  Alarm.enable(hourEventAlarmId);
}

void minTimeEventHandler() {
#ifdef DEBUG
  Serial.println("minTimeEventHandler");
#endif
  // read sensor data
  float currentTmp = dht.readTemperature();
  float currentHum = dht.readHumidity();
  // refresh Max and Min values of temperature and humidity
  if(currentTmp > experimentState.oneHourMaxTemp) experimentState.oneHourMaxTemp = currentTmp;
  if(currentTmp < experimentState.oneHourMinTemp) experimentState.oneHourMinTemp = currentTmp;
  if(currentHum > experimentState.oneHourMaxHum) experimentState.oneHourMaxHum = currentHum;
  if(currentHum < experimentState.oneHourMinHum) experimentState.oneHourMinHum = currentHum;
  // refresh temperature and humidity averages
  if(experimentState.currentMinute > 0) {
    experimentState.oneHourAvgTemp = (experimentState.oneHourAvgTemp*((float) experimentState.currentMinute/(experimentState.currentMinute+1))) + currentTmp/(experimentState.currentMinute+1);
    experimentState.oneHourAvgHum = (experimentState.oneHourAvgHum*((float) experimentState.currentMinute/(experimentState.currentMinute+1))) + currentHum/(experimentState.currentMinute+1);
  }
  else {
    experimentState.oneHourAvgTemp = currentTmp;
    experimentState.oneHourAvgHum = currentHum;
  }
  experimentState.currentMinute++;
}

void hourTimeEventHandler() {

#ifdef DEBUG
  Serial.println("hourTimeEventHandler");
#endif
  // write current hour values to file
  File file;  
  // write csv file
  file = SPIFFS.open("/hour.csv", "a");
  if(file) {
    char line[120];
    strCurrentState(line);
    file.println(line);
    file.close();
  }
else {
#ifdef DEBUG
  Serial.println("ERROR: opening 'hour.csv'...");
#endif
  }
  // refresh Max and Min values of temperature and humidity
  if(experimentState.oneHourMaxTemp > experimentState.oneDayMaxTemp) experimentState.oneDayMaxTemp = experimentState.oneHourMaxTemp;
  if(experimentState.oneHourMinTemp < experimentState.oneDayMinTemp) experimentState.oneDayMinTemp = experimentState.oneHourMinTemp;
  if(experimentState.oneHourMaxHum > experimentState.oneDayMaxHum) experimentState.oneDayMaxHum = experimentState.oneHourMaxHum;
  if(experimentState.oneHourMinHum < experimentState.oneDayMinHum) experimentState.oneDayMinHum = experimentState.oneHourMinHum;
  // refresh temperature and humidity averages
  if(experimentState.currentHour > 0) {
    experimentState.oneDayAvgTemp = (experimentState.oneDayAvgTemp*((float) experimentState.currentHour/(experimentState.currentHour+1))) + experimentState.oneHourAvgTemp/(experimentState.currentHour+1);
    experimentState.oneDayAvgHum = (experimentState.oneDayAvgHum*((float) experimentState.currentHour/(experimentState.currentHour+1))) + experimentState.oneHourAvgHum/(experimentState.currentHour+1);
  }
  else {
    experimentState.oneDayAvgTemp = experimentState.oneHourAvgTemp;
    experimentState.oneDayAvgHum = experimentState.oneHourAvgHum;
  }
  clearHourValues();
  experimentState.currentHour++;
}

void dailyTimeEventHandler() {
#ifdef DEBUG
  Serial.println("dailyTimeEventHandler");
#endif
  experimentState.isAutoIrrigationSuspended = false;
  float es = 0.6108 * exp((17.27*experimentState.oneDayAvgTemp)/(experimentState.oneDayAvgTemp + 237.3));
  float ea = (es * experimentState.oneDayAvgHum) / 100;
  float eto = (2.5982 * pow((1 + (experimentState.oneDayAvgTemp/25)), 2) * (1 - (ea/es))) + 0.7972;
  float etc = eto * kc[experimentState.currentDay];
  float ll = etc * 0.1 * sqrt(35);
  float lb = ll / experimentParameters.efc;
  // calculate irrigation time in seconds (hour -> seconds)
  experimentState.autoIrrigationDuration = (unsigned int) ((lb / experimentParameters.ia) * 3600);

  // write current hour values to file
  File file;  
  // write csv file
  file = SPIFFS.open("/day.csv", "a");
  if(file) {
    char line[120];
    char kcStr[10];
    char etoStr[10];
    dtostrf(kc[experimentState.currentDay], 9, 3, kcStr);
    dtostrf(eto, 9, 3, etoStr);
    sprintf(line,"%ld,%s,%s,%d", RTC.get(), kcStr, etoStr, experimentState.autoIrrigationDuration);
    file.println(line);
    file.close();
  }
else {
#ifdef DEBUG
  Serial.println("ERROR: opening 'day.csv'...");
#endif
  }

  experimentState.currentDay++;
  clearDayValues();
}

void startAutoIrrigationTimeEventHandler() {
#ifdef DEBUG
  Serial.println("startAutoIrrigationTimeEventHandler");
#endif
  if(!experimentState.isAutoIrrigationSuspended) {
    digitalWrite(AUT_PUMP_RELAY, LOW);
    Alarm.timerOnce(experimentState.autoIrrigationDuration, stopAutoIrrigationTimeEventHandler);
    experimentState.autoIrrigationStartTime = RTC.get();
  }
}

void stopAutoIrrigationTimeEventHandler() {
#ifdef DEBUG
  Serial.println("stopAutoIrrigationTimeEventHandler");
#endif
  digitalWrite(AUT_PUMP_RELAY, HIGH);
  unsigned long stopTime = RTC.get();
  // write current hour values to file
  File file;  
  // write csv file
  file = SPIFFS.open("/aut-irrig.csv", "a");
  if(file) {
    char line[120];
    sprintf(line,"%ld,%ld,%ld", experimentState.autoIrrigationStartTime, stopTime, (stopTime-experimentState.autoIrrigationStartTime));
    file.println(line);
    file.close();
  }
else {
#ifdef DEBUG
  Serial.println("ERROR: opening '/aut-irrig.csv'...");
#endif
  } 
}
