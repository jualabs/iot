#include "sdkconfig.h"
#include "stdint.h"
#include "esp_timer.h"

extern "C" {
   	int app_main(void);
}

const int64_t ACCEL_INTERVAL_US = 100000;
const int64_t GPS_INTERVAL_US = 6000000;

extern void initGPS();
extern void readGPS();
extern void smartDelay(int64_t us);

extern void initSD();

extern void initAccelerometer();
extern void getAccelData();

void setup() {
  // init gps configurations
  initGPS();
  initSD();
  initAccelerometer();
}

int app_main(void) {
  int64_t start = esp_timer_get_time();
  setup();

  while(true) {
    smartDelay(ACCEL_INTERVAL_US);
    // getAccelData();
    if ((esp_timer_get_time()) - start >= GPS_INTERVAL_US) {
      readGPS();
      start = esp_timer_get_time();
    }
  }
  
  return 0;
}