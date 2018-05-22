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

#define STATE_LED_PIN D4

class Context {
	public:
		static Context* getInstance();
		enum class State {STAND_BY, RUNNING, GET_DATA, FAILED};
		void resetHourContext();
		void resetDayContext();
		unsigned int getAutoIrrigationDuration();
		void setAutoIrrigationDuration(unsigned int duration);
		unsigned long getAutoIrrigationStartTime();
		void setAutoIrrigationStartTime(unsigned long startTime);
		unsigned int getCurrentDay();
		void setCurrentDay(unsigned int curDay);
		unsigned int getCurrentHour();
		void setCurrentHour(unsigned int curHour);
		unsigned int getCurrentMinute();
		void setCurrentMinute(unsigned int curMinute);
		bool getIsAutoIrrigationSuspended();
		void setIsAutoIrrigationSuspended(bool suspendedFlag);
		bool getIsManuallyIrrigating();
		void setIsManuallyIrrigating(bool manuallyIrrigatingFlag);
		unsigned long getManIrrigationStartTime();
		void setManIrrigationStartTime(unsigned long startTime);
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
		char* getCurrentContextString(uint32_t ts);
		void changeState(State toState);
		JLed* getStateLed();
		void initContext();
	private:
		Context();  // private so that it can  not be called
	    Context(const Context&) = delete;
	    Context& operator=(const Context&) = delete;
		static Context* m_pInstance;
		JLed stateLed;
		State currentState;
		bool isManuallyIrrigating;
		bool isAutoIrrigationSuspended;
		unsigned long autoIrrigationStartTime;
		unsigned int autoIrrigationDuration;
		unsigned long manIrrigationStartTime;
		unsigned int currentMinute;
		unsigned int currentHour;
		unsigned int currentDay;
		float   oneHourMaxTemp;
		float   oneHourMaxHum;
		float   oneHourMinTemp;
		float   oneHourMinHum;
		float   oneHourAvgTemp;
		float   oneHourAvgHum;
		float   oneDayMaxTemp;
		float   oneDayMaxHum;
		float   oneDayMinTemp;
		float   oneDayMinHum;
		float   oneDayAvgTemp;
		float   oneDayAvgHum;
};

#endif /* __CONTEXT_H___ */
