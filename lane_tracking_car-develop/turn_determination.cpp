#include <opencv2/opencv.hpp>
#include <iostream>
#include <unistd.h>

using namespace cv;
using namespace std;

Mat kmeans_clustering(Mat src, int cluster_count, int kmeans_attempts);

int turn_determination(Mat crop)
{
	const int cluster_count = 3;
	const int kmeans_attempts = 2;
	
	Scalar blue_low_bound(100,20, 20), blue_high_bound(135,255, 255); 

	Mat clustered_img, hsv_img, edge_clst, blue_object, target, knn_hsv_mix, hsv_channels[3];
	Mat element3x3 = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(-1, -1));
	vector<Vec4i> lines;

	cvtColor(crop, hsv_img, CV_BGR2HSV);
	inRange(hsv_img, blue_low_bound, blue_high_bound, blue_object);

	clustered_img = kmeans_clustering(hsv_img, cluster_count, kmeans_attempts);
	split(clustered_img, hsv_channels);
	clustered_img = hsv_channels[2];
	Canny(clustered_img, edge_clst, 100, 200, 3);

	bitwise_or(edge_clst, blue_object, knn_hsv_mix);
	morphologyEx(knn_hsv_mix, knn_hsv_mix, MORPH_CLOSE, element3x3, Point(-1, -1), 2);
	erode(knn_hsv_mix, knn_hsv_mix, element3x3, Point(-1, -1), 2);

	HoughLinesP(knn_hsv_mix, lines, 0.7, CV_PI/90, 40, 30, 20);
/*
		for( size_t i = 0; i < lines.size(); i++ ){
    		Vec4i l = lines[i];
			line(crop, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 1, CV_AA);
  		}
*/		
	Vec4i right_line(crop.cols / 2, 0, crop.cols / 2, 0);
	Vec4i left_line(crop.cols / 2, 0, crop.cols / 2, 0);

 	for( size_t i = 0; i < lines.size(); i++ ){
    		Vec4i l = lines[i];
		if(l[0] <= left_line[0] || l[2] <= left_line[2]){
			left_line = l;
		}

		if(l[0] >= right_line[0] || l[2] >= right_line[2]){
			right_line = l;
		}
  	}

	line(crop, Point(left_line[0], left_line[1]), Point(left_line[2], left_line[3]), Scalar(0,0,255), 1, CV_AA);
	line(crop, Point(right_line[0], right_line[1]), Point(right_line[2], right_line[3]), Scalar(0,255,0), 1, CV_AA);		

	double r_slope = (double)(right_line[1]-right_line[3]) / (double)(right_line[2]-right_line[0]);
	double l_slope = (double)(left_line[1]-left_line[3]) / (double)(left_line[2]-left_line[0]);		
//	cout << "r: " << r_slope << endl;
//	cout << "l: " << l_slope << endl;
	if(abs(r_slope) >= 5) r_slope = 5;
	if(abs(l_slope) <= -5) l_slope = -5;
	double total_slope = r_slope + l_slope;		
//	cout << "t: " << total_slope << endl;

	//namedWindow("imgOriginal", CV_WINDOW_NORMAL);
//	namedWindow("imgKmeans", CV_WINDOW_NORMAL);
//	namedWindow("imgEdge", CV_WINDOW_NORMAL);
	namedWindow("blue", CV_WINDOW_NORMAL);
	//imshow("imgOriginal", frame);
//	imshow("imgKmeans", crop);
//	imshow("imgEdge", edge_clst);
	imshow("blue", knn_hsv_mix);

	if(isnan(r_slope) && isnan(l_slope))
	{
		cout <<" forward slow" << endl;
		return 0;
	}else if(isnan(l_slope) || total_slope < -0.8 )
	{
		cout << " turn left" << total_slope << endl;
		if(total_slope <= -5) total_slope = -5;
		return total_slope * 10;
	//	return -1;
	}else if(isnan(r_slope) || total_slope > 0.8)
	{
		cout << " turn right" << total_slope << endl;
		if(total_slope >= 5) total_slope = 5;
		return total_slope * 10;
	//	return 1;
	}else
	{
		cout << "forward fast: " << total_slope  << endl;
		return total_slope * 15;
	//	return 0;
	}
}

Mat kmeans_clustering(Mat src, int cluster_count, int kmeans_attempts)
{
	Mat labels, centers;
	Mat samples(src.rows * src.cols, 3, CV_32F);
	Mat clustered_img(src.size(), src.type());
	float* samples_ptr = samples.ptr<float>(0);

    	for( int row = 0; row != src.rows; ++row){
        	uchar* hsv_img_begin = src.ptr<uchar>(row);
        	uchar* hsv_img_end = hsv_img_begin + src.cols * src.channels();
        	while(hsv_img_begin != hsv_img_end){
           	samples_ptr[0] = hsv_img_begin[0];
            	samples_ptr[1] = hsv_img_begin[1];
            	samples_ptr[2] = hsv_img_begin[2];
       		samples_ptr += 3;
		hsv_img_begin += 3;
        	}        
    	}
  
	kmeans(samples, cluster_count, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 5, 0.05), kmeans_attempts, KMEANS_PP_CENTERS, centers);

	for( int row = 0; row < src.rows; ++row){
        	uchar* clustered_img_begin = clustered_img.ptr<uchar>(row);
        	uchar* clustered_img_end = clustered_img_begin + clustered_img.cols * 3;
        	int* labels_ptr = labels.ptr<int>(row * src.cols);
		
        	while(clustered_img_begin != clustered_img_end){
           		float* centers_ptr = centers.ptr<float>(* labels_ptr);
            		clustered_img_begin[0] = centers_ptr[0];
            		clustered_img_begin[1] = centers_ptr[1];
            		clustered_img_begin[2] = centers_ptr[2];
            		clustered_img_begin += 3;
			++labels_ptr;
        	}
    	}
	return clustered_img;
}
