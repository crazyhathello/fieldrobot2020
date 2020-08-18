#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/cudacodec.hpp>
#include <unistd.h>
#include <string>
#include <stdlib.h>

using namespace std;
using namespace cv;

int main()
{
        VideoCapture camera(1);
        while (!camera.isOpened())
        {
                cout << "cannot open camera";
                sleep(3);
        }
	
	int b_width = 19, b_height = 3;
	Size board_size = Size(b_width, b_height);

        int frame_width = 320, frame_height = 240;
    //    int roi_width = 320, roi_height = frame_height / 2;
        char checkForEscKey = 0;
	vector<Point2f> corners;
        Mat frame, pt, gray;
	bool found_board = false;
	char charCheckForEscKey;
	Mat warped_image;

        camera.set(CAP_PROP_FRAME_WIDTH, frame_width);
        camera.set(CAP_PROP_FRAME_HEIGHT, frame_height);
  //      camera.set(CAP_PROP_FPS, 30);
/*
        Rect roi;
        roi.x = 0;
        roi.y = roi_height / 4;
        roi.width = roi_width;
        roi.height = roi_height;
*/
	camera.read(frame);
	//cvtColor(frame, frame, CV_BGR2GRAY);
	found_board = findChessboardCorners(frame, board_size, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
		
	cout << "fb: " << found_board << endl;
	if(found_board)
	{
		//cornerSubPix(frame, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
	//	drawChessboardCorners(frame, board_size, corners, found_board);
		vector<Point2f> srcPoints = {Point2f(corners[0].x, corners[0].y), Point2f(corners[18].x, corners[18].y), Point2f(corners[38].x, corners[38].y), Point2f(corners[56].x, corners[56].y)};
		circle(frame, Point(int(corners[0].x), int(corners[0].y)), 12, (255,0,0), 2, CV_AA);
		circle(frame, Point(int(corners[18].x), int(corners[18].y)), 10, (0, 255, 0), 2, CV_AA);
		circle(frame, Point(int(corners[56].x), int(corners[56].y)), 6, (0, 255, 0), 2, CV_AA);
		circle(frame, Point(int(corners[38].x), int(corners[38].y)), 8, (0, 255, 0), 2, CV_AA);
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

		cout <<Point2f(corners[38].x, 120 + (corners[38].y-(corners[56].x - corners[38].x)/9)) <<endl;
		cout <<Point2f(corners[56].x, 120 + (corners[56].y-(corners[56].x - corners[38].x)/9)) << endl;
		cout << Point2f(corners[38].x, 120 + corners[38].y) << endl;
		cout << Point2f(corners[56].x, 120 + corners[56].y) << endl;
		Mat homography = findHomography(srcPoints, dstPoints, CV_RANSAC, 3);
		warpPerspective(frame, warped_image, homography, Size(1.5*frame.cols, 2*frame.rows));
	}else
	{
		cout << "chessboard not found!!" << endl;
		return 0;
	}
	
		
//	namedWindow("imgCB", CV_WINDOW_NORMAL);
	namedWindow("imgWarp", CV_WINDOW_NORMAL);
	imwrite("imgCB2.jpg", frame);
	imwrite("imgWarp.jpg", warped_image);
	imshow("imgWarp", warped_image);

	charCheckForEscKey = waitKey(5000);
	
	return 0;
}





