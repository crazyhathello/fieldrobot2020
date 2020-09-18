#ifndef _PUMP_CONTROL_H_
#define _PUMP_CONTROL_H_
#include "Arduino.h"


#define  PUMP_OPEN_DELAY_1 8500
#define  PUMP_OPEN_DELAY_2 7500
#define PUMP_IN1 12
#define PUMP_IN2 13

void pump_init();
void open_pump();
void close_pump();
void pump_control(String cmd);
#endif
