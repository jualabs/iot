#ifndef __BTN_EVENTS_HANDLER_H__
#define __BTN_EVENTS_HANDLER_H__

#include <Arduino.h>
#include <Time.h>
#include <TimeLib.h>
#include <DS1302RTC.h>
#include <TimeAlarms.h>

void initTimeEventsHandler();
void checkTimeEvents();
void cfgMinAndHourTimerEventHandler();
void minTimeEventHandler();
void hourTimeEventHandler();
void dailyTimeEventHandler();
void stopAutoIrrigationTimeEventHandler();

#endif
