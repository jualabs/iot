/*
 * Context.h
 *
 *  Created on: 11 de mai de 2018
 *      Author: victormedeiros
 */

#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <Arduino.h>

class Context {
	public:
		Context();
		enum class State {STAND_BY, RUNNING, GET_DATA, FAILED};
		void resetHourContext();
		void resetDayContext();
		unsigned int getAutoIrrigationDuration() const;
		void setAutoIrrigationDuration(unsigned int autoIrrigationDuration);
		unsigned long getAutoIrrigationStartTime() const;
		void setAutoIrrigationStartTime(unsigned long autoIrrigationStartTime);
		unsigned int getCurrentDay() const;
		void setCurrentDay(unsigned int currentDay);
		unsigned int getCurrentHour() const;
		void setCurrentHour(unsigned int currentHour);
		unsigned int getCurrentMinute() const;
		void setCurrentMinute(unsigned int currentMinute);
		bool getIsAutoIrrigationSuspended() const;
		void setIsAutoIrrigationSuspended(bool isAutoIrrigationSuspended);
		bool getIsManuallyIrrigating() const;
		void setIsManuallyIrrigating(bool isManuallyIrrigating);
		unsigned long getManIrrigationStartTime() const;
		void setManIrrigationStartTime(unsigned long manIrrigationStartTime);
		float getOneDayAvgHum() const;
		void setOneDayAvgHum(float oneDayAvgHum);
		float getOneDayAvgTemp() const;
		void setOneDayAvgTemp(float oneDayAvgTemp);
		float getOneDayMaxHum() const;
		void setOneDayMaxHum(float oneDayMaxHum);
		float getOneDayMaxTemp() const;
		void setOneDayMaxTemp(float oneDayMaxTemp);
		float getOneDayMinHum() const;
		void setOneDayMinHum(float oneDayMinHum);
		float getOneDayMinTemp() const;
		void setOneDayMinTemp(float oneDayMinTemp);
		float getOneHourAvgHum() const;
		void setOneHourAvgHum(float oneHourAvgHum);
		float getOneHourAvgTemp() const;
		void setOneHourAvgTemp(float oneHourAvgTemp);
		float getOneHourMaxHum() const;
		void setOneHourMaxHum(float oneHourMaxHum);
		float getOneHourMaxTemp() const;
		void setOneHourMaxTemp(float oneHourMaxTemp);
		float getOneHourMinHum() const;
		void setOneHourMinHum(float oneHourMinHum);
		float getOneHourMinTemp() const;
		void setOneHourMinTemp(float oneHourMinTemp);
		State getCurrentState() const;
		void setCurrentState(State currentState);
		char* getCurrentContextString(uint32_t ts);

	private:
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
