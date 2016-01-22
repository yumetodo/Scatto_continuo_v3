#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include "win32api_wrap.h"
#include "convert.h"
#include "commandline_read.h"
void count_down(size_t sec) {
	std::cout << "撮影開始まで" << std::endl;
	for (decltype(sec) i = 0; i < sec; ++i) {
		//auto begin = std::chrono::high_resolution_clock::now();
		std::cout << sec - i << "..." << std::endl;
		win32api_wrap::beep(30000, 200);//レ＃, 0.2sec
		std::this_thread::sleep_for(std::chrono::milliseconds(999));
		//std::this_thread::sleep_until(begin + std::chrono::milliseconds(1000));
	}
}
cv::Mat image_editer(cv::Mat& in, const PROCESS_CONF& conf) {
	if (colormode_edit::none == conf.process_mode) {
		return in;
	}
	else {
		cv::Mat tmp1;
		cv::cvtColor(in, tmp1, cv::COLOR_BGR2GRAY);
		if (colormode_edit::gray_scaled == conf.process_mode) {
			return tmp1;
		}
		else {
			cv::Mat tmp2;
			cv::threshold(tmp1, tmp2, conf.threshold, 255, (0.0 == conf.threshold) ? cv::THRESH_BINARY | cv::THRESH_OTSU : cv::THRESH_BINARY);
			return tmp2;
		}
	}
}
void save_image(cv::Mat& img, const PROCESS_CONF& conf, uintmax_t roop_turn) {
	const auto name = determine_filename(roop_turn, conf);
	if (conf.param.empty()) {
		cv::imwrite(name, img);
	}
	else {
		cv::imwrite(name, img, conf.param);
	}
}
int main(int argc, char* argv[]) {
	try {
		const auto conf = commandline_analyzer(argc, argv);
		if(!conf.no_countdown_timer) count_down(5);
		auto cap = win32api_wrap::capture_c(win32api_wrap::get_desktop_window_handle());
		for (size_t i = 0; i < conf.frame_num; ++i) {
			auto bm = cap.capture();
			auto cvimage = converter::BITMAP_to_cv_Mat(bm);
			auto re = image_editer(cvimage, conf);
			save_image(re, conf, i);
		}
	}
	catch (const successful_termination&) {//commandline_analyzerのexit例外
		return 0;
	}
	catch (const std::exception& er) {
		std::cerr << er.what() << std::endl;
		return -1;
	}
	return 0;
}
