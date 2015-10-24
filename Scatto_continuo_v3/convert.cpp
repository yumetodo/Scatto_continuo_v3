#include "convert.h"
#include <cstring>
namespace converter {
	cv::Mat BITMAP_to_cv_Mat(const BITMAP & img) {
		cv::Mat re(img.bmHeight, img.bmWidth, CV_8UC4, img.bmBits,img.bmWidthBytes);
		cv::flip(re, re, 0);//上下反転
		return re;
	}
}
