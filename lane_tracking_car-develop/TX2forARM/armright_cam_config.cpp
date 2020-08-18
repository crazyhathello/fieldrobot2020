#include"armright_cam_config.h"
#include <unistd.h>

extern cv::VideoWriter video_filtering;
extern cv::VideoWriter video;


int Camera::init_cam(int w , int h) {
	mWidth = w;
	mHeight = h;
	mCap.set(CV_CAP_PROP_FRAME_WIDTH, w);
	mCap.set(CV_CAP_PROP_FRAME_HEIGHT, h);
	cam_expos_cali(mCap, mExpose);
	//mCap.set(CV_CAP_PROP_FPS, 10);
//	while(!mCap.isOpened()) {
//		std::cout<<"open camera"<<std::endl;
//		sleep(1);
//	}
	return 0;
}



void Camera::set_parameter(CameraParameters parameters){

	switch (parameters)
	{

	case APPLE:
		mLowBottomHue_test = cv::Scalar(0, 100, 100);
		mLowTopHue_test = cv::Scalar(20, 255, 200);    // Hue 20 ~ 30
		mHighBottomHue_test = cv::Scalar(160, 100, 100);
		mHighTopHue_test = cv::Scalar(179, 255, 200);

		mArea_thres = 6500;
		mH_w_high_thres = 1.1;
		mH_w_low_thres = 0.9;

		mDil_iter = 1;
		mEro_iter = 3;

		mDil_ksize = cv::Size(13, 13);
		mEro_ksize = cv::Size(7, 7);

		mCali_num = 0;
		mCali_ksize = 3;
		break;

	case PLATE:
		mLowBottomHue_test = mHighBottomHue_test = cv::Scalar(0, 80, 80);   //Hue 95~115
		mLowTopHue_test = mHighTopHue_test = cv::Scalar(20, 255, 255);

		mArea_thres = 6500;
		mH_w_high_thres = 1.1;
		mH_w_low_thres = 0.9;

		mDil_iter = 1;
		mEro_iter = 1;

		mDil_ksize = cv::Size(13, 13);
		mEro_ksize = cv::Size(7, 7);

		mCali_num = 0;
		mCali_ksize = 3;

		break;
	case POT:
		mLowBottomHue_test = mHighBottomHue_test = cv::Scalar(0,130,112);   //Hue white pot 30~40(39~47)(37~45)  red pot 20~30(18~26)(20~25)
		mLowTopHue_test = mHighTopHue_test = cv::Scalar(30, 229, 255);     //Sat white pot 27~32(28~38)(25~35)  red pot 50~80(75~90)(55~80)   select 130~150   
																		  //Val white pot 58~100(38~70)(60~100) red pot 60~100(44~85)(55~100)
		
		mArea_thres = 8000;
		mH_w_high_thres = 1.1;
		mH_w_low_thres = 0.9;

		mDil_iter = 1;
		mEro_iter = 1;

		mDil_ksize = cv::Size(13, 13);
		mEro_ksize = cv::Size(7, 7);

		mCali_num = 0;
		mCali_ksize = 3;
		break;
	case REDSIGN:
		mLowBottomHue_test = cv::Scalar(0, 80, 80);
		mLowTopHue_test = cv::Scalar(10, 255, 255);    // Hue 10 ~ 20
		mHighBottomHue_test = cv::Scalar(160, 80, 80);
		mHighTopHue_test = cv::Scalar(179, 255, 255);

		mArea_thres = 40000;
		mH_w_high_thres = 1.1;
		mH_w_low_thres = 0.9;

		mDil_iter = 0;
		mEro_iter = 2;

		mDil_ksize = cv::Size(13, 13);
		mEro_ksize = cv::Size(13, 13);

		mCali_num = 0;
		mCali_ksize = 3;
		break;

	case BLUESIGN:
		mLowBottomHue_test = mHighBottomHue_test = cv::Scalar(90, 80, 80);   //Hue 95~115
		mLowTopHue_test = mHighTopHue_test = cv::Scalar(115, 255, 255);

		mArea_thres = 40000;
		mH_w_high_thres = 1.1;
		mH_w_low_thres = 0.9;

		mDil_iter = 0;
		mEro_iter = 2;

		mDil_ksize = cv::Size(13, 13);
		mEro_ksize = cv::Size(13, 13);

		mCali_num = 0;
		mCali_ksize = 3;
		break;

	case YELLOWSIGN: 
		mLowBottomHue_test = mHighBottomHue_test = cv::Scalar(20, 80, 80); // Hue 20 ~ 25
		mLowTopHue_test = mHighTopHue_test = cv::Scalar(40, 255, 255);

		mArea_thres = 40000;
		mH_w_high_thres = 1.1;
		mH_w_low_thres = 0.9;

		mDil_iter = 0;
		mEro_iter = 2;

		mDil_ksize = cv::Size(13, 13);
		mEro_ksize = cv::Size(13, 13);

		mCali_num = 0;
		mCali_ksize = 3;
		break;
	default:
		break;
	}
}

