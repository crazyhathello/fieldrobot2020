#include <opencv2/opencv.hpp>
#include <iostream>
#include <unistd.h>
#include <cmath>

using namespace cv;
using namespace std;

Mat kmeans_clustering(Mat src, int cluster_count, int kmeans_attempts);
double curve_fitting(Mat src);
void get_curve(double* x, double* y, int countOfElements, int order, double* coefficients);
void get_curvature(double coefficients[], double &curvature, double point, char& direction, Mat src);

int turn_determination(Mat crop)
{
	const int cluster_count = 4;
	const int kmeans_attempts = 2;
	
//	Scalar blue_low_bound(100, 50, 50), blue_high_bound(135, 220, 220); 

	Mat clustered_img, hsv_img, edge_clst, blue_object, target, knn_hsv_mix, hsv_channels[3], lab_img;
	Mat element_close = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
	Mat element3x3_cir = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(-1, -1));

//	cvtColor(crop, lab_img, CV_BGR2Lab);
	cvtColor(crop, hsv_img, CV_BGR2HSV);
//	inRange(hsv_img, blue_low_bound, blue_high_bound, blue_object);

	split(hsv_img, hsv_channels);

	GaussianBlur(hsv_channels[0], hsv_channels[0], Size(5, 5), 0, 0);
	clustered_img = kmeans_clustering(hsv_channels[0], cluster_count, kmeans_attempts);
//	split(clustered_img, hsv_channels);
//	clustered_img = hsv_channels[2];
	morphologyEx(clustered_img, clustered_img, MORPH_OPEN, element3x3_cir, Point(-1, -1), 1);
	Canny(clustered_img, edge_clst, 50, 200, 3, true);

//	bitwise_or(edge_clst, blue_object, knn_hsv_mix);
	morphologyEx(edge_clst, edge_clst, MORPH_CLOSE, element_close, Point(-1, -1), 1);
//	erode(edge_clst, edge_clst, element3x3, Point(-1, -1), 1);

//	fastNlMeansDenoising(edge_clst, edge_clst, 3, 7, 21);

//	namedWindow("imgOriginal", CV_WINDOW_NORMAL);
	namedWindow("imgKmeans", CV_WINDOW_NORMAL);
	namedWindow("imgEdge", CV_WINDOW_NORMAL);
//	namedWindow("lab", CV_WINDOW_NORMAL);
//	imshow("imgOriginal", frame);
	imshow("imgKmeans", clustered_img);
	imshow("imgEdge", edge_clst);
//	imshow("lab", lab_img);

	

	return curve_fitting(edge_clst);	
}

double curve_fitting(Mat src)
{
	const int middle_pt = src.cols / 2;
	vector<Point> sample_pts;
	double curvature = 0;
	char direction;
	double radius = 0;
	int turn_value = 0;
	int index = 0;
	int sample_num = 10;
	double step = double(src.rows) * (2.0/3.0) * (1.0/double(sample_num));
	double point_x = 0, x1 = 0, x2 = 0;
	double x[sample_num];
	double y[sample_num];

	for(double row = src.rows * (1.0/6.0); row < src.rows * (5.0/6.0); row += step) //get 5 row from  image 
	{
		const uchar* row_pos = src.ptr<uchar>(int(row));
		double sum = 0;
		double count = 0;

		for(int col = 0; col < src.cols; col++)
		{
			if(int(row_pos[col]) == 255)
			{
				sum += col;
				count += 1.0;
				//sample_pts.push_back(Point(col, row));
			}
		}

		if(count != 0)
		{
			x[index] = sum / count;
			y[index] = src.rows - row;
			index++;
		}
//		cout << "x " << x[index] << " y " << y[index] << endl;
 	}

	
//	for(int i = 0; i < sample_pts.size(); ++i)
//	{
//		
//		x[i] = sample_pts[i].x;
//		y[i] = sample_pts[i].y;
//	}	
	
	double* coefficients = new double[3];
	get_curve(x, y, sample_num, 2, coefficients); //second ordered polynominal curve
//	double point_x = -0.5 * coefficients[1] / coefficients[2];
	
//	if(point_x >= )

	double judgement = pow(coefficients[1], 2) - 4.0f * coefficients[2] * (coefficients[0]);
	if(judgement < 0) return 0;
	else
	{
		x1 = (-coefficients[1] + sqrt(judgement)) / (2 * coefficients[2]);
		x2 = (-coefficients[1] - sqrt(judgement)) / (2 * coefficients[2]);
		if(x1 <= src.cols && x1 >=0  && (x2 < 0 || x2 > src.cols))
		{
			point_x = x1;
		}else if(x2 <= src.cols && x2 >=0  && (x1 < 0 || x1 > src.cols))
		{
			point_x = x2;
		}else
		{
			point_x = 0;
		}
	}

//	point_x = sample_pts[sample_pts.size() - 1].x;
//	cout << "point_x: " <<  point_x <<endl;

	double second_deri_coef =  2.0 * coefficients[2];
	double first_deri = second_deri_coef * point_x + coefficients[1];
	curvature = abs(second_deri_coef / pow(1.0 + pow(first_deri, 2.0), 3.0/2.0));

	if(first_deri > 0) direction ='r';
	else direction = 'l';

	radius = 1.0 / curvature;

//	cout << "real function: y = " << coefficients[2] << "x^2 + (" << coefficients[1] << "x) + " << coefficients[0] << endl;
//	cout << "x1: " <<  x1 << " x2: " << x2 << " point_x: "<< point_x << endl;
//	cout << "curvature: " << curvature << endl; 
	cout << "direction: " << direction << endl;
	cout << "radius: " << radius << endl;
	
//	namedWindow("img_sample", CV_WINDOW_NORMAL);
//	namedWindow("img_ori", CV_WINDOW_NORMAL);
//	imshow("img_sample", src);
//	imshow("img_ori", ori);

	if(radius > 600)
	{
		if(radius > 1200) radius = 1200;
		turn_value = 10.0 - (radius - 600.0) / 60.0;
	}else if(radius <=  600 && radius > 300) turn_value = 30.0 - ((radius - 300.0) / 15.0);
	else if(radius <= 300) turn_value = 50.0 - (radius / 15.0) ;

	if(direction == 'r') return turn_value;
	else
	{
		turn_value = -1 * turn_value;
		return turn_value;
	} 

}

