#ifndef __PARAMETERS_H__
#define __PARAMETERS_H__

#include <Arduino.h>

#define DEBUG 1

static const uint8_t autoIrrigationStartHour = 7;
static const uint8_t autoIrrigationStartMinute = 30;
static const float MAX_ACCEPTABLE_HUM = 100.0;
static const float MIN_ACCEPTABLE_HUM = 9.9;
static const float MAX_ACCEPTABLE_TMP = 44.0;
static const float MIN_ACCEPTABLE_TMP = 13.4;

static const float efc = 0.9;
static const unsigned int ngp = 1;
static const float vg = 1.5;
static const float esp1 = 0.7;
static const float esp2 = 0.4;
static const float ia = (ngp * vg) / (esp1 * esp2);

/* the max number of days that must be considered for a system recover after a power failure */
static const uint8_t MAX_RECOVER_DAYS = 15;
/* experiment duration in days */
static const uint8_t EXPERIMENT_DURATION = 70;

static const float kc[EXPERIMENT_DURATION] = {0.8,0.8,0.8,0.8,0.8,0.8,0.8,0.8,0.8,0.8,0.8,0.8,0.8,0.8,0.8,0.812,0.824,0.836,0.848,
                      	  	 0.86,0.872,0.884,0.896,0.908,0.92,0.932,0.944,0.956,0.968,0.98,0.992,1.004,1.016,1.028,
							 1.04,1.052,1.064,1.076,1.088,1.1,1.117647059,1.135294118,1.152941176,1.170588235,1.188235294,
							 1.205882353,1.223529412,1.241176471,1.258823529,1.276470588,1.294117647,1.311764706,1.329411765,
							 1.347058824,1.364705882,1.382352941,1.4,1.315384615,1.230769231,1.146153846,1.061538462,0.976923077,
							 0.892307692,0.807692308,0.723076923,0.638461538,0.553846154,0.469230769,0.384615385,0.3};

#endif
