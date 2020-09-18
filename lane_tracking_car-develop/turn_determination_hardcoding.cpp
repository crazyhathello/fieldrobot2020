#include <opencv2/opencv.hpp>
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <vector>

using namespace cv;
using namespace std;

Mat kmeans_clustering(Mat src, int cluster_count, int kmeans_attempts);
double get_slope(vector<double> &x, vector<double> &y);

int turn_determination(Mat src)
{
	const int cluster_count = 3;
	const int kmeans_attempts = 2;
	Mat blue_object, YCbCr_img, hsv_cs[3], blue_object2;
	int sample_num = 50;
	double step = double(src.rows) * (1.0/double(sample_num));
	Scalar blue_low_bound(50, 100, 135), blue_high_bound(255, 125, 180); 
	vector<double> x, y;
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5), Point(-1, -1));
	vector<Vec4i> lines;

	cvtColor(src, YCbCr_img, CV_BGR2YCrCb);
	split(YCbCr_img, hsv_cs);

//	inRange(hsv_cs[0], Scalar(100), Scalar(130), blue_object2);
	inRange(YCbCr_img, blue_low_bound, blue_high_bound, blue_object);

	morphologyEx(blue_object, blue_object, MORPH_CLOSE, element, Point(-1, -1), 1);

	HoughLinesP(blue_object, lines, 0.7, CV_PI/90, 40, 100, 20);

	for( size_t i = 0; i < lines.size(); i++ ){
                 Vec4i l = lines[i];
                 line(src, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 1, CV_AA);
        }


	const int mid = 40;
	int sum = 0;
	int turn_value = 0;
	for(size_t i = 0; i < lines.size(); i++ ){
		Vec4i l = lines[i];
		sum += (l[0] + l[2]) /2.0;
	}

	cout << "dist: " <<  sum / lines.size() <<  endl; //range from 200 to 270 mid is 230

	if(lines.size() == 0) turn_value = 0;
	else turn_value = ((sum / int(lines.size())) - mid) * 1.8;

	if(turn_value > 50 )turn_value = 50;
	else if (turn_value < -50) turn_value = -50;
//	cout << "slope: " << get_slope(x, y) << endl;

//	namedWindow("imgOriginal", CV_WINDOW_NORMAL);
//	namedWindow("imgKmeans", CV_WINDOW_NORMAL);
	namedWindow("imgEdge", CV_WINDOW_NORMAL);
//	namedWindow("blue", CV_WINDOW_NORMAL);
//	imshow("imgOriginal", src);
//	imshow("imgKmeans", crop);
	imshow("imgEdge", blue_object);
//	imshow("blue", blue_object2);
	return turn_value;

}

double get_slope(vector<double> &x, vector<double> &y)
{
	if(x.size() != y.size())
	{
        	throw "x and y size is not equal";
	}

	const double n = x.size();
    	const double s_x  = accumulate(x.begin(), x.end(), 0.0);
	const double s_y  = accumulate(y.begin(), y.end(), 0.0);
	const double s_xx = inner_product(x.begin(), x.end(), x.begin(), 0.0);
	const double s_xy = inner_product(x.begin(), x.end(), y.begin(), 0.0);
	const double slope = (n * s_xy - s_x * s_y) / (n * s_xx - s_x * s_x);

	return slope;	
}
