#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <stdlib.h>
#include "lane_tracking.h"

using namespace std;
using namespace cv;

int main()
{
	int front_camera_index = 1;
	VideoCapture front_camera(front_camera_index);

	while (!front_camera.isOpened())
	{
		cout << "cannot open camera";
		sleep(3);
	}
	lane_tracking(front_camera);
	return 0;
}
