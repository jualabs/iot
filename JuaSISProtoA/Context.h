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
		Context();
		enum class State {STAND_BY, RUNNING, GET_DATA, FAILED};
		static void resetHourContext();
		static void resetDayContext();
		static unsigned int getAutoIrrigationDuration();
		static void setAutoIrrigationDuration(unsigned int duration);
		static unsigned long getAutoIrrigationStartTime();
		static void setAutoIrrigationStartTime(unsigned long startTime);
		static unsigned int getCurrentDay();
		static void setCurrentDay(unsigned int curDay);
		static unsigned int getCurrentHour();
		static void setCurrentHour(unsigned int curHour);
		static unsigned int getCurrentMinute();
		static void setCurrentMinute(unsigned int curMinute);
		static bool getIsAutoIrrigationSuspended();
		static void setIsAutoIrrigationSuspended(bool suspendedFlag);
		static bool getIsManuallyIrrigating();
		static void setIsManuallyIrrigating(bool manuallyIrrigatingFlag);
		static unsigned long getManIrrigationStartTime();
		static void setManIrrigationStartTime(unsigned long startTime);
		static float getOneDayAvgHum();
		static void setOneDayAvgHum(float avgHum);
		static float getOneDayAvgTemp();
		static void setOneDayAvgTemp(float avgTemp);
		static float getOneDayMaxHum();
		static void setOneDayMaxHum(float maxHum);
		static float getOneDayMaxTemp();
		static void setOneDayMaxTemp(float maxTemp);
		static float getOneDayMinHum();
		static void setOneDayMinHum(float minHum);
		static float getOneDayMinTemp();
		static void setOneDayMinTemp(float minTemp);
		static float getOneHourAvgHum();
		static void setOneHourAvgHum(float avgHum);
		static float getOneHourAvgTemp();
		static void setOneHourAvgTemp(float avgTemp);
		static float getOneHourMaxHum();
		static void setOneHourMaxHum(float maxHum);
		static float getOneHourMaxTemp();
		static void setOneHourMaxTemp(float maxTemp);
		static float getOneHourMinHum();
		static void setOneHourMinHum(float minHum);
		static float getOneHourMinTemp();
		static void setOneHourMinTemp(float minTemp);
		static State getCurrentState();
		static char* getCurrentStateString();
		static void setCurrentState(State currentState);
		static char* getCurrentContextString(uint32_t ts);
		static void changeState(State toState);
		static JLed* getStateLed();
		static void initContext();
	private:
		static JLed stateLed;
		static State currentState;
		static bool isManuallyIrrigating;
		static bool isAutoIrrigationSuspended;
		static unsigned long autoIrrigationStartTime;
		static unsigned int autoIrrigationDuration;
		static unsigned long manIrrigationStartTime;
		static unsigned int currentMinute;
		static unsigned int currentHour;
		static unsigned int currentDay;
		static float   oneHourMaxTemp;
		static float   oneHourMaxHum;
		static float   oneHourMinTemp;
		static float   oneHourMinHum;
		static float   oneHourAvgTemp;
		static float   oneHourAvgHum;
		static float   oneDayMaxTemp;
		static float   oneDayMaxHum;
		static float   oneDayMinTemp;
		static float   oneDayMinHum;
		static float   oneDayAvgTemp;
		static float   oneDayAvgHum;
};

#endif /* __CONTEXT_H___ */
