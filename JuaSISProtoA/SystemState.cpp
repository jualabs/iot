#include "SystemState.h"


SystemState::SystemState() {
  /* init led */
  stateLed = JLed(STATE_LED).Off();
  Context ctx();
  currentState = SystemState::STAND_BY;
}

void SystemState::stateLedUpdate() {
  stateLed.Update();
}

void SystemState::changeState(SystemState::state toState) {
  // change current state
  currentState = toState;
  // update led state
  switch(toState) {
    case SystemState::STAND_BY:
      stateLed.Breathe(3000).Forever();
      break;
    case SystemState::RUNNING:
      stateLed.On();
      break;
    case SystemState::GET_DATA:
      stateLed.Blink(500, 500).Forever();
      break;
    case SystemState::FAILED:
      stateLed.Blink(150, 150).Forever();
      break;
  }
}

void SystemState::recoverContextFromFile() {

}

void SystemState::currentStateToString(char *str) {
  char minTmpStr[10];
  char maxTmpStr[10];
  char avgTmpStr[10];
  char minHumStr[10];
  char maxHumStr[10];
  char avgHumStr[10];
  
  dtostrf(ctx.getOneHourMinTemp(), 9, 2, minTmpStr);
  dtostrf(ctx.getOneHourMaxTemp(), 9, 2, maxTmpStr);
  dtostrf(ctx.getOneHourAvgTemp(), 9, 2, avgTmpStr);

  dtostrf(ctx.getOneHourMinHum(), 9, 2, minHumStr);
  dtostrf(ctx.getOneHourMaxHum(), 9, 2, maxHumStr);
  dtostrf(ctx.getOneHourAvgHum(), 9, 2, avgHumStr);

  // sprintf(str, "%ld,%s,%s,%s,%s,%s,%s", RTC.get(), minTmpStr, maxTmpStr, avgTmpStr, minHumStr, maxHumStr, avgHumStr);
}

