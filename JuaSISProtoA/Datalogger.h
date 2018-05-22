#ifndef __DATALOGGER_H__
#define __DATALOGGER_H__

#include <Arduino.h>
#include <FS.h>
#include <string.h>

static const unsigned int NUM_JSON_FILES = 4;
static const unsigned int NUM_CSV_FILES = 4;
static constexpr char csv_files[NUM_CSV_FILES][20]= {"/hour.csv","/day.csv","/aut-irrig.csv","/man-irrig.csv"};
static constexpr char json_files[NUM_JSON_FILES][20]= {"/hour.json","/day.json","/aut-irrig.json","/man-irrig.json"};
static constexpr char csv_files_headers[NUM_CSV_FILES][80] = {"ts,tmp_min,tmp_max,tmp_avg,hum_min,hum_max,hum_avg",
    	    																  "ts,kc,eto,irrig_duration",
    																		  "ts_init,ts_stop,duration",
    																		  "ts_init,ts_stop,duration"};

class Datalogger {
	public:
		static Datalogger* getInstance();
    	void logError(char* errorStr);
    	void appendLineInFile(char* filename, char* line);
    	void dumpFiles();
	private:
    	Datalogger();  // private so that it can  not be called
    	Datalogger(const Datalogger&) = delete;
    	Datalogger& operator=(const Datalogger&) = delete;
		static Datalogger* pInstance;
    	void loadFiles();
};

#endif
