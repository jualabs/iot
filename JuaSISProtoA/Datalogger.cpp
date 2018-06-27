#include "Datalogger.h"

// global static pointer used to ensure a single instance of the class.
Datalogger* Datalogger::pInstance = nullptr;

/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this getInstance() function.
*/

Datalogger* Datalogger::getInstance() {
   if (!pInstance)   // Only allow one instance of class to be generated.
      pInstance = new Datalogger();

   return pInstance;
}

Datalogger::Datalogger() {
  // mount file system  
  SPIFFS.begin();
  loadFiles();
}

void Datalogger::formatFS() {
	SPIFFS.format();
}

void Datalogger::loadFiles() {
	File file;
	/* init csv files */
	for(int i = 0; i < NUM_CSV_FILES; i++) {
		if(!SPIFFS.exists(csv_files[i])) {
			file = SPIFFS.open(csv_files[i], "w");
			/* write csv headers */
			if(file) {
				file.println(csv_files_headers[i]);
				file.close();
			}
			else {
				/* TODO: decide how to notify opening file errors. */
				Serial.print("ERROR: Creating file ");
				Serial.println(csv_files[i]);
			}
		}
    }
}

void Datalogger::appendLineInFile(const char* filename, const char* line) {
	/* append text line to a file */
	File file;
	/* write file */
	file = SPIFFS.open(filename, "a");
	if(file) {
		file.println(line);
		file.close();
	}
	else {
		/* TODO: decide how to notify opening file errors. */
		Serial.print("ERROR: Opening file ");
		Serial.println(filename);
	}
}

void Datalogger::dumpFiles() {
	File file;
	/* dump csv files */
	for(int i = 0; i < NUM_CSV_FILES; i++) {
		file = SPIFFS.open(csv_files[i], "r");
		if(file) {
			Serial.print("********** ");
			Serial.print(csv_files[i]);
			Serial.println(" **********");
			while(file.available())
				Serial.println(file.readStringUntil('\n'));
			file.close();
		}
		else {
			/* TODO: decide how to notify opening file errors. */
			Serial.print("ERROR: Opening file ");
			Serial.println(csv_files[i]);
	    }
	}
	/* dump error log */
	file = SPIFFS.open("/log_error.txt", "w");
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
