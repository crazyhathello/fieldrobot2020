#ifndef _ARMRIGHT_CAM_UTILS_
#define _ARMRIGHT_CAM_UTILS_

#include <opencv2/opencv.hpp>





//The input hsv image will be filtered into a binary filtered image corresponding to the given Hue range
void filter_color(cv::Mat &hsv_img, cv::Mat &filtered_hsv, cv::Scalar lowBottomHue, cv::Scalar lowTopHue, cv::Scalar highBottomHue, cv::Scalar highTopHue);


void quickSort(int* src, int L_idx, int R_idx);

void swap(int* a, int* b);


#endif // !_ARMRIGHT_CAM_UTILS_
