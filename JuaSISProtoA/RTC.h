#ifndef __RTC_H__
#define __RTC_H__

#include <Arduino.h>
#include "DS1302RTC.h"

class RTC {
	public:
		static RTC* getInstance();
		uint32_t getTimeStamp();
		bool getTimeElements(tmElements_t* tm);
	private:
		RTC();  // private so that it can  not be called
	    RTC(const RTC&) = delete;
	    RTC& operator=(const RTC&) = delete;
		static RTC* pInstance;
		DS1302RTC rtc;
};

#endif
