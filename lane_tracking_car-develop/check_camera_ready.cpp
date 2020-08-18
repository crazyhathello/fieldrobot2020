#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include "check_camera_ready.h"
#include <string>

using namespace std;
using namespace cv;

bool check_camera_ready(int index, string camera_name)
{
	VideoCapture camera(index);
	while(!camera.isOpened())
	{
		cout << "cannot open " << camera_name << endl;
		sleep(1);
	}

	camera.set(CV_CAP_PROP_FOURCC ,CV_FOURCC('M', 'J', 'P', 'G') );

	Mat frame;
	char check_key = 0;

	while(check_key != 27 && camera.isOpened())
	{
		camera.read(frame);
		namedWindow(camera_name, CV_WINDOW_NORMAL);
		imshow(camera_name, frame);
		check_key  = waitKey(1);
	}
	destroyWindow(camera_name);
	camera.release();
	return true;
}
