#include "sdkconfig.h"

// extern "C" {
//   	int app_main(void);
// }

extern void initGPS();
extern void readGPS();

void setup() {
  // init gps configurations
  initGPS();
}

int app_main(void)
{
  setup();
  
  return 0;
}