Camera::~Camera()
{
}




void cam_expos_cali(cv::VideoCapture& cap, exposure& expose)
{
	/*double exposure;
	switch (expose)
	{
	case OUT:
		exposure = -10;
		break;
	case OUT_SUNLIGHT:
		exposure = -10;
		break;
	case IN:
		exposure = -6;
		break;
	default:
		break;
	}*/
	//cap.set(CV_CAP_PROP_EXPOSURE, 1);
	//cap.set(CV_CAP_PROP_EXPOSURE, );
#if SET_CAMERA_CONFIG==1
	cap.set(CV_CAP_PROP_SETTINGS, 1);
#endif // CAMERA_CONFIG

}

bool Camera::flush_cam_buffer(){
	 for(int i=0;i<CAMERA_BUFFER_FRAMES;i++){
		if(!mCap.read(mFrame)){
		
                	std::cout <<" cannot read buffer frame... " << std::endl;
	                return false;
		}
        }
	return true;
}

bool Camera::Detect(int &dest_x, int &dest_y){

        Timer module_t;
        bool cali = false;
        int median_idx = mCali_ksize / 2;
        int cali_x = 0;
        int cali_y = 0;
        int* temp_x = new int[mCali_ksize];
        int* temp_y = new int[mCali_ksize];
        int pre_x = 0;
        int pre_y = 0;
        cv::Rect pre_bound_Rect;


        //module_t.start();

        ////////////////////////STEP 1   Read frame  //////////////////////////
        if (!mCap.read(mFrame)) {
                std::cout <<" cannot read frame... " << std::endl;
                return -1;
        }
        ////////////////////////STEP   Read frame  //////////////////////////
        //module_t.stop();      
        //module_t.display("readframe");


        ////////////////////////STEP 2   Clone //////////////////////////
        cv::Mat bgr_img = mFrame.clone();
        ////////////////////////STEP 2   Clone //////////////////////////

	 ////////////////////////STEP 3  medianBlur //////////////////////////
        cv::medianBlur(bgr_img, bgr_img, 3.0f);
        ////////////////////////STEP 3   medianBlur //////////////////////////



        ////////////////////////STEP 4   GaussianBlur //////////////////////////
        GaussianBlur(bgr_img, bgr_img, cv::Size(3, 3), 4);
        ////////////////////////STEP 4   GaussianBlur //////////////////////////

        //module_t.start();
        ////////////////////////STEP 5   ColorFiltering //////////////////////////
        cv::Mat hsv_img;
        cv::Mat filtered_hsv;
        cv::cvtColor(bgr_img, hsv_img, cv::COLOR_BGR2HSV);

        filter_color(hsv_img, filtered_hsv,mLowBottomHue_test, mLowTopHue_test, mHighBottomHue_test, mHighTopHue_test);
        ////////////////////////STEP 5   ColorFiltering //////////////////////////
        //module_t.stop();      
        //module_t.display("colorfilter");

	 module_t.start();
        ////////////////////////STEP 6   Erode & Dilate //////////////////////////
        cv::Mat ero_element = cv::getStructuringElement(cv::MORPH_ELLIPSE, mEro_ksize);
        cv::Mat dil_element = cv::getStructuringElement(cv::MORPH_ELLIPSE, mDil_ksize);
        cv::Mat filtered_hsv_dil;
        cv::Mat filtered_hsv_ero;
        cv::erode(filtered_hsv, filtered_hsv_ero, ero_element, cv::Point(-1, -1), mEro_iter);
        cv::dilate(filtered_hsv_ero, filtered_hsv_dil, dil_element, cv::Point(-1, -1), mDil_iter);
        ////////////////////////STEP 6   Erode & Dilate //////////////////////////
        module_t.stop();
        module_t.display("E&D");


        ////////////////////////STEP 7   Canny //////////////////////////
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::Mat canny_contours;
        cv::Canny(filtered_hsv_dil, canny_contours, 50, 100);
        //cv::threshold(canny_contours,canny_contours,128,255,cv::THRESH_BINARY_INV);
        ////////////////////////STEP 7   Canny //////////////////////////


        ////////////////////////STEP 8   FindContours //////////////////////////
        cv::findContours(canny_contours, contours, hierarchy, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
        ////////////////////////STEP 8   FindContours //////////////////////////

                        //module_t.start();
        ////////////////////////STEP 9   Shape + Area + Height Width Ratio + Center //////////////////////////
        //std::cout<<contours.size()<<std::endl;
        std::string shape;
        std::vector<std::vector<cv::Point>> approx_contours(contours.size());

        int cm_rect_x = 0;
        int cm_rect_y = 0;
        std::vector<cv::Rect> bound_Rect(contours.size());


        for (int i = 0; i<contours.size(); i++) {
                //cv::drawContours(mFrame, contours, i, cv::Scalar(100, 255, 255), 1, 8, hierarchy, 1);
                //      area = cv::contourArea(contours[i]);  // contour area
                bound_Rect[i] = cv::boundingRect(cv::Mat(contours[i]));
                //std::cout << i << "-th contour area: " << bound_Rect[i].area() << std::endl;

                //check if area and height width ratio is met
                float h_w_ratio = (float)bound_Rect[i].width / bound_Rect[i].height;
                bool Area_gate = bound_Rect[i].area() >= mArea_thres;
                bool h_w_ratio_gate = (h_w_ratio <= mH_w_high_thres) ? ((h_w_ratio >= mH_w_low_thres) ? true : false) : false;


		 if (Area_gate && h_w_ratio_gate) {

                        double contour_len = cv::arcLength(contours[i], true);
                        cv::approxPolyDP(contours[i], approx_contours[i], contour_len*0.04, true);

                        ///////////calculate moments////////
                        //m = cv::moments(contours[i]);
                        //cm_x = int(m.m10 / m.m00);
                        //cm_y = int(m.m01 / m.m00);
                        ////////////////////////////////////

                        ///////////calculate bound rect cm/////////////////////////
                        cm_rect_x = (bound_Rect[i].x + bound_Rect[i].width / 2);
                        cm_rect_y = (bound_Rect[i].y + bound_Rect[i].height / 2);
                        ///////////calculate bound rect cm/////////////////////////

                        ////////Determine shape////////////////
                        switch (approx_contours[i].size()) {
                        case 3:
                                shape = "triangle";
                                break;

                        case 4:
                                shape = "rect";
                                break;

                        default:
                                shape = "circle";
                                break;
                        }
                        ////////////////////////////////////////
                        /////////////////////////debug  info///////////////////////////////////////////////////////

#if CALIBRATE==1
                        temp_x[mCali_num] = cm_rect_x;
                        temp_y[mCali_num] = cm_rect_y;
                       	mCali_num++;
                        if (mCali_num % mCali_ksize == 0) {
                                cali = true;
                                mCali_num = 0;
                        }
                        else cali = false;


                        if (cali) {
                                quickSort(temp_x, 0, mCali_ksize);
                                quickSort(temp_y, 0, mCali_ksize);
                                cali_x = temp_x[median_idx];
                                cali_y = temp_y[median_idx];
                                std::cout << i << "-th contour area: " << bound_Rect[i].area() << "    height-width ratio=" << h_w_ratio << std::endl;
                                std::cout << "          x= " << cali_x << "             y=" << cali_y << "\n" << std::endl;;
                                cv::rectangle(mFrame, bound_Rect[i], cv::Scalar(100, 255, 255));
                                cv::putText(mFrame, "o", cv::Point(cali_x, cali_y), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(180, 255, 255));
                                pre_bound_Rect = bound_Rect[i];
                                pre_x = cali_x;
                                pre_y = cali_y;
                        }

                        dest_x = pre_x;
                        dest_y = pre_y;
#else
                        dest_x = cm_rect_x;
                        dest_y = cm_rect_y;
#endif

#if (CALIBRATE==1 && SHOWDEBUG==1)
                        cv::rectangle(mFrame, pre_bound_Rect, cv::Scalar(100, 255, 255));
                        cv::putText(mFrame, "o", cv::Point(pre_x, pre_y), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(180, 255, 255));
                        std::cout << "          x= " << pre_x << "              y=" << pre_y << "\n" << std::endl;
#endif

#if (CALIBRATE==0 && SHOWDEBUG==1 )
                        cv::rectangle(mFrame, bound_Rect[i], cv::Scalar(100, 255, 255));
                        cv::putText(mFrame, "o", cv::Point(cm_rect_x, cm_rect_y), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(180, 255, 255));
                //      std::cout << "          x= " << cm_rect_x << "          y=" << cm_rect_y << "\n" << std::endl;
#endif


                        //send data  

                }
        }
        ////////////////////////STEP 9   Shape + Area + Height Width Ratio + Center //////////////////////////
        //module_t.stop();      
        //module_t.display("area");


#if (IMG_MODE==0 && RECORD_VIDEO==1)
        video_filtering.write(filtered_hsv_dil);
        video.write(mFrame);
#endif
        //cv::namedWindow("filtering", cv::WINDOW_AUTOSIZE);
        //cv::imshow("filtering", filtered_hsv);

        //module_t.start();
#if SHOWDEBUG==1
                cv::namedWindow("dilate", cv::WINDOW_AUTOSIZE);
                cv::imshow("dilate", filtered_hsv_dil);
             // cv::namedWindow("erode", cv::WINDOW_AUTOSIZE);
             // cv::imshow("erode", filtered_hsv_ero);
              cv::namedWindow("window", cv::WINDOW_AUTOSIZE);
             cv::imshow("window", mFrame);
		//std::cout<<"dest x: "<<dest_x<<"  dest y: "<<dest_y<<std::endl;
#endif
        //module_t.stop();      
        //module_t.display("display");
        return 1;


}

bool Camera::Detect_side(int& dest_x, int& dest_y, MissionColors& color){
	Timer module_t;
	bool cali = false;
	int median_idx = mCali_ksize / 2;
	int cali_x = 0;
	int cali_y = 0;
	int* temp_x = new int[mCali_ksize];
	int* temp_y = new int[mCali_ksize];
	int pre_x = 0;
	int pre_y = 0;
	cv::Rect pre_bound_Rect;

	//module_t.start();
	mFrame.release(); //flush the previous image
	////////////////////////STEP 1   Read frame  //////////////////////////
	if (!mCap.read(mFrame)) {
		std::cout << " cannot read side frame... " << std::endl;
		return -1;
	}
	////////////////////////STEP   Read frame  //////////////////////////
	//module_t.stop();	
	//module_t.display("readframe");


	////////////////////////STEP 2   Clone //////////////////////////
	cv::Mat bgr_img = mFrame.clone();
	////////////////////////STEP 2   Clone //////////////////////////



	////////////////////////STEP 3  medianBlur //////////////////////////
	cv::medianBlur(bgr_img, bgr_img, 3.0f);
	////////////////////////STEP 3   medianBlur //////////////////////////



	////////////////////////STEP 4   GaussianBlur //////////////////////////
	GaussianBlur(bgr_img, bgr_img, cv::Size(3, 3), 4);
	////////////////////////STEP 4   GaussianBlur //////////////////////////


	


	//////////////////////////////////////////////////////////////////////////
	////////////////////////STEP 5   ColorFiltering //////////////////////////
	cv::Mat hsv_img;
	cv::Mat filtered_hsv;
	cv::cvtColor(bgr_img, hsv_img, cv::COLOR_BGR2HSV);

	module_t.start();
	//////////////////////scan red sign///////////////////////////
	if (Detect_side_scanner(hsv_img, filtered_hsv, dest_x, dest_y, REDSIGN) > 0) color= RED;
	//////////////////////scan red sign///////////////////////////

	//////////////////////scan blue sign///////////////////////////
	if (Detect_side_scanner(hsv_img, filtered_hsv, dest_x, dest_y, BLUESIGN) > 0) color = BLUE;
	//////////////////////scan blue sign///////////////////////////

	//////////////////////scan yellow sign///////////////////////////
	if (Detect_side_scanner(hsv_img, filtered_hsv, dest_x, dest_y, YELLOWSIGN) > 0) color = YELLOW;
	//////////////////////scan yellow sign///////////////////////////

	//////////////////////////////////////////////////////////////////////////
	////////////////////////STEP 5   ColorFiltering //////////////////////////

	module_t.stop();	
	module_t.display("three scanner");


#if RECORD_VIDEO==1
	video_filtering.write(filtered_hsv);
	video.write(mFrame);
#endif


	//module_t.start();
#if SHOWDEBUG==1
	//cv::namedWindow("ero", cv::WINDOW_AUTOSIZE);
	//cv::imshow("ero", filtered_hsv_ero);
	cv::namedWindow("window", cv::WINDOW_AUTOSIZE);
	cv::imshow("window", mFrame);
#endif
	//module_t.stop();	
	//module_t.display("display");
	return 1;


}



bool Camera::Detect_side_scanner(cv::Mat& hsv_img, cv::Mat& filtered_hsv, int& dest_x, int& dest_y, CameraParameters colorParameter){
	
	Timer module_t;
	bool color_scanned = false;
////////////////// set the upcomming fitering function with the corresponding filtering parameters///////////////
	set_parameter(colorParameter);
////////////////// set the upcomming fitering function with the corresponding filtering parameters///////////////


/////////////////// finish filtering color/////////////////////////////////////////
	filter_color(hsv_img, filtered_hsv,mLowBottomHue_test, mLowTopHue_test, mHighBottomHue_test, mHighTopHue_test);
/////////////////// finish filtering color/////////////////////////////////////////


/*

module_t.start();
////////////////////////STEP 6   Erode & Dilate //////////////////////////
cv::Mat ero_element = cv::getStructuringElement(cv::MORPH_ELLIPSE, mEro_ksize);
cv::Mat filtered_hsv_ero;
cv::erode(filtered_hsv, filtered_hsv_ero, ero_element, cv::Point(-1, -1),mEro_iter);
////////////////////////STEP 6   Erode & Dilate //////////////////////////
module_t.stop();
module_t.display("E&D");

*/


////////////////////////STEP 7   Canny //////////////////////////
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::Mat canny_contours;
	cv::Canny(filtered_hsv, canny_contours, 50, 100);
//cv::threshold(canny_contours,canny_contours,128,255,cv::THRESH_BINARY_INV);
////////////////////////STEP 7   Canny //////////////////////////


////////////////////////STEP 8   FindContours //////////////////////////
	cv::findContours(canny_contours, contours, hierarchy, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
////////////////////////STEP 8   FindContours //////////////////////////


//module_t.start();
////////////////////////STEP 9   Shape + Area + Height Width Ratio + Center //////////////////////////
//std::cout<<contours.size()<<std::endl;
	std::string shape;
	std::vector<std::vector<cv::Point>> approx_contours(contours.size());

	int cm_rect_x = 0;
	int cm_rect_y = 0;
	std::vector<cv::Rect> bound_Rect(contours.size());


	for (int i = 0; i<contours.size(); i++) {
	//cv::drawContours(mFrame, contours, i, cv::Scalar(100, 255, 255), 1, 8, hierarchy, 1);
	//	area = cv::contourArea(contours[i]);  // contour area
		bound_Rect[i] = cv::boundingRect(cv::Mat(contours[i]));
	//std::cout << i << "-th contour area: " << bound_Rect[i].area() << std::endl;

	//check if area and height width ratio is met
		float h_w_ratio = (float)bound_Rect[i].width / bound_Rect[i].height;
		bool Area_gate = bound_Rect[i].area() >= mArea_thres;
		bool h_w_ratio_gate = (h_w_ratio <= mH_w_high_thres) ? ((h_w_ratio >= mH_w_low_thres) ? true : false) : false;


		if (Area_gate && h_w_ratio_gate) {
	
			double contour_len = cv::arcLength(contours[i], true);
			cv::approxPolyDP(contours[i], approx_contours[i], contour_len*0.04, true);



		///////////calculate bound rect cm/////////////////////////
			cm_rect_x = (bound_Rect[i].x + bound_Rect[i].width / 2);
			cm_rect_y = (bound_Rect[i].y + bound_Rect[i].height / 2);
		///////////calculate bound rect cm/////////////////////////


		////////Determine shape////////////////
			switch (approx_contours[i].size()) {
			case 3:
				shape = "triangle";
				break;

			case 4:
				shape = "rect";
				break;

			default:
				shape = "circle";
				break;
			}
		////////////////////////////////////////
		/////////////////////////debug  info///////////////////////////////////////////////////////



#if CALIBRATE==1
			temp_x[mCali_num] = cm_rect_x;
			temp_y[mCali_num] = cm_rect_y;
			mCali_num++;
			if (mCali_num % mCali_ksize == 0) {
				cali = true;
				mCali_num = 0;
			}
			else cali = false;


			if (cali) {
				quickSort(temp_x, 0, mCali_ksize);
				quickSort(temp_y, 0, mCali_ksize);
				cali_x = temp_x[median_idx];
				cali_y = temp_y[median_idx];
				std::cout << i << "-th contour area: " << bound_Rect[i].area() << "    height-width ratio=" << h_w_ratio << std::endl;
				std::cout << "		x= " << cali_x << "		y=" << cali_y << "\n" << std::endl;;
				cv::rectangle(mFrame, bound_Rect[i], cv::Scalar(100, 255, 255));
				cv::putText(mFrame, "o", cv::Point(cali_x, cali_y), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(180, 255, 255));
				pre_bound_Rect = bound_Rect[i];
				pre_x = cali_x;
				pre_y = cali_y;
			}

			dest_x = pre_x;
			dest_y = pre_y;
#else
			dest_x = cm_rect_x;
			dest_y = cm_rect_y;
#endif

#if (CALIBRATE==1 && SHOWDEBUG==1)
			cv::rectangle(mFrame, pre_bound_Rect, cv::Scalar(100, 255, 255));
			cv::putText(mFrame, "o", cv::Point(pre_x, pre_y), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(180, 255, 255));
			std::cout << "		x= " << pre_x << "		y=" << pre_y << "\n" << std::endl;
#endif

#if (CALIBRATE==0 && SHOWDEBUG==1 )
			cv::rectangle(mFrame, bound_Rect[i], cv::Scalar(100, 255, 255));
			cv::putText(mFrame, "o", cv::Point(cm_rect_x, cm_rect_y), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(180, 255, 255));
		//	std::cout << "		x= " << cm_rect_x << "		y=" << cm_rect_y << "\n" << std::endl;
#endif
			color_scanned = true;
		}
	}


////////////////////////STEP 9   Shape + Area + Height Width Ratio + Center //////////////////////////
//module_t.stop();	
//module_t.display("area");

	return color_scanned;
}

