#ifndef _ARMRIGHT_CAM_CONFIG_H_
#define _ARMRIGHT_CAM_CONFIG_H_

#include <opencv2/opencv.hpp>
#include "armright_cam_utils.h"
#include <iostream>
#include <string>
#include <vector>
#include "timer.h"


#define RECORD_VIDEO 0
#define CALIBRATE 0
#define SHOWDEBUG 0 
#define SET_CAMERA_CONFIG 1

#define CAMERA_BUFFER_FRAMES 10

enum exposure {
	OUT,
	OUT_SUNLIGHT,
	IN
};

enum MissionColors
{
	RED,
	BLUE,
	YELLOW,
	OTHER
};


enum CameraParameters
{
	APPLE,
	PLATE,
	POT,
	REDSIGN,
	BLUESIGN,
	YELLOWSIGN
};


class Camera {

private:
	cv::Mat mFrame;

	cv::Scalar mLowBottomHue_test;
	cv::Scalar mLowTopHue_test;
	cv::Scalar mHighBottomHue_test;
	cv::Scalar mHighTopHue_test;

	double mArea_thres;
	double mH_w_high_thres;
	double mH_w_low_thres;
	double mFps = 30.0;
	int mWidth = 0;
	int mHeight = 0;

	int mDil_iter;
	int mEro_iter;

	cv::Size mDil_ksize;
	cv::Size mEro_ksize;

	int32_t mCali_num = 0;
	int mCali_ksize = 3;
public:

	exposure mExpose = IN;
	cv::VideoCapture mCap;	
	Camera() {};
	Camera(int index, int width, int height) :mCap(index) { init_cam(width, height); };
	~Camera();
	int init_cam(int, int);
	int getWidth()	const { return mWidth;}
	int getHeight()	const { return mHeight;}

	//set the parameters of the Camera(area,filter color...etc) corresponding to the input mission(apple,pot,plate,redsign...etc )
	void set_parameter(CameraParameters parameters);


	// flush the camera buffer to avoid repeated image read
	bool flush_cam_buffer();


////////////////////// detection utils /////////////////////////////////////

	// fetch the target`s centroid, passing the target`s coordinate by reference and return True if no errors occur
	bool Detect(int& dest_x, int& dest_y);


// fetch the target`s centroid, passing the target`s coordinate as well as the detected mission color by reference and return True if no errors occur
	bool Detect_side(int& dest_x, int& dest_y, MissionColors& color);


//Used for scanning the given CameraParameters(e.g. parameters for red sign) ,return True if condition is met
	bool Detect_side_scanner(cv::Mat& hsv_img, cv::Mat& filtered_hsv, int& dest_x, int& dest_y, CameraParameters colorParameter);
////////////////////// detection utils ///////////////////////////////////////////////////

};

extern Camera armCam;
extern Camera sideCam;

void cam_expos_cali(cv::VideoCapture& cap, exposure& expose);

#endif // !_ARMRIGHT_CAM_CONFIG

