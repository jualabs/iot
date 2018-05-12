#include "Context.h"

Context::Context() {
	isManuallyIrrigating = false;
	isAutoIrrigationSuspended = false;
	autoIrrigationStartTime = 0;
	autoIrrigationDuration = 0;
	manIrrigationStartTime = 0;
	currentMinute = 0;
	currentHour = 0;
	currentDay = 0;
	oneHourMaxTemp = -50.0;
	oneHourMaxHum = 0.0;
	oneHourMinTemp = 50.0;
	oneHourMinHum = 100.0;
	oneHourAvgTemp = 0.0;
	oneHourAvgHum = 0.0;
	oneDayMaxTemp= -50.0;
	oneDayMaxHum = 0.0;
	oneDayMinTemp = 50.0;
	oneDayMinHum = 100.0;
	oneDayAvgTemp = 0.0;
	oneDayAvgHum = 0.0;
}

void Context::resetHourContext() {
	currentMinute = 0;
	oneHourMaxTemp = -50.0;
	oneHourMaxHum = 0.0;
	oneHourMinTemp = 50.0;
	oneHourMinHum = 100.0;
	oneHourAvgTemp = 0.0;
	oneHourAvgHum = 0.0;
}

void Context::resetDayContext() {
	currentHour = 0;
	oneDayMaxTemp = -50.0;
	oneDayMaxHum = 0.0;
	oneDayMinTemp = 50.0;
	oneDayMinHum = 100.0;
	oneDayAvgTemp = 0.0;
	oneDayAvgHum = 0.0;
}

unsigned int Context::getAutoIrrigationDuration() const {
	return autoIrrigationDuration;
}

void Context::setAutoIrrigationDuration(unsigned int autoIrrigationDuration) {
	this->autoIrrigationDuration = autoIrrigationDuration;
}

unsigned long Context::getAutoIrrigationStartTime() const {
	return autoIrrigationStartTime;
}

void Context::setAutoIrrigationStartTime(
		unsigned long autoIrrigationStartTime) {
	this->autoIrrigationStartTime = autoIrrigationStartTime;
}

unsigned int Context::getCurrentDay() const {
	return currentDay;
}

void Context::setCurrentDay(unsigned int currentDay) {
	this->currentDay = currentDay;
}

unsigned int Context::getCurrentHour() const {
	return currentHour;
}

void Context::setCurrentHour(unsigned int currentHour) {
	this->currentHour = currentHour;
}

unsigned int Context::getCurrentMinute() const {
	return currentMinute;
}

void Context::setCurrentMinute(unsigned int currentMinute) {
	this->currentMinute = currentMinute;
}

bool Context::getIsAutoIrrigationSuspended() const {
	return isAutoIrrigationSuspended;
}

void Context::setIsAutoIrrigationSuspended(bool isAutoIrrigationSuspended) {
	this->isAutoIrrigationSuspended = isAutoIrrigationSuspended;
}

bool Context::getIsManuallyIrrigating() const {
	return isManuallyIrrigating;
}

void Context::setIsManuallyIrrigating(bool isManuallyIrrigating) {
	this->isManuallyIrrigating = isManuallyIrrigating;
}

unsigned long Context::getManIrrigationStartTime() const {
	return manIrrigationStartTime;
}

void Context::setManIrrigationStartTime(unsigned long manIrrigationStartTime) {
	this->manIrrigationStartTime = manIrrigationStartTime;
}

float Context::getOneDayAvgHum() const {
	return oneDayAvgHum;
}

void Context::setOneDayAvgHum(float oneDayAvgHum) {
	this->oneDayAvgHum = oneDayAvgHum;
}

float Context::getOneDayAvgTemp() const {
	return oneDayAvgTemp;
}

void Context::setOneDayAvgTemp(float oneDayAvgTemp) {
	this->oneDayAvgTemp = oneDayAvgTemp;
}

float Context::getOneDayMaxHum() const {
	return oneDayMaxHum;
}

void Context::setOneDayMaxHum(float oneDayMaxHum) {
	this->oneDayMaxHum = oneDayMaxHum;
}

float Context::getOneDayMaxTemp() const {
	return oneDayMaxTemp;
}

void Context::setOneDayMaxTemp(float oneDayMaxTemp) {
	this->oneDayMaxTemp = oneDayMaxTemp;
}

float Context::getOneDayMinHum() const {
	return oneDayMinHum;
}

void Context::setOneDayMinHum(float oneDayMinHum) {
	this->oneDayMinHum = oneDayMinHum;
}

float Context::getOneDayMinTemp() const {
	return oneDayMinTemp;
}

void Context::setOneDayMinTemp(float oneDayMinTemp) {
	this->oneDayMinTemp = oneDayMinTemp;
}

float Context::getOneHourAvgHum() const {
	return oneHourAvgHum;
}

void Context::setOneHourAvgHum(float oneHourAvgHum) {
	this->oneHourAvgHum = oneHourAvgHum;
}

float Context::getOneHourAvgTemp() const {
	return oneHourAvgTemp;
}

void Context::setOneHourAvgTemp(float oneHourAvgTemp) {
	this->oneHourAvgTemp = oneHourAvgTemp;
}

float Context::getOneHourMaxHum() const {
	return oneHourMaxHum;
}

void Context::setOneHourMaxHum(float oneHourMaxHum) {
	this->oneHourMaxHum = oneHourMaxHum;
}

float Context::getOneHourMaxTemp() const {
	return oneHourMaxTemp;
}

void Context::setOneHourMaxTemp(float oneHourMaxTemp) {
	this->oneHourMaxTemp = oneHourMaxTemp;
}

float Context::getOneHourMinHum() const {
	return oneHourMinHum;
}

void Context::setOneHourMinHum(float oneHourMinHum) {
	this->oneHourMinHum = oneHourMinHum;
}

float Context::getOneHourMinTemp() const {
	return oneHourMinTemp;
}

void Context::setOneHourMinTemp(float oneHourMinTemp) {
	this->oneHourMinTemp = oneHourMinTemp;
}
