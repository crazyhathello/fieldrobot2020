#include <iostream>
#include "turn_determination_hardcoding.h"
#include "send_to_arduino.h"
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <string>
#include <stdlib.h>

using namespace std;
using namespace cv;

void lane_tracking(VideoCapture &camera, int frame_width, int frame_height)
{
//	VideoCapture camera(camera_index);
//	VideoCapture camera("roaddtest_1009.avi");

	while (!camera.isOpened())
	{
		cout << "cannot open front camera";
		sleep(3);
	}

//	int frame_width = 320, frame_height = 240;
	char checkForEscKey = 0;
	char stop[] = "stop";
	Mat frame, crop, warped_image;

//	camera.set(CAP_PROP_FRAME_WIDTH, frame_width);
//	camera.set(CAP_PROP_FRAME_HEIGHT, frame_height);
//	camera.set(CAP_PROP_EXPOSURE, false);
//	camera.set(CV_CAP_PROP_BUFFERSIZE, 2);
//	camera.set(CAP_PROP_FPS, 30);

	Rect roi;
	roi.x = frame_width / 2;
	roi.y = 0;
	roi.width = frame_width / 2;
	roi.height = frame_height;

	if (checkForEscKey != 27 && camera.isOpened())
	{
		camera.read(frame);

//		namedWindow("ori", CV_WINDOW_NORMAL);
//		imshow("ori", frame);
		frame = frame(roi);

//		warped_image = warped_image(roi);
//		namedWindow("after_warp", CV_WINDOW_NORMAL);
//		imshow("after_warp", warped_image);
	//	int turn_value = turn_determination(crop);
		string turn_value = "move" + to_string(turn_determination(frame));
//		cout << "turn value: "<< turn_value.c_str() << endl;
		/*if(turn_value == -1)
		{
			send_to_arduino(l);
		}else if(turn_value == 1)
		{
			send_to_arduino(r);
		}else if(turn_value == 0)
		{
			send_to_arduino(f);
		}else{

			send_to_arduino(stop);
		}
*/

		send_to_arduino(turn_value.c_str());
		checkForEscKey  = waitKey(1);
	}

//	send_to_arduino(stop);

	return;
}
