#include <opencv2/opencv.hpp>
#include <iostream>
#include <unistd.h>
#include <cmath>

using namespace cv;
using namespace std;

Mat kmeans_clustering(Mat src, int cluster_count, int kmeans_attempts);
double transfer_to_line(Mat src, Mat ori);
int polyfit(const double* const dependentValues,\
            const double* const independentValues, \
            unsigned int        countOfElements, \
            unsigned int        order, \
            double*             coefficients);
double get_curvature(double coefficients[], double &curvature, int point_x);

int turn_determination(Mat crop)
{
	const int cluster_count = 4;
	const int kmeans_attempts = 2;
	
	Scalar blue_low_bound(100, 50, 50), blue_high_bound(135, 220, 220); 

	Mat clustered_img, hsv_img, edge_clst, blue_object, target, knn_hsv_mix, hsv_channels[3], lab_img;
	Mat element3x3 = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(-1, -1));

//	cvtColor(crop, lab_img, CV_BGR2Lab);
	cvtColor(crop, hsv_img, CV_BGR2HSV);
	inRange(hsv_img, blue_low_bound, blue_high_bound, blue_object);

	split(hsv_img, hsv_channels);

	clustered_img = kmeans_clustering(hsv_channels[0], cluster_count, kmeans_attempts);
//	split(clustered_img, hsv_channels);
//	clustered_img = hsv_channels[2];
	Canny(clustered_img, edge_clst, 100, 200, 3);

	bitwise_or(edge_clst, blue_object, knn_hsv_mix);
	morphologyEx(knn_hsv_mix, knn_hsv_mix, MORPH_CLOSE, element3x3, Point(-1, -1), 2);
	erode(knn_hsv_mix, knn_hsv_mix, element3x3, Point(-1, -1), 2);

	transfer_to_line(knn_hsv_mix, crop);	

//	namedWindow("imgOriginal", CV_WINDOW_NORMAL);
//	namedWindow("imgKmeans", CV_WINDOW_NORMAL);
//	namedWindow("imgEdge", CV_WINDOW_NORMAL);
//	namedWindow("lab", CV_WINDOW_NORMAL);
//	imshow("imgOriginal", frame);
//	imshow("imgKmeans", knn_hsv_mix);
//	imshow("imgEdge", edge_clst);
//	imshow("lab", lab_img);
	return 1;
}

double transfer_to_line(Mat src, Mat ori)
{
	const int middle_pt = src.cols / 2;
	double coefficients[3]; //second ordered polynominal curve
	double curvature = 0;
	vector<Point> curve;

	for(int row = src.rows * (1.0/6.0); row < src.rows * (5.0/6.0); row += src.rows * (2.0/3.0) * (1.0/10.0)) //get 10 row from middle 2/3 image 
	{
		const uchar* row_pos = src.ptr<uchar>(row);
		vector<Point> sample_pts;
		double sum = 0;
		for(int col = 0; col < src.cols; col++)
		{
			if(int(row_pos[col]) == 255)
			{
				sum += double(col);
				sample_pts.push_back(Point(col, row));
			}
		}
		
		if(sum < 200) continue;

		double avg_x = sum / double(sample_pts.size());
		if(avg_x > 260) curve.push_back(Point(260, row));
		else if (avg_x < 60) curve.push_back(Point(60, row));
		else curve.push_back(Point(avg_x, row));
 	}

	for(vector<Point>::iterator it = curve.begin(); it != curve.end(); it++)
	{
		circle(src, *it, 2, Scalar(255), 2, CV_AA, 0);
	}
	
//	double *x = new double[sample_pts.size()];
//	double *y = new double[sample_pts.size()];
	
//	for(int i = 0; i < sample_pts.size(); ++i)
//	{
//		x[i] = sample_pts[i].x;
//		y[i] = sample_pts[i].y;
//	}	
	
//	polyfit(x, y, sample_pts.size(), 2, coefficients);
//	double point_x = -0.5 * coefficients[1] / coefficients[2];
	
//	if(point_x >= )

//	cout << "point_x: " << point_x << endl;
//	get_curvature(coefficients, curvature, point_x);
			
//	cout <<"function: "<< coefficients[2] << "x^2 + (" << coefficients[1] << "x) + " << coefficients[0] << endl;

//	cout << "curvature: " << curvature << endl; 
//	cout << "radius: " << 1.0 / curvature << endl;
	
	namedWindow("img_sample", CV_WINDOW_NORMAL);
	namedWindow("img_ori", CV_WINDOW_NORMAL);
	imshow("img_sample", src);
	imshow("img_ori", ori);
	
	return 1; 
}

double get_curvature(double coefficients[], double &curvature, int point_x)
{
	double second_deri_coef = 2.0 * coefficients[2];
	double first_deri = second_deri_coef * point_x + coefficients[1];
	curvature = second_deri_coef / pow(1 + pow(first_deri, 2.0), 3.0/2.0);

	return curvature;
}

