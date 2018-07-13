/*
 * Context.h
 *
 *  Created on: 11 de mai de 2018
 *      Author: victormedeiros
 */

#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <Arduino.h>
#include <jled.h>
#include <FS.h>
#include <Time.h>
#include <TimeLib.h>
#include "LogError.h"
#include "Parameters.h"

#define STATE_LED_PIN D4

class Context {
	public:
		static Context* getInstance();
		enum class State {STAND_BY, RUNNING, GET_DATA, FAILED, SET_TIME, STOPPED};
		void resetHourContext();
		void resetDayContext();
		uint16_t getAutoIrrigationDuration();
		void setAutoIrrigationDuration(uint16_t duration);
		uint16_t getLastValidAutoIrrigationDuration();
		void setLastValidAutoIrrigationDuration(uint16_t duration);
		uint32_t getAutoIrrigationStartTime();
		void setAutoIrrigationStartTime(uint32_t startTime);
		uint16_t getCurrentDay();
		void setCurrentDay(uint16_t curDay);
		uint8_t getCurrentHour();
		void setCurrentHour(uint8_t curHour);
		uint8_t getCurrentMinute();
		void setCurrentMinute(uint8_t curMinute);
		bool getIsAutoIrrigationSuspended();
		void setIsAutoIrrigationSuspended(bool suspendedFlag);
		bool getIsManuallyIrrigating();
		void setIsManuallyIrrigating(bool manuallyIrrigatingFlag);
		uint32_t getManIrrigationStartTime();
		void setManIrrigationStartTime(uint32_t startTime);
		float getOneDayAvgHum();
		void setOneDayAvgHum(float avgHum);
		float getOneDayAvgTemp();
		void setOneDayAvgTemp(float avgTemp);
		float getOneDayMaxHum();
		void setOneDayMaxHum(float maxHum);
		float getOneDayMaxTemp();
		void setOneDayMaxTemp(float maxTemp);
		float getOneDayMinHum();
		void setOneDayMinHum(float minHum);
		float getOneDayMinTemp();
		void setOneDayMinTemp(float minTemp);
		float getOneHourAvgHum();
		void setOneHourAvgHum(float avgHum);
		float getOneHourAvgTemp();
		void setOneHourAvgTemp(float avgTemp);
		float getOneHourMaxHum();
		void setOneHourMaxHum(float maxHum);
		float getOneHourMaxTemp();
		void setOneHourMaxTemp(float maxTemp);
		float getOneHourMinHum();
		void setOneHourMinHum(float minHum);
		float getOneHourMinTemp();
		void setOneHourMinTemp(float minTemp);
		State getCurrentState();
		char* getCurrentStateString();
		void setCurrentState(State currentState);
		char* getCurrentContextString();
		void changeState(State toState);
		JLed* getStateLed();
		bool recoverContext();
		void saveContext();
		void printContextSerial();
	private:
		Context();  // private so that it can  not be called
	    Context(const Context&) = delete;
	    Context& operator=(const Context&) = delete;
		static Context* pInstance;
		JLed stateLed;
		State currentState;
		bool isManuallyIrrigating;
		bool isAutoIrrigationSuspended;
		uint32_t autoIrrigationStartTime;
		uint16_t autoIrrigationDuration;
		uint16_t lastValidAutoIrrigationDuration;
		uint32_t manIrrigationStartTime;
		uint8_t currentMinute;
		uint8_t currentHour;
		uint16_t currentDay;
		float oneHourMaxTemp;
		float oneHourMaxHum;
		float oneHourMinTemp;
		float oneHourMinHum;
		float oneHourAvgTemp;
		float oneHourAvgHum;
		float oneDayMaxTemp;
		float oneDayMaxHum;
		float oneDayMinTemp;
		float oneDayMinHum;
		float oneDayAvgTemp;
		float oneDayAvgHum;
};

#endif /* __CONTEXT_H___ */
