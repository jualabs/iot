#ifndef __DATALOGGER_H__
#define __DATALOGGER_H__

#include <Arduino.h>
#include <FS.h>
#include <string.h>
#include <Time.h>
#include <TimeLib.h>

static const unsigned int NUM_CSV_FILES = 4;
static constexpr char csv_files[NUM_CSV_FILES][20]= {"/hour.csv","/day.csv","/aut-irrig.csv","/man-irrig.csv"};
static constexpr char csv_files_headers[NUM_CSV_FILES][80] = {"ts,tmp_min,tmp_max,tmp_avg,hum_min,hum_max,hum_avg",
    	    												  "ts,tmp_avg,hum_avg,kc,eto,irrig_duration",
    														  "ts_init,ts_stop,duration",
    														  "ts_init,ts_stop,duration"};

class Datalogger {
	public:
		static Datalogger* getInstance();
		void formatFS();
    	void appendLineInFile(const char* filename, const char* line);
    	void dumpFiles();
	private:
    	Datalogger();  // private so that it can  not be called
    	Datalogger(const Datalogger&) = delete;
    	Datalogger& operator=(const Datalogger&) = delete;
		static Datalogger* pInstance;
    	void loadFiles();
};

#endif
