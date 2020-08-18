#ifndef _POT_WATERING_H_
#define _POT_WATERING_H_
#include "armright_cam_config.h"
#include "arm_control.h"
#include "../send_to_arduino.h"
#include "unistd.h"

const int  WATERING_TIME1 = 5.5;
const int  WATERING_TIME2 = 6.5;

void pot_watering(int,int);

#endif // !_POT_WATERING_H_