//	double x1 = (coefficients[1] + sqrt(pow(-coefficients[1], 2) - 4.0f * -coefficients[2] * (-coefficients[0] + point))) / (2 * -coefficients[2]);
//	double x2 = (coefficients[1] - sqrt(pow(-coefficients[1], 2) - 4.0f * -coefficients[2] * (-coefficients[0] + point))) / (2 * -coefficients[2]);

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
  
	kmeans(samples, cluster_count, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 5, 0.1), kmeans_attempts, KMEANS_PP_CENTERS, centers);

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

void get_curve(double x[], double y[], int num_of_points, int order, double* coefficients)
{
	int N = num_of_points, n = order;//N = number of points, n = order
	int i, j, k;

	double X[2*n+1];  
                      //Array that will store the values of sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
    	for (i=0;i<2*n+1;i++)
    	{
        	X[i]=0;
        	for (j=0;j<N;j++)
            		X[i]=X[i]+pow(x[j],i);        //consecutive positions of the array will store N,sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
    	}
    	
	double B[n+1][n+2], a[n+1];            //B is the Normal matrix(augmented) that will store the equations, 'a' is for value of the final coefficients
    	for (i=0;i<=n;i++)
        {	
		for (j=0;j<=n;j++)
            		B[i][j]=X[i+j];            //Build the Normal matrix by storing the corresponding coefficients at the right positions except the last column of the matrix
	}
    		
	double Y[n+1];                    //Array to store the values of sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
    	for (i=0;i<n+1;i++)
    	{    
        	Y[i]=0;
        	for (j=0;j<N;j++)
        		Y[i]=Y[i]+pow(x[j],i)*y[j];        //consecutive positions will store sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
    	}
    
	for (i=0;i<=n;i++)
        	B[i][n+1]=Y[i];                //load the values of Y as the last column of B(Normal Matrix but augmented)
    	
	n=n+1;                //n is made n+1 because the Gaussian Elimination part below was for n equations, but here n is the degree of polynomial and for n degree we get n+1 equations
    	for (i=0;i<n;i++)                    //From now Gaussian Elimination starts(can be ignored) to solve the set of linear equations (Pivotisation)
        	for (k=i+1;k<n;k++)
            		if (B[i][i]<B[k][i])
                		for (j=0;j<=n;j++)
                		{
                    			double temp=B[i][j];
                    			B[i][j]=B[k][j];
                    			B[k][j]=temp;
                		}
    
    	for (i=0;i<n-1;i++)            //loop to perform the gauss elimination
        	for (k=i+1;k<n;k++)
            	{
                	double t=B[k][i]/B[i][i];
                	for (j=0;j<=n;j++)
                	    	B[k][j]=B[k][j]-t*B[i][j];    //make the elements below the pivot elements equal to zero or elimnate the variables
            	}

	for (i=n-1;i>=0;i--)                //back-substitution	
	{                        //x is an array whose values correspond to the values of x,y,z..
        	coefficients[i]=B[i][n];                //make the variable to be calculated equal to the rhs of the last equation
        	for (j=0;j<n;j++)
            		if (j!=i)            //then subtract all the lhs values except the coefficient of the variable whose value is being calculated
                		coefficients[i]=coefficients[i]-B[i][j]*coefficients[j];
        	coefficients[i]=coefficients[i]/B[i][i];            //now finally divide the rhs by the coefficient of the variable to be calculated
    	}

//	cout<<"the fitted Polynomial is given by:\ny=";
  //  	for (i=0;i<n;i++)
    //    	cout<<" + ("<<a[i]<<")"<<"x^"<<i;
    //	cout<<"\n";
}

