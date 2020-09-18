#include <iostream>
#include <opencv2/opencv.hpp>
//#include "lane_tracking.h"
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

//files for mission and camera are in directory TX2forARM and there directories should not change in order to be included in the main function


using namespace std;


////////////////// global objects for Camera //////////////
Camera armCam(3,480,640);
Camera sideCam(1,640,480);
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

	bool check_camera_ready = true;
	bool check_arduino_ready = true;
	bool mission_complete = false;

	int watering_times = 2;   // local variable that stores watering count
	int apple_gripping_times = 2;  // local variable that stores gripping count


	
	//check_camera_ready = check_camera();    // Cameras are already initialized in the constructor of Camera class, but exception handle hasn`t code yet.
	//check_arduino_ready = check_arduino();  // Haven`t code yet	
	//const int initial_arm_location[3] = {x, y, z};  //arm location is set in arm_control.cpp

	while(!mission_complete && check_arduino_ready && check_camera_ready)
	{
		//if(argc == 1)   // Don`t know why is there an argment in main() ?
		//{
		//In every loop, the mission controller will decide the mission

	//	mission_state mission = mission_decision();
		mission_state mission = GRIPPING;
		switch(mission)
		{
			case TRACKING: 
				//lane_tracking();
				std::cout << "NOW Tracking..." << std::endl;
				break;

			case WATERING:
				armCam.flush_cam_buffer();

				if(watering_times == 0) break;
				std::cout << "NOW Watering..." << std::endl;
				//pot_watering();
				//watering_times--;
				//if(watering_times==0) watering_times=0;
				sideCam.flush_cam_buffer(); // flush sideCam frame buffer before reading and determining missions	
				break;

			case GRIPPING:
				armCam.flush_cam_buffer();
				
				if(apple_gripping_times == 0) break;
					std::cout << "NOW Gripping..." << std::endl;
				switch(apple_gripping_times){
					case 2:
					apple_gripping(FETCH);
					//apple_gripping_times--;   // will be uncommit in competition
					//if(apple_gripping_times==0) apple_gripping_times=0;

					std::cout<<"finished apple fetch"<<std::endl;
					break;
					case 1:
					apple_gripping(RELEASE);
					//apple_gripping_times--;
					//if(apple_gripping_times==0) apple_gripping_times=0;

						std::cout<<"finished apple release"<<std::endl;	
					break;
				}
					sideCam.flush_cam_buffer(); // flush sideCam frame buffer before reading and determining missions
				break;

				case STOP:
				std::cout << "NOW Stop!!!!!!!" << std::endl;
				break;

				default:
				break;
		}

	
		//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		
		if (cv::waitKey(10) == 27) 
		{
			std::cout << "esc key is pressed by user" << std::endl;
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


