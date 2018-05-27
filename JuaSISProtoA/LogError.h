#ifndef __LOGERROR_H__
#define __LOGERROR_H__

#include <Arduino.h>
#include <FS.h>
#include <Time.h>
#include <TimeLib.h>


class LogError {
	public:
		static LogError* getInstance();
    	void insertError(char* errorMsg);
    	void dumpErrorLog();
	private:
    	LogError();  // private so that it can  not be called
    	LogError(const LogError&) = delete;
    	LogError& operator=(const LogError&) = delete;
		static LogError* pInstance;
};

#endif