Mat kmeans_clustering(Mat src, int cluster_count, int kmeans_attempts)
{
	Mat labels, centers;
	Mat samples(src.rows * src.cols, 1, CV_32F);
	Mat clustered_img(src.size(), src.type());
	float* samples_ptr = samples.ptr<float>(0);

    	for( int row = 0; row != src.rows; ++row){
        	uchar* hsv_img_begin = src.ptr<uchar>(row);
        	uchar* hsv_img_end = hsv_img_begin + src.cols * src.channels();
        	while(hsv_img_begin != hsv_img_end){
           	samples_ptr[0] = hsv_img_begin[0];
//            	samples_ptr[1] = hsv_img_begin[1];
  //          	samples_ptr[2] = hsv_img_begin[2];
       		samples_ptr += 1;
		hsv_img_begin += 1;
        	}        
    	}
  
	kmeans(samples, cluster_count, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 5, 0.05), kmeans_attempts, KMEANS_PP_CENTERS, centers);

	for( int row = 0; row < src.rows; ++row){
        	uchar* clustered_img_begin = clustered_img.ptr<uchar>(row);
        	uchar* clustered_img_end = clustered_img_begin + clustered_img.cols * 1;
        	int* labels_ptr = labels.ptr<int>(row * src.cols);
		
        	while(clustered_img_begin != clustered_img_end){
           		float* centers_ptr = centers.ptr<float>(* labels_ptr);
            		clustered_img_begin[0] = centers_ptr[0];
//            		clustered_img_begin[1] = centers_ptr[1];
  //          		clustered_img_begin[2] = centers_ptr[2];
            		clustered_img_begin += 1;
			++labels_ptr;
        	}
    	}
	return clustered_img;
}

int polyfit(const double* const dependentValues, \
            const double* const independentValues, \
            unsigned int        countOfElements, \
            unsigned int        order, \
            double*             coefficients)
{
    enum {maxOrder = 5};
    
    double B[maxOrder+1] = {0.0f};
    double P[((maxOrder+1) * 2)+1] = {0.0f};
    double A[(maxOrder + 1)*2*(maxOrder + 1)] = {0.0f};

    double x, y, powx;

    unsigned int ii, jj, kk;

    // Verify initial conditions....

    if (countOfElements <= order)
        return -1;

    // This method has imposed an arbitrary bound of
    // order <= maxOrder.  Increase maxOrder if necessary.
    if (order > maxOrder)
        return -1;

    // Begin Code...
    // ----------------------------------

    // Identify the column vector
    for (ii = 0; ii < countOfElements; ii++)
    {
        x    = dependentValues[ii];
        y    = independentValues[ii];
        powx = 1;

        for (jj = 0; jj < (order + 1); jj++)
        {
            B[jj] = B[jj] + (y * powx);
            powx  = powx * x;
        }
    }

    // Initialize the PowX array
    P[0] = countOfElements;

    // Compute the sum of the Powers of X
    for (ii = 0; ii < countOfElements; ii++)
    {
        x    = dependentValues[ii];
        powx = dependentValues[ii];

        for (jj = 1; jj < ((2 * (order + 1)) + 1); jj++)
        {
            P[jj] = P[jj] + powx;
            powx  = powx * x;
        }
    }

    // Initialize the reduction matrix
    //
    for (ii = 0; ii < (order + 1); ii++)
    {
        for (jj = 0; jj < (order + 1); jj++)
        {
            A[(ii * (2 * (order + 1))) + jj] = P[ii+jj];
        }

        A[(ii*(2 * (order + 1))) + (ii + (order + 1))] = 1;
    }

    // Move the Identity matrix portion of the redux matrix
    // to the left side (find the inverse of the left side
    // of the redux matrix
    for (ii = 0; ii < (order + 1); ii++)
    {
        x = A[(ii * (2 * (order + 1))) + ii];
        if (x != 0)
        {
            for (kk = 0; kk < (2 * (order + 1)); kk++)
            {
                A[(ii * (2 * (order + 1))) + kk] = 
                    A[(ii * (2 * (order + 1))) + kk] / x;
            }

            for (jj = 0; jj < (order + 1); jj++)
            {
                if ((jj - ii) != 0)
                {
                    y = A[(jj * (2 * (order + 1))) + ii];
                    for (kk = 0; kk < (2 * (order + 1)); kk++)
                    {
                        A[(jj * (2 * (order + 1))) + kk] = 
                            A[(jj * (2 * (order + 1))) + kk] -
                            y * A[(ii * (2 * (order + 1))) + kk];
                    }
                }
            }
        }
        else
        {
            // Cannot work with singular matrices
            return -1;
        }
    }

    // Calculate and Identify the coefficients
    for (ii = 0; ii < (order + 1); ii++)
    {
        for (jj = 0; jj < (order + 1); jj++)
        {
            x = 0;
            for (kk = 0; kk < (order + 1); kk++)
            {
                x = x + (A[(ii * (2 * (order + 1))) + (kk + (order + 1))] *
                    B[kk]);
            }
            coefficients[ii] = x;
        }
    }

    return 0;
}
