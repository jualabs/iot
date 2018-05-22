#include "Actuators.h"

// global static pointer used to ensure a single instance of the class.
Actuators* Actuators::pInstance = nullptr;

/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this getInstance() function.
*/

Actuators* Actuators::getInstance() {
   if (!pInstance)   // Only allow one instance of class to be generated.
      pInstance = new Actuators();

   return pInstance;
}

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

