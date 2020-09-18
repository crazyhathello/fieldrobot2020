#include <iostream>
#include <opencv2/opencv.hpp>
#include "lane_tracking.h"
#include <string>
#include <vector>
#include "TX2forARM/timer.h"
#include "TX2forARM/armright_cam_utils.h"
#include "TX2forARM/armright_cam_config.h"
#include "TX2forARM/arm_control.h"
#include "send_to_arduino.h"
#include "TX2forARM/apple_gripping.h"
#include "mission_decision.h"
#include "TX2forARM/pot_watering.h"
#include "check_camera_ready.h"

#define side_cam_index 1
#define front_cam_index 2
#define arm_cam_index 3

//files for mission and camera are in directory TX2forARM and there directories should not change in order to be included in the main function

using namespace std;
using namespace cv;

//Camera sideCam(side_cam_index, 640, 480);
//Camera armCam(arm_cam_index, 480, 640);

////////////////// global objects for Camera //////////////
//Camera armCam(1);
//Camera sideCam(2);
////////////////// global objects for Camera //////////////

//////////////////////////////////// recording ///////////////////////////////////////////////
#if  RECORD_VIDEO==1
cv::VideoWriter video_filtering("pot_f.wmv", CV_FOURCC('W', 'M', 'V', '1'), armCam.mFps, cv::Size(armCam.mWidth, armCam.mHeight), false);
cv::VideoWriter video("pot.wmv", CV_FOURCC('W', 'M', 'V', '1'), armCam.mFps, cv::Size(armCam.mWidth, armCam.mHeight), true);
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	int apple_x, apple_y;  
	Timer module_t;        //used for counting time

	bool camera_ready = true;
	bool arduino_ready = true;
	bool mission_complete = false;

	camera_ready = check_camera_ready(side_cam_index, "side camera");
	camera_ready = check_camera_ready(front_cam_index, "front camera");
	camera_ready = check_camera_ready(arm_cam_index, "arm camera");

	Camera sideCam(side_cam_index, 640, 480);
	VideoCapture front_camera(front_cam_index);
	front_camera.set(CAP_PROP_FRAME_WIDTH, 320);
	front_camera.set(CAP_PROP_FRAME_HEIGHT, 240);
	front_camera.set(CV_CAP_PROP_BUFFERSIZE, 2);
	front_camera.set(CAP_PROP_FPS, 30);
	Camera* sideCamPtr = &sideCam ;   //create a local pointer to store caps

	if(camera_ready) cout << "3 Cameras are ready\n";

	int watering_times = 2;   // local variable that stores watering count
	int apple_gripping_times = 2;  // local variable that stores gripping count
	bool arm_mission = false;
	//check_camera_ready = check_camera();    // Cameras are already initialized in the constructor of Camera class, but exception handle hasn`t code yet.
	//check_arduino_ready = check_arduino();  // Haven`t code yet	
	//const int initial_arm_location[3] = {x, y, z};  //arm location is set in arm_control.cpp

	while(!mission_complete && arduino_ready && camera_ready)
	{
		//In every loop, the mission controller will decide the mission
		if(arm_mission)
		{
			//Camera sideCam(side_cam_index, 640, 480);
			sideCamPtr = new Camera(side_cam_index, 640, 480);
			arm_mission = false;
			cout << "reopne side cam" << endl; 
		}

		mission_state mission = mission_decision(*sideCamPtr);
		//mission_state mission = WATERING;
		switch(mission)
		{
			case TRACKING: 
				lane_tracking(front_camera, 320, 240);
				cout << "NOW Tracking..." << endl;
				break;

			case WATERING:
//				armCam.flush_cam_buffer();
				sideCamPtr->mCap.release();
				if(watering_times == 0) break;
				cout << "NOW Watering..." << endl;
				pot_watering(arm_cam_index,watering_times);
				arm_mission = true;				
				//watering_times--;
				//if(watering_times==0) watering_times=0;
//				sideCam.flush_cam_buffer(); // flush sideCam frame buffer before reading and determining missions	
				break;

			case GRIPPING:
//				armCam.flush_cam_buffer();
				sideCamPtr->mCap.release();
				if(apple_gripping_times == 0) break;
				cout << "NOW Gripping..." << endl;
				switch(apple_gripping_times){
				case 2:
					apple_gripping(arm_cam_index, FETCH);
					//apple_gripping_times--;   // will be uncommit in competition
					//if(apple_gripping_times==0) apple_gripping_times=0;

					cout << "finished apple fetch" << endl;
					break;
				case 1:
					apple_gripping(arm_cam_index, RELEASE);
					//apple_gripping_times--;
					//if(apple_gripping_times==0) apple_gripping_times=0;

					cout << "finished apple release" << endl;	
					break;
				}
//				sideCam.flush_cam_buffer(); // flush sideCam frame buffer before reading and determining missions
				arm_mission = true;
				break;

			case STOP:
				cout << "NOW Stop!!!!!!!" << endl;
				send_to_arduino("stop");
				break;

			default:
				cout << "default: noooooo~~~" << endl;
				break;
		}

	
		//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		
		if (cv::waitKey(1) == 27) 
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}

	}

	return 0;
}

/*
void lane_tracking()
{
	int turn_value = turn_determination(); //read from front camera, return the turn_value
	send_to_arduino("move" + String(turn_value)); //range from -100 ~ 100 means the grade of left to right
}
*/

/*
void set_times(string type_of_mission)
{
	if(type_of_mission == "gripping")
	{
		set_time_out(5);
		apple_gripping_times--;
	}else if(type_of_mission == "watering")
	{
		set_time_out(5);
		watering_times--;
	}
}

*/


