#include "SystemController.h"

SystemController::SystemController() : context(Context::getInstance()), rtc(DS1302RTC(D0, D2, D1)), datalogger(Datalogger::getInstance()),
									   sensors(Sensors::getInstance()), actuators(Actuators::getInstance()),
									   timeEventsHandler(TimeEventsHandler::getInstance()), comm(Communication::getInstance()),
									   buttonEventsHandler(ButtonEventsHandler::getInstance()) {};

void SystemController::setup() {
	/* initialize communication */
	// comm->initCommunication();
	/* initialize timing parameters and RTC */
	setupTime();
	/* initialize time events */
	timeEventsHandler->initTimeEvents();
	/* initialize the buttons events */
	buttonEventsHandler->initButtons();
	/* try to recover context */
	if(context->recoverContext()) {
		/* if recovered restart the process through time events*/
		timeEventsHandler->startTimeEvents();
	}

#if 0
#ifdef DEBUG
	String str = "";
	Dir dir = SPIFFS.openDir("/");
	while (dir.next()) {
	    str += dir.fileName();
	    str += " / ";
	    str += dir.fileSize();
	    str += "\r\n";
	}
	Serial.print(str);
#endif
#endif
}

void SystemController::loop() {
	stateLedUpdate();
	timeEventsHandler->checkTimeEvents();
	buttonEventsHandler->checkButtonEvents();
	comm->checkCommunication();
	/* if entered in SET_TIME state */
	if(context->getCurrentState() == Context::State::SET_TIME) {
		setDateTimeFromSerial();
	}
}

void SystemController::stateLedUpdate() {
	context->getStateLed()->Update();
}

void SystemController::setupTime() {
	setSyncProvider(rtc.get);
	if(timeStatus() != timeSet) {
		Serial.println("Unable to sync with the RTC");
	}
	else {
		Serial.println("RTC has set the system time");
	}
	timeEventsHandler->printTime();
}

void SystemController::setDateTimeFromSerial() {
	time_t t;
	tmElements_t tm;
	/* check for input to set the RTC, minimum length is 12, i.e. yy,m,d,h,m,s */
	if (Serial.available() >= 12) {
		/* note that the tmElements_t Year member is an offset from 1970,
		 * but the RTC wants the last two digits of the calendar year.
		 * use the convenience macros from Time.h to do the conversions. */
		int y = Serial.parseInt();
		if (y >= 100 && y < 1000)
			Serial.println("ERROR: year must be two digits or four digits!");
		else {
			if (y >= 1000) {
				tm.Year = CalendarYrToTm(y);
			}
			else {
                tm.Year = y2kYearToTm(y);
			}
			tm.Month = Serial.parseInt();
			tm.Day = Serial.parseInt();
			tm.Hour = Serial.parseInt();
			tm.Minute = Serial.parseInt();
			tm.Second = Serial.parseInt();
			t = makeTime(tm);
			/* use the time_t value to ensure correct weekday is set */
			if(rtc.set(t) == 0) {
				setTime(t);
				Serial.print("RTC set to: ");
				char datetime[] = "YYYY-MM-DD 00:00:00";
				sprintf(datetime, "%04i-%02i-%02i %02i:%02i:%02i", year(), month(), day(), hour(), minute(), second());
				Serial.println(datetime);
				/* try to recover context with new datetime */
				context->recoverContext();
			}
			else
				Serial.print("ERROR: unable to set date and time!");
			/* dump any extraneous input */
			while (Serial.available() > 0) Serial.read();
		}
	}
}

