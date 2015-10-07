#include "convert.h"
#include <cstring>
cv::Mat converter::BITMAP_to_cv_Mat(const BITMAP & img){
	cv::Mat(img.bmWidth, img.bmHeight, CV_8UC4);
	return cv::Mat();
}
