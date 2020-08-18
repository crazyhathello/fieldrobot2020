#ifndef _CLAW_CONTROL_H_
#define _CLAW_CONTROL_H_
#include "src/arm_utils.h"

#define CLAW_OPEN_DELAY 7500
#define CLAW_CLOSE_DELAY 7500
#define  CLAW_IN1 8
#define  CLAW_IN2 9

void claw_init();
void claw_control(String cmd);
void close_claw();
void open_claw();
void stop_claw();


#endif // !_ARM_CONTROL_FOR_ARD_H_
