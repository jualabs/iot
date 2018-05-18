#include "Datalogger.h"

Datalogger::Datalogger() {
  // configure serial
  Serial.begin(9600);
  // mount file system  
  SPIFFS.begin();
  loadFiles();
}

void Datalogger::logError(char* errorStr) {
  
}

void Datalogger::loadFiles() {
  File file;
  // init csv files
  for(int i = 0; i < NUM_CSV_FILES; i++) {
    if(!SPIFFS.exists(csv_files[i])) {
       file = SPIFFS.open(csv_files[i], "w");
       // write csv headers
       if(file) {
        file.println(csv_files_headers[i]);
        file.close();
       }
       else {
#ifdef DEBUG
         Serial.print("ERROR: creating file -> ");
         Serial.println(csv_files[i]);
#endif
       }
    }
  }
  // init json files
  for(int i = 0; i < NUM_JSON_FILES; i++) {
    if(!SPIFFS.exists(json_files[i])) {
       file = SPIFFS.open(json_files[i], "w");
       if(file) file.close();
       else {
#ifdef DEBUG
         Serial.print("ERROR: creating file -> ");
         Serial.println(json_files[i]);
#endif
       }
    }
  }
  // init error log
  if(!SPIFFS.exists("/log_error.txt")) {
       file = SPIFFS.open("/log_error.txt", "w");
       if(file) file.close();
       else {
#ifdef DEBUG
         Serial.println("ERROR: creating file -> /log_error.txt");
#endif
       }
    }

}

void Datalogger::appendLineInFile(char* filename, char* line) {
	// append text line to a file
	File file;
	// write csv file
	file = SPIFFS.open(filename, "a");
	if(file) {
		file.println(line);
		file.close();
	}
	else {
#ifdef DEBUG
		Serial.println("ERROR: opening '%s'...", filename);
#endif
	}
}

void Datalogger::dumpFiles() {
	File file;
	// dump csv files
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
#ifdef DEBUG
			Serial.print("ERROR: opening file -> ");
			Serial.println(csv_files[i]);
#endif
	    }
	}
	// dump json files
	for(int i = 0; i < NUM_JSON_FILES; i++) {
		file = SPIFFS.open(json_files[i], "r");
		if(file) {
			Serial.print("********** ");
			Serial.print(json_files[i]);
			Serial.println(" **********");
			while(file.available())
				Serial.println(file.readStringUntil('\n'));
			file.close();
	    }
	    else {
#ifdef DEBUG
	    	Serial.print("ERROR: opening file -> ");
	    	Serial.println(json_files[i]);
#endif
	    }
	}
	// dump error log
	file = SPIFFS.open("/log_error.txt", "w");
	if(file) {
		Serial.println("********** /log_error.txt **********");
	    while(file.available())
	    	Serial.println(file.readStringUntil('\n'));
	    file.close();
	}
	else {
#ifdef DEBUG
		Serial.print("ERROR: opening file -> ");
	    Serial.println("/log_error.txt");
#endif
	}
}
