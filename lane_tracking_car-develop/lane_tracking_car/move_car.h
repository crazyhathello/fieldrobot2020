#ifndef _MOVE_CAR_H_
#define _MOVE_CAR_H_
#include <Wire.h>
#include "PID_v1.h"
#include "Arduino.h"

void car_init();
void stop();
void move_car(int);
void motor_R_motion(int);
void motor_L_motion(int);

#endif
