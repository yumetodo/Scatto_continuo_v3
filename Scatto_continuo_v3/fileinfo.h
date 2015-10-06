#pragma once
#include <string>
#include <memory>
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
	struct Impl;
	std::shared_ptr<Impl> pimpl_;
};
class filename_c {
public:
	filename_c();
	filename_c(std::string&& ext);
	filename_c(std::string&& name, std::string&& ext);
	filename_c(std::string && n_l, std::string && n_r, std::string && ext);
	filename_c(std::string && path, std::string && n_l, std::string && n_r, std::string && ext);
	filename_c(const filename_c& o) = default;
	filename_c(filename_c&& o) = default;
	filename_c& operator=(const filename_c& o) = default;
	filename_c& operator=(filename_c&& o) = default;
	std::string create(const std::string& num_str) const;
	std::string create_fullpath(const std::string& num_str) const;
	const std::string& get_extension() const noexcept;
	picture_type get_pic_type() const;
private:
	std::string l_;
	std::string n_l_;
	std::string n_r_;
	std::string dp_;
	std::string ext_;
	std::string r_;
	bool has_fullpath_;
	to_picture_type conv_;
};
