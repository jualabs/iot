#ifndef __GPSFUNCTIONS_H__
#define __GPSFUNCTIONS_H__

#include <Arduino.h>
#include <LGPS.h>
#include <LBattery.h>
#include <stdio.h>

typedef struct GPSData_t {
	double latitude;
	double longitude;
	float altitude;
	float dop;
	float geoid;
	float knts_speed;
	float kmh_speed;
	float track_angle;
	uint8_t fix;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t sat_num;
	uint8_t day;
	uint8_t month;
	uint16_t year;
	String time_format;
	String date_format;
	char lat_format[12];
	char lon_format[12];
	char lastValidLatitude[12];
	char lastValidLongitude[12];
} GPSData;

class GPSInterface {
	public:
		static GPSInterface* getInstance();
		void init();
		String parseDataJSON();
		String parseDataSMS();
		bool isFixed();
		void powerOff();

	private:
		GPSInterface();  /* private so that it can  not be called */
		GPSInterface(const GPSInterface&) = delete;
		GPSInterface& operator=(const GPSInterface&) = delete;
		static GPSInterface* pInstance;
		gpsSentenceInfoStruct gpsInfo;
		GPSData gpsData;
		bool parseGPSData(GPSData* gpsData);
		float convert(String str, boolean dir);
		void getAndSendGPSData();
		const char *nextToken(const char* src, char* buf);
		float arrayToFloat(const char* char_array);
		int arrayToInt(const char* char_array);
		void convertCoords(const char* latitude, const char* longitude, const char* lat_direction,
		                   const char* lon_direction, char* lat_return, char* lon_return, int buff_length);

};

#endif
