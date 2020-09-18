#ifndef _MISSION_DECISION_H_
#define _MISSION_DECISION_H_
#include "TX2forARM/armright_cam_config.h"

#define MISSION_THRES_X 320
#define MISSION_THRES_Y 240



enum mission_state
{
	TRACKING,
	WATERING,
	GRIPPING,
	STOP
};


	
mission_state mission_decision(Camera&);


bool trigger_mission(int sign_x, int sign_y);

#endif // !_MISSION_DECISION_H_
