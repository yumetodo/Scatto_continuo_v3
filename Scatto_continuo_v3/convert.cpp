#include "convert.h"
#include <cstring>
namespace converter {
	cv::Mat BITMAP_to_cv_Mat(const BITMAP & img) {
		cv::Mat re;
		cv::flip(cv::Mat(img.bmWidth, img.bmHeight, CV_8UC4, img.bmBits), re, 0);//上下反転
		return re;
	}
}
