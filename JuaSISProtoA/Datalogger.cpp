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

