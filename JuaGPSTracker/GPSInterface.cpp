#include "GPSInterface.h"

GPSInterface* gpsInterface;

/* global static pointer used to ensure a single instance of the class. */
GPSInterface* GPSInterface::pInstance = nullptr;

/*
 * 	This function is called to create an instance of the class.
 *  Calling the constructor publicly is not allowed. The constructor
 *  is private and is only called by this getInstance() function.
 */
GPSInterface* GPSInterface::getInstance() {
	if (!pInstance)   /* Only allow one instance of class to be generated. */
		pInstance = new GPSInterface();

	return pInstance;
}

GPSInterface::GPSInterface() : gpsInfo() {
	gpsData.latitude = 0.00;
	gpsData.longitude = 0.00;
	gpsData.altitude = 0.00;
	gpsData.dop = 100.00;
	gpsData.geoid = 0.00;
	gpsData.knts_speed = 0.00;
	gpsData.kmh_speed = 0.00;
	gpsData.track_angle = 0.00;
	gpsData.fix = 0;
	gpsData.hour = 0;
	gpsData.minute = 0;
	gpsData.second = 0;
	gpsData.sat_num = 0;
	gpsData.day = 0;
	gpsData.month = 0;
	gpsData.year = 0;
	String time_format = "00:00:00", date_format = "00:00:0000";
	String lat_format = "0.00000", lon_format = "0.00000";
	String lastValidLatitude = "0.00000", lastValidLongitude = "0.00000";
}

bool GPSInterface::isFixed() {
	return gpsData.fix;
}

void GPSInterface::init() {
	LGPS.powerOn();
}

void GPSInterface::powerOff() {
	LGPS.powerOff();
}


String GPSInterface::parseDataJSON() {
	String payload = "";

	String batteryLevel = String(LBattery.level());
	String batteryCharging = String(LBattery.isCharging());

	/* if GPS is fixed, create JSON with all data */
	if(parseGPSData(&gpsData) == true) {
		Serial.println("GPS is fixed!");
		payload = "{";
		payload += "\"batc\":";
		payload += "{\"value\":"; payload += batteryCharging; payload += ", ";
		payload += "\"context\":";
		payload += "{\"lat\":"; payload += gpsData.lat_format; payload += ", ";
		payload += "\"lng\":"; payload += gpsData.lon_format; payload += "}},";
		payload += "\"alt\":"; payload += gpsData.altitude; payload += ", ";
		payload += "\"spdk\":"; payload += gpsData.kmh_speed; payload += ", ";
		payload += "\"tang\":"; payload += gpsData.track_angle; payload += ", ";
		payload += "\"nsat\":"; payload += gpsData.sat_num; payload += ", ";
		payload += "\"batl\":"; payload += batteryLevel;
		payload += "}";
	}
	/* if not fixed, creates JSON with only available data */
	else {
		Serial.println("GPS is NOT fixed yet!");
		payload = "{";
		payload += "\"nsat\":"; payload += gpsData.sat_num; payload += ", ";
		payload += "\"batl\":"; payload += batteryLevel; payload += ", ";
		payload += "\"batc\":"; payload += batteryCharging;
		payload += "}";
	}
	return payload;
}

String GPSInterface::parseDataSMS() {
	String payload = "http://maps.google.com/?q=";
	payload += gpsData.lastValidLatitude;
	payload +=",";
	payload += gpsData.lastValidLongitude;
	return payload;
}

float GPSInterface::convert(String str, boolean dir) {
  double mm, dd;
  int point = str.indexOf('.');
  dd = str.substring(0, (point - 2)).toFloat();
  mm = str.substring(point - 2).toFloat() / 60.00;
  return (dir ? -1 : 1) * (dd + mm);
}

bool GPSInterface::parseGPSData(GPSData* gpsData) {
	Serial.println("Collecting GPS data.");
	LGPS.getData(&gpsInfo);
	// Serial.println((char*)gpsInfo.GPGGA);

	if (gpsInfo.GPGGA[0] == '$') {
		// Serial.print("Parsing GGA data....");
		String str = (char*)(gpsInfo.GPGGA);
		str = str.substring(str.indexOf(',') + 1);
		gpsData->hour = str.substring(0, 2).toInt();
		gpsData->minute = str.substring(2, 4).toInt();
		gpsData->second = str.substring(4, 6).toInt();
		gpsData->time_format = "";
		gpsData->time_format += gpsData->hour; gpsData->time_format += ":";
		gpsData->time_format += gpsData->minute; gpsData->time_format += ":";
		gpsData->time_format += gpsData->second;
		str = str.substring(str.indexOf(',') + 1);
		gpsData->latitude = convert(str.substring(0, str.indexOf(',')), str.charAt(str.indexOf(',') + 1) == 'S');
		sprintf(gpsData->lat_format, "%2.6f", gpsData->latitude);
		str = str.substring(str.indexOf(',') + 3);
		gpsData->longitude = convert(str.substring(0, str.indexOf(',')), str.charAt(str.indexOf(',') + 1) == 'W');
		sprintf(gpsData->lon_format, "%3.6f", gpsData->longitude);
		str = str.substring(str.indexOf(',') + 3);
		gpsData->fix = str.charAt(0) - 48;
		str = str.substring(2);
		gpsData->sat_num = str.substring(0, 2).toInt();
		str = str.substring(3);
		gpsData->dop = str.substring(0, str.indexOf(',')).toFloat();
		str = str.substring(str.indexOf(',') + 1);
		gpsData->altitude = str.substring(0, str.indexOf(',')).toFloat();
		str = str.substring(str.indexOf(',') + 3);
		gpsData->geoid = str.substring(0, str.indexOf(',')).toFloat();

		if (gpsInfo.GPRMC[0] == '$') {
			// Serial.print("Parsing RMC data....");
			str = (char*)(gpsInfo.GPRMC);
			int comma = 0;
			for (int i = 0; i < 60; ++i) {
				if (gpsInfo.GPRMC[i] == ',') {
					comma++;
					if (comma == 7) {
						comma = i + 1;
						break;
					}
				}
			}
			str = str.substring(comma);
			gpsData->knts_speed = str.substring(0, str.indexOf(',')).toFloat();
			gpsData->kmh_speed = gpsData->knts_speed * 1.852;
			str = str.substring(str.indexOf(',') + 1);
			gpsData->track_angle = str.substring(0, str.indexOf(',')).toFloat();
			str = str.substring(str.indexOf(',') + 1);
			gpsData->day = str.substring(0, 2).toInt();
			gpsData->month = str.substring(2, 4).toInt();
			gpsData->year = str.substring(4, 6).toInt();
			gpsData->date_format = "20"; gpsData->date_format += gpsData->year; gpsData->date_format += "-";
			gpsData->date_format += gpsData->month; gpsData->date_format += "-";
			gpsData->date_format += gpsData->day;

			if(gpsData->fix == true) {
				strcpy(gpsData->lastValidLatitude, gpsData->lat_format);
				strcpy(gpsData->lastValidLongitude, gpsData->lon_format);
			}

			return gpsData->fix;
		}
	}
	else {
		Serial.println("No GGA data");
	}

	return false;
}

