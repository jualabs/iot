#ifndef __RTC_H__
#define __RTC_H__

#include <Arduino.h>
#include "DS1302RTC.h"

class RTC {
	public:
		RTC();
		uint32_t getTimeStamp();
		bool getTimeElements(tmElements_t* tm);
	private:
		DS1302RTC rtc;
};

#endif
