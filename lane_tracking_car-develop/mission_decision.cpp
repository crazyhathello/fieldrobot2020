#include "mission_decision.h"
#include "opencv2/opencv.hpp"
#include "TX2forARM/armright_cam_config.h"
#include "unistd.h"

using namespace cv;

mission_state mission_decision(Camera& sideCam)
{
	int sign_x, sign_y;
	MissionColors color = OTHER;   //default color, if none of 'RED' 'BLUE' 'YELLOW' is detected, then the default color is 'OTHER' 
	mission_state mission;  
//	Camera sideCam(cam_index, 640, 480);

	while (!sideCam.mCap.isOpened())
	{
		cout << "cannot open side camera";
		sleep(1);
	}

	// Side cam will detect color and pass by reference to variable 'color' , return True if frame is fetch successfully no matter sign is detected or not
	if (sideCam.Detect_side(sign_x, sign_y, color) > 0) 
	{
		switch (color)
		{
			case RED:
				if(trigger_mission(sign_x,sign_y)>0)
					mission = STOP;
				break;
			case BLUE:
				if (trigger_mission(sign_x, sign_y)>0)
					mission = WATERING;
				break;
			case YELLOW:
				if (trigger_mission(sign_x, sign_y)>0)
					mission = GRIPPING;
				break;
			case OTHER:
				mission = TRACKING;
				break;
			default:
				break;
		}
	}

	return mission;
}


bool trigger_mission(int sign_x, int sign_y) {

	bool triggered = false;
	if (sign_x > MISSION_THRES_X ) {
		triggered = true;
	}

	return triggered;
}
