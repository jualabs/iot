#include "RTC.h"

// global static pointer used to ensure a single instance of the class.
RTC* RTC::pInstance = nullptr;

/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this getInstance() function.
*/

RTC* RTC::getInstance() {
   if (!pInstance)   // Only allow one instance of class to be generated.
      pInstance = new RTC();

   return pInstance;
}

RTC::RTC() : rtc(DS1302RTC(D0, D2, D1)) {
}

uint32_t RTC::getTimeStamp() {
	return rtc.get();
}

bool RTC::getTimeElements(tmElements_t* tm) {
	if(rtc.read(*tm) == true) {
		return true;
	}
	else {
		return false;
	}
}
