#ifndef __GPSFUNCTIONS_H__
#define __GPSFUNCTIONS_H__

#include <Arduino.h>
#include <LGPS.h>
#include <LBattery.h>
#include <stdio.h>

class GPSInterface {
	public:
		static GPSInterface* getInstance();
		void init();
		String getData();
	private:
		GPSInterface();  /* private so that it can  not be called */
		GPSInterface(const GPSInterface&) = delete;
		GPSInterface& operator=(const GPSInterface&) = delete;
		static GPSInterface* pInstance;
		gpsSentenceInfoStruct gpsInfo;
		const char* nextToken(const char* src, char* buf);
		int arrayToInt(const char* char_array);
		float arrayToFloat(const char* char_array);
		void convertCoords(const char* latitude, const char* longitude, const char* lat_direction,
		                   const char* lon_direction, char* lat_return, char* lon_return, int buff_length);
};

#endif
