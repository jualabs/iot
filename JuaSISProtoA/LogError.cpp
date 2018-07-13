#include "LogError.h"

// global static pointer used to ensure a single instance of the class.
LogError* LogError::pInstance = nullptr;

/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this getInstance() function.
*/

LogError* LogError::getInstance() {
   if (!pInstance)   // only allow one instance of class to be generated.
      pInstance = new LogError();

   return pInstance;
}

LogError::LogError() {
	/* mount file system */
	SPIFFS.begin();
	/* init error log */
	if(!SPIFFS.exists("/log_error.txt")) {
		File file = SPIFFS.open("/log_error.txt", "w");
		if(file) {
			file.close();
		}
	    else {
			/* TODO: decide how to notify opening file errors. */
			Serial.print("ERROR: Creating file ");
	    	Serial.println("/log_error.txt");
	    }
	}
}

void LogError::insertError(char* errorMsg) {
	File file;
	// dump error log
	file = SPIFFS.open("/log_error.txt", "a");
	if(file) {
		file.print(now());
		file.print(",");
		file.println(errorMsg);
		file.close();
	}
	else {
		/* TODO: decide how to notify opening file errors. */
		Serial.print("ERROR: Opening file ");
		Serial.println("/log_error.txt");
	}
}

void LogError::dumpErrorLog() {
	File file;
	// dump error log
	file = SPIFFS.open("/log_error.txt", "r");
	if(file) {
		Serial.println("********** /log_error.txt **********");
	    while(file.available())
	    	Serial.println(file.readStringUntil('\n'));
	    file.close();
	}
	else {
		/* TODO: decide how to notify opening file errors. */
		Serial.print("ERROR: Opening file ");
		Serial.println("/log_error.txt");
	}
}
