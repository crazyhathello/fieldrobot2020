#include <iostream>
#include "pt_turn_determination.h"
#include "send_to_arduino.h"
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <string>
#include <stdlib.h>

using namespace std;
using namespace cv;

Mat getHomography(Mat frame)
{
	vector<Point2f> corners;
	int b_width = 19, b_height = 3;
	Size board_size = Size(b_width, b_height);
	bool found_board = false;
	Mat warped_image;

	found_board = findChessboardCorners(frame, board_size, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

	cout << "fb: " << found_board << endl;
	if(found_board)
	{
		//cornerSubPix(frame, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
	//	drawChessboardCorners(frame, board_size, corners, found_board);
		vector<Point2f> srcPoints = {Point2f(corners[0].x, corners[0].y), Point2f(corners[18].x, corners[18].y), Point2f(corners[38].x, corners[38].y), Point2f(corners[56].x, corners[56].y)};
//		circle(frame, Point(int(corners[0].x), int(corners[0].y)), 12, (255,0,0), 2, CV_AA);
//		circle(frame, Point(int(corners[18].x), int(corners[18].y)), 10, (0, 255, 0), 2, CV_AA);
//		circle(frame, Point(int(corners[56].x), int(corners[56].y)), 6, (0, 255, 0), 2, CV_AA);
//		circle(frame, Point(int(corners[38].x), int(corners[38].y)), 8, (0, 255, 0), 2, CV_AA);
		vector<Point2f> dstPoints;

		if(corners[38].x < corners[56].x)
		{
			dstPoints = {	Point2f(frame.cols/4 + corners[38].x, frame.rows + (corners[38].y-(corners[56].x - corners[38].x)/9)), \
					Point2f(frame.cols/4 + corners[56].x, frame.rows + (corners[56].y-(corners[56].x - corners[38].x)/9)), \
					Point2f(frame.cols/4 + corners[38].x, frame.rows + corners[38].y), \
					Point2f(frame.cols/4 + corners[56].x, frame.rows + corners[56].y)};

		}else
		{
			dstPoints = {	Point2f(frame.cols/4 + corners[0].x, frame.rows + corners[0].y), \
					Point2f(frame.cols/4 + corners[18].x, frame.rows + corners[18].y), \
					Point2f( frame.cols/4 + corners[0].x, frame.rows + (corners[0].y-(corners[0].x - corners[18].x)/9)), \
					Point2f( frame.cols/4 + corners[18].x, frame.rows + (corners[18].y-(corners[0].x - corners[18].x)/9))};
		}

//		cout << corners[38]<< endl;
//		cout << corners[56]<< endl;

//		cout <<Point2f(corners[38].x, 120 + (corners[38].y-(corners[56].x - corners[38].x)/9)) <<endl;
//		cout <<Point2f(corners[56].x, 120 + (corners[56].y-(corners[56].x - corners[38].x)/9)) << endl;
//		cout << Point2f(corners[38].x, 120 + corners[38].y) << endl;
//		cout << Point2f(corners[56].x, 120 + corners[56].y) << endl;
		Mat homography = findHomography(srcPoints, dstPoints, CV_RANSAC, 3);

		string filename = "homography.xml";
		FileStorage fs(filename, FileStorage::WRITE);
		fs.open(filename, FileStorage::WRITE);
		fs << "homography" << homography;
		fs.release();
		cout << "found chessboard, and write it to file." << endl;

		return homography;
	}else
	{
		Mat homography;
        	FileStorage fs;
        	fs.open("homography.xml", FileStorage::READ);

		if (!fs.isOpened())
        	{
            		cerr << "Failed to open homography.xml" << endl;
			exit (EXIT_FAILURE);
        	}
		
		fs["homography"] >> homography;

		cout << "chessboard not found!! Read from file!" << endl;
		return homography;
	}
}

int main()
{
	VideoCapture camera(1);
	while (!camera.isOpened())
	{
		cout << "cannot open camera";
		sleep(3);
	}

	int frame_width = 320, frame_height = 240;
	char checkForEscKey = 0;
	char stop[] = "stop";
	Mat frame, crop, warped_image;

	camera.set(CAP_PROP_FRAME_WIDTH, frame_width);
	camera.set(CAP_PROP_FRAME_HEIGHT, frame_height);
	camera.set(CAP_PROP_EXPOSURE, false);
	camera.set(CV_CAP_PROP_BUFFERSIZE, 5);
	camera.set(CAP_PROP_FPS, 30);

	camera.read(frame);
	Mat homography = getHomography(frame);

	int pt_w = 1.5 * frame.cols;
	int pt_h = 2 * frame.rows;

	Rect roi;
	roi.x = (pt_w - frame.cols) / 2;
	roi.y = 0.5 * frame.rows;
	roi.width = frame.cols;
	roi.height = 1.5 * frame.rows;

	while (checkForEscKey != 27 && camera.isOpened())
	{
		camera.read(frame);

//		namedWindow("ori", CV_WINDOW_NORMAL);
//		imshow("ori", frame);

		warpPerspective(frame, warped_image, homography, Size(pt_w, pt_h));
//		namedWindow("before_warp", CV_WINDOW_NORMAL);
//		imshow("before_warp", warped_image);

		warped_image = warped_image(roi);
//		namedWindow("after_warp", CV_WINDOW_NORMAL);
//		imshow("after_warp", warped_image);
	//	int turn_value = turn_determination(crop);
		string turn_value = "move" + to_string(turn_determination(warped_image));
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

	send_to_arduino(stop);

	return 0;
}
