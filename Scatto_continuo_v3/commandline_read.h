#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <unordered_map>
enum class colormode_edit {
	none,
	gray_scaled,
	thresholding
};
enum class picture_type {
	png,
	jpg,
	pbm,
	pgm,
	ppm
};
struct to_picture_type {
public:
	to_picture_type();
	picture_type operator()(const std::string& ext) const;
private:
	std::unordered_map<std::string, picture_type> convert_table_;
};
class filename_c {
public:
	filename_c();
	filename_c(std::string&& ext);
	filename_c(std::string&& l, std::string&& r, std::string&& ext);
	filename_c(const filename_c& o) = default;
	filename_c(filename_c&& o) = default;
	filename_c& operator=(const filename_c& o) = default;
	filename_c& operator=(filename_c&& o) = default;
	std::string create(const std::string& num_str) const;
	const std::string& get_extension() const noexcept;
	picture_type get_pic_type() const;
private:
	std::string l_;
	std::string r_without_ext_;
	std::string ext_;
	std::string r_;
	bool constracted_;
	bool has_fullpath_;
	to_picture_type conv_;
};
struct PROCESS_CONF {
	std::vector<int> param;
	double threshold;//二値化のしきい値。0なら大津のアルゴリズムを使用（default = 0）
	filename_c o_file;//-n
	colormode_edit process_mode;//-edit
	uintmax_t frame_num;//-f
	double fps;//-fps
};
std::string determine_filename(const uintmax_t roop_turn, const PROCESS_CONF& conf);
