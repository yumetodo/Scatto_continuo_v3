#include "fileinfo.h"
#include "win32api_wrap.h"
#include <unordered_map>

filename_c::filename_c() : filename_c("png") {}

filename_c::filename_c(std::string && ext) : filename_c("caputure", std::move(ext)) {}

filename_c::filename_c(std::string && name, std::string && ext) : filename_c(std::move(name), "", std::move(ext)) {}

filename_c::filename_c(std::string && n_l, std::string && n_r, std::string && ext)
	: dp_(""), n_l_(std::move(n_l)), n_r_(std::move(n_r)), ext_(std::move(ext)), has_fullpath_(false), conv_()
{
	this->l_ = win32api_wrap::get_current_directory() + "\\" + this->n_l_;
	this->r_ = this->n_r_ + "." + this->ext_;
}

filename_c::filename_c(std::string && path, std::string && n_l, std::string && n_r, std::string && ext)
	: dp_(std::move(path)), n_l_(std::move(n_l)), n_r_(std::move(n_r)), ext_(std::move(ext)), conv_()
{
	this->has_fullpath_ = win32api_wrap::path_exist(this->dp_);
	if (this->has_fullpath_) {
		this->l_ = this->dp_ + this->n_l_;
	}
	else {
		this->l_ = win32api_wrap::get_current_directory() + "\\" + this->n_l_;
	}
	this->r_ = this->n_r_ + "." + this->ext_;
}

std::string filename_c::create(const std::string& num_str) const {
	return ((this->has_fullpath_) ? this->l_ : this->n_l_) + num_str + this->r_;
}

std::string filename_c::create_fullpath(const std::string & num_str) const {
	return this->n_l_ + num_str + this->r_;
}

const std::string & filename_c::get_extension() const noexcept {
	return this->ext_;
}

picture_type filename_c::get_pic_type() const {
	return this->conv_(this->ext_);
}

struct to_picture_type::Impl {
	Impl(std::unordered_map<std::string, picture_type>&& table) {
		this->convert_table_ = std::move(table);
	}
	std::unordered_map<std::string, picture_type> convert_table_;
};

to_picture_type::to_picture_type() {
	std::unordered_map<std::string, picture_type> re;
	re.insert(std::make_pair(std::string("png"), picture_type::png));
	re.insert(std::make_pair(std::string("jpg"), picture_type::jpg));
	re.insert(std::make_pair(std::string("pbm"), picture_type::pbm));
	re.insert(std::make_pair(std::string("pgm"), picture_type::pgm));
	re.insert(std::make_pair(std::string("ppm"), picture_type::ppm));
	this->pimpl_ = std::make_shared<Impl>(std::move(re));
}

picture_type to_picture_type::operator()(const std::string & ext) const {
	return this->pimpl_->convert_table_.at(ext);
}
