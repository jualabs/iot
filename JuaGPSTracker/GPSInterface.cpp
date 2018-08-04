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

GPSInterface::GPSInterface() : gpsInfo(), rfComm(RFCommunication::getInstance()) {
}

const char* GPSInterface::nextToken(const char* src, char* buf) {
  int i = 0;
  while (src[i] != 0 && src[i] != ',')
    i++;
  if (buf)
  {
    strncpy(buf, src, i);
    buf[i] = 0;
  }
  if (src[i])
    i++;
  return src + i;
}

int GPSInterface::arrayToInt(const char* char_array) {
  int temp;
  sscanf(char_array, "%d", &temp);
  return temp;
}

float GPSInterface::arrayToFloat(const char* char_array) {
  float temp;
  sscanf(char_array, "%f", &temp);
  return temp;
}
void GPSInterface::convertCoords(const char* latitude, const char* longitude, const char* lat_direction,
                   const char* lon_direction, char* lat_return, char* lon_return, int buff_length) {
  char lat_deg[3];

  // extract the first 2 chars to get the latitudinal degrees
  strncpy(lat_deg, latitude, 2);

  // null terminate
  lat_deg[2] = 0;

  char lon_deg[4];

  // extract first 3 chars to get the longitudinal degrees
  strncpy(lon_deg, longitude, 3);

  // null terminate
  lon_deg[3] = 0;

  // convert to integer from char array
  int lat_deg_int = arrayToInt(lat_deg);
  int lon_deg_int = arrayToInt(lon_deg);

  // must now take remainder/60
  // this is to convert from degrees-mins-secs to decimal degrees
  // so the coordinates are "google mappable"

  // convert the entire degrees-mins-secs coordinates into a float - this is for easier manipulation later
  float latitude_float = arrayToFloat(latitude);
  float longitude_float = arrayToFloat(longitude);

  // remove the degrees part of the coordinates - so we are left with only minutes-seconds part of the coordinates
  latitude_float = latitude_float - (lat_deg_int * 100);
  longitude_float = longitude_float - (lon_deg_int * 100);

  // convert minutes-seconds to decimal
  latitude_float /= 60;
  longitude_float /= 60;

  // add back on the degrees part, so it is decimal degrees
  latitude_float += lat_deg_int;
  longitude_float += lon_deg_int;

  if (strcmp (lat_direction, "S") == 0) {
    latitude_float *= -1;
  }

  if (strcmp (lon_direction, "W") == 0) {
    longitude_float *= -1;
  }
  // format the coordinates nicey - no more than 6 decimal places
  // snprintf(lat_return, buff_length, "%2.6f", latitude_float);
  // snprintf(lon_return, buff_length, "%3.6f", longitude_float);
  sprintf(lat_return, "%2.6f\0", latitude_float);
  sprintf(lon_return, "%3.6f\0", longitude_float);
}

bool GPSInterface::isFixed() {
	LGPS.getData(&gpsInfo);
	const char* p = (char*)gpsInfo.GPGGA;
	char buffer[30];
	for(int i = 0; i < 7; i++) {
		p = nextToken(p, buffer);
	}
	return buffer[0] == '1' ? true : false;
}

String GPSInterface::getData() {
	String payload = "";
	Serial.println("Collecting GPS data.");
	LGPS.getData(&gpsInfo);
	Serial.println((char*)gpsInfo.GPGGA);

	char latitude[20];
	char lat_direction[1];
	char longitude[20];
	char lon_direction[1];
	char buf[20];
	char time[30];

	const char* p = (char*)gpsInfo.GPGGA;

	p = nextToken(p, 0); // GGA
	p = nextToken(p, time); // Time
	p = nextToken(p, latitude); // Latitude
	p = nextToken(p, lat_direction); // N or S?
	p = nextToken(p, longitude); // Longitude
	p = nextToken(p, lon_direction); // E or W?
	p = nextToken(p, buf); // fix quality

	const int coord_size = 12;
	char lat_fixed[coord_size], lon_fixed[coord_size];
	convertCoords(latitude, longitude, lat_direction, lon_direction, lat_fixed, lon_fixed, coord_size);
	/* Obtain battery level */
	String batteryLevel = String(LBattery.level());
	String batteryCharging = String(LBattery.isCharging());
	String num_sat = String(0);

	/* verify if GPS is fixed */
	if (buf[0] == '1')
	{
		/* GPS fix */
		Serial.println("GPS is fixed!");
		p = nextToken(p, buf); // number of satellites
		num_sat = String(buf);

		/* just debug messages */
		Serial.print("JuaTracker data: [");
		Serial.print("lat: "); Serial.print(lat_fixed); Serial.print(" lng: ");Serial.print(lon_fixed);
		Serial.print(" Battery level: "); Serial.print(batteryLevel);
		Serial.print(" Battery charging: "); Serial.print(batteryCharging);
		Serial.print(" Number of satellites: "); Serial.print(num_sat);
		Serial.print(" RF Button 0: "); Serial.print(rfComm->getRFEventState(0));
		Serial.print(" RF Button 1: "); Serial.println(rfComm->getRFEventState(1));

		/* prepare a JSON payload string for UBIDOTS */
		payload = "{";
		payload += "\"batl\":";
		payload += "{\"value\":"; payload += batteryLevel;  payload += ", ";
		payload += "\"context\":";
		payload += "{\"lat\":"; payload += lat_fixed; payload += ", ";
		payload += "\"lng\":"; payload += lon_fixed; payload += "}},";
		payload += "\"batc\":"; payload += batteryCharging; payload += ", ";
		payload += "\"nsat\":"; payload += num_sat; payload += ", ";
		payload += "\"btn0\":"; payload += rfComm->getRFEventState(0); payload += ", ";
		payload += "\"btn1\":"; payload += rfComm->getRFEventState(1);
		payload += "}";
	}
	else {
		Serial.println("GPS is NOT fixed yet!");
		Serial.print("JuaTracker data: [");
		Serial.print(" Battery level: "); Serial.print(batteryLevel);
		Serial.print(" Battery charging: "); Serial.print(batteryCharging);
		Serial.print(" Number of satellites: "); Serial.print(num_sat);
		Serial.print(" RF Button 0: "); Serial.print(rfComm->getRFEventState(0));
		Serial.print(" RF Button 1: "); Serial.println(rfComm->getRFEventState(1));

		/* prepare a JSON payload string for UBIDOTS */
		payload = "{";
		payload += "\"batl\":"; payload += batteryLevel;  payload += ", ";
		payload += "\"batc\":"; payload += batteryCharging; payload += ", ";
		payload += "\"nsat\":"; payload += num_sat; payload += ", ";
		payload += "\"btn0\":"; payload += rfComm->getRFEventState(0); payload += ", ";
		payload += "\"btn1\":"; payload += rfComm->getRFEventState(1);
		payload += "}";

	}
	return payload;
}

void GPSInterface::init() {
	LGPS.powerOn();
}