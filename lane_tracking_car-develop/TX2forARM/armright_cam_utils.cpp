#include"armright_cam_utils.h"


void filter_color(cv::Mat &hsv_img, cv::Mat &filtered_hsv, cv::Scalar lowBottomHue, cv::Scalar lowTopHue, cv::Scalar highBottomHue, cv::Scalar highTopHue) {
	
	cv::Mat low_hue_hsv;
	cv::Mat high_hue_hsv;

	cv::inRange(hsv_img, lowBottomHue, lowTopHue, low_hue_hsv);
	cv::inRange(hsv_img, highBottomHue, highTopHue, high_hue_hsv);
	cv::addWeighted(low_hue_hsv, 1.0, high_hue_hsv, 1.0, 0.0, filtered_hsv);
	//cv::GaussianBlur(red_hue_hsv, red_hue_hsv, cv::Size(9, 9), 1, 1);
}


void swap(int* a, int* b) {
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}
void quickSort(int* src, int L_idx, int R_idx) {
	int i, j;
	if (L_idx < R_idx) {
		i = L_idx;
		j = R_idx + 1;
		while (1)
		{
			while (i<R_idx &&  src[++i]<src[L_idx]);
			while (j>L_idx &&  src[--j]>src[L_idx]);
			if (i >= j) break;
			swap(&src[i], &src[j]);
		}
		swap(&src[L_idx], &src[j]);
		quickSort(src, L_idx, j - 1);
		quickSort(src, j + 1, R_idx);
	}
}





