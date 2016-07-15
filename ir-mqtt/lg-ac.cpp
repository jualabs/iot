#include <IRremoteESP8266.h>
#include <Arduino.h>

// 0 : TOWER; 1 : WALL
const int AC_TYPE  = 1;
// 0 : cooling; 1 : heating
int AC_HEAT = 0;
// 0 : off ; 1 : on
int AC_POWER_ON    = 1;
// 0 : off; 1 : on --> power on
int AC_AIR_ACLEAN  = 0;
// temperature : 18 ~ 30
int AC_TEMPERATURE = 18;
// 0 : low; 1 : mid; 2 : high
// if AC_TYPE = 1, 3 : change
int AC_FLOW = 2;

const int AC_FLOW_TOWER[3] = {0, 4, 6};
const int AC_FLOW_WALL[4]  = {0, 2, 4, 5};

unsigned long AC_CODE_TO_SEND;

void ac_send_code(unsigned long code, IRsend *irsend)
{
  for (int i=0;i<3;i++){
    irsend->sendLG(code, 28);
    delay(40);
  }
}

void ac_activate(int temperature, int air_flow, IRsend *irsend)
{

  int AC_MSBITS1 = 8;
  int AC_MSBITS2 = 8;
  int AC_MSBITS3 = 0;
  int AC_MSBITS4 ;
  if ( AC_HEAT == 1 ) {
    // heating
    AC_MSBITS4 = 4;
  } else {
    // cooling
    AC_MSBITS4 = 0;
  }
  int AC_MSBITS5 = temperature - 15;
  int AC_MSBITS6 ;

  if ( AC_TYPE == 0) {
    AC_MSBITS6 = AC_FLOW_TOWER[air_flow];
  } else {
    AC_MSBITS6 = AC_FLOW_WALL[air_flow];
  }

  int AC_MSBITS7 = (AC_MSBITS3 + AC_MSBITS4 + AC_MSBITS5 + AC_MSBITS6) & 0x0F;

  AC_CODE_TO_SEND =  AC_MSBITS1 << 4 ;
  AC_CODE_TO_SEND =  (AC_CODE_TO_SEND + AC_MSBITS2) << 4;
  AC_CODE_TO_SEND =  (AC_CODE_TO_SEND + AC_MSBITS3) << 4;
  AC_CODE_TO_SEND =  (AC_CODE_TO_SEND + AC_MSBITS4) << 4;
  AC_CODE_TO_SEND =  (AC_CODE_TO_SEND + AC_MSBITS5) << 4;
  AC_CODE_TO_SEND =  (AC_CODE_TO_SEND + AC_MSBITS6) << 4;
  AC_CODE_TO_SEND =  (AC_CODE_TO_SEND + AC_MSBITS7);

  ac_send_code(AC_CODE_TO_SEND, irsend);

  AC_POWER_ON = 1;
  AC_TEMPERATURE = temperature;
  AC_FLOW = air_flow;
}

void ac_change_air_swing(int air_swing, IRsend *irsend)
{
  if ( AC_TYPE == 0) {
    if ( air_swing == 1) {
      AC_CODE_TO_SEND = 0x881316B;
    } else {
      AC_CODE_TO_SEND = 0x881317C;
    }
  } else {
    if ( air_swing == 1) {
      AC_CODE_TO_SEND = 0x8813149;
    } else {
      AC_CODE_TO_SEND = 0x881315A;
    }
  }

  ac_send_code(AC_CODE_TO_SEND, irsend);
}

void ac_power_down(IRsend *irsend)
{
  AC_CODE_TO_SEND = 0x88C0051;

  ac_send_code(AC_CODE_TO_SEND, irsend);

  AC_POWER_ON = 0;
}

void ac_air_clean(int air_clean, IRsend *irsend)
{
  if ( air_clean == 1) {
    AC_CODE_TO_SEND = 0x88C000C;
  } else {
    AC_CODE_TO_SEND = 0x88C0084;
  }

  ac_send_code(AC_CODE_TO_SEND, irsend);

  AC_AIR_ACLEAN = air_clean;
}
