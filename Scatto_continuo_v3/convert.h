#pragma once
#include <opencv2/core/core.hpp>
#include <wtypes.h>
#include <wingdi.h>

namespace converter {
	cv::Mat BITMAP_to_cv_Mat(const BITMAP& img);
}