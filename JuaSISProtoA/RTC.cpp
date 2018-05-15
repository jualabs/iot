#include "RTC.h"

RTC::RTC() {
	rtc = DS1302RTC(D0, D2, D1);
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
