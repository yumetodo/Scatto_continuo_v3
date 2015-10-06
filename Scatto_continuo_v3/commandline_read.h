#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <memory>
#include <stdexcept>
#include "fileinfo.h"
enum class colormode_edit {
	none,
	gray_scaled,
	thresholding
};
struct PROCESS_CONF {
	std::vector<int> param;
	double threshold;//二値化のしきい値。0なら大津のアルゴリズムを使用（default = 0）
	filename_c o_file;//-n
	colormode_edit process_mode;//-edit
	uintmax_t frame_num;//-f
	double fps;//-fps
};

class successful_termination : public std::runtime_error {
public:
	explicit successful_termination(const std::string& what_arg);
	explicit successful_termination(const char* what_arg);
};
std::string determine_filename(const uintmax_t roop_turn, const PROCESS_CONF& conf);
PROCESS_CONF commandline_analyzer(int argc, char* argv[]);
void print_help();
