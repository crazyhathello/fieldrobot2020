/*
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
*/
#include <opencv2/opencv.hpp>


#include <iostream>
#include <string>
#include <vector>
#include "timer.h"
#include "armright_cam_utils.h"
#include "armright_cam_config.h"
#include "arm_control.h"
#include "../send_to_arduino.h"
#include "apple_gripping.h"
#include "mission_decision.h"
#include "pot_watering.h"


//using namespace std;
//using namespace cv;

MissionController missionController;
//////////////////globale variables for Camera//////////////

Camera armCam(1);
Camera sideCam(1);
//////////////////globale variables for Camera//////////////



#if (IMG_MODE==0 && RECORD_VIDEO==1)
cv::VideoWriter video_filtering("pot_f.wmv", CV_FOURCC('W', 'M', 'V', '1'), armCam.mFps, cv::Size(armCam.mWidth, armCam.mHeight), false);
cv::VideoWriter video("pot.wmv", CV_FOURCC('W', 'M', 'V', '1'), armCam.mFps, cv::Size(armCam.mWidth, armCam.mHeight), true);
#endif

int main(int argc, char **argv)
{


	int apple_x, apple_y;
	Timer module_t;
	mission_state mission;
	
	while (true)
	{
		//module_t.start();
		//mission = missionController.mission_decision();
		//module_t.display("mission decision time");
		mission = GRIPPING;
		switch (mission)
		{
		case TRACKING:
			std::cout << "NOW Tracking..." << std::endl;
			
			break;
		case WATERING:
			std::cout << "NOW Watering..." << std::endl;
			//arm_Controller.move_to_pos(watering_pos);
			//pot_watering();
			break;
		case GRIPPING:
			std::cout << "NOW Gripping..." << std::endl;
			//arm_Controller.move_to_pos(gripping_pos);
			apple_gripping(FETCH);
			
			break;
		case STOP:
			std::cout << "NOW Stop!!!!!!!" << std::endl;
			break;
		default:
			break;
		}
		
	

		
		if (cv::waitKey(10) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			std::cout << "esc key is pressed by user" << std::endl;
			break;
		}
	}
	
	//apple_gripping(FETCH);


	return 0;
}







