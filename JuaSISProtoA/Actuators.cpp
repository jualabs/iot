#include "Actuators.h"

Actuators::Actuators() {
  /* config relays */
  pinMode(MAN_PUMP_RELAY, OUTPUT);
  pinMode(AUT_PUMP_RELAY, OUTPUT);
  digitalWrite(MAN_PUMP_RELAY, HIGH);
  digitalWrite(AUT_PUMP_RELAY, HIGH);
}

void Actuators::setManPump(bool state) {
  if(state == true) {
    digitalWrite(MAN_PUMP_RELAY, LOW);
  }
  else {
	  digitalWrite(MAN_PUMP_RELAY, HIGH);
  }
}

void Actuators::setAutoPump(bool state) {
  if(state == true) {
    digitalWrite(AUT_PUMP_RELAY, LOW);
  }
  else {
    digitalWrite(AUT_PUMP_RELAY, HIGH);
  }
}

