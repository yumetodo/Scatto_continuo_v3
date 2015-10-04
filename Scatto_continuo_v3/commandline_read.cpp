#include <opencv2/highgui/highgui.hpp>
#include "commandline_read.h"
#include <Windows.h>
#include <iomanip>
#include <functional>
#include <type_traits>
#include <errno.h>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <exception>
#include <Shlwapi.h>//need -lShlwapi
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
filename_c::filename_c() : filename_c(std::string("png")) {
	this->constracted_ = false;
}

filename_c::filename_c(std::string && ext) : l_("caputure"), r_without_ext_(""), ext_(std::move(ext)), has_fullpath_(false){
	this->r_ = this->r_without_ext_ + "." + this->ext_;
	this->constracted_ = true;
}

filename_c::filename_c(std::string && l, std::string && r, std::string && ext)
: l_(std::move(l)), r_without_ext_(std::move(r)), ext_(std::move(ext)), constracted_(true), has_fullpath_(false)
{
	this->r_ = this->r_without_ext_ + "." + this->ext_;
}

std::string filename_c::create(const std::string& num_str) const {
	return this->l_ + num_str + this->r_;
}

const std::string & filename_c::get_extension() const noexcept{
	return this->ext_;
}

picture_type filename_c::get_pic_type() const{
	return this->conv_(this->ext_);
}
to_picture_type::to_picture_type(){
	std::unordered_map<std::string, picture_type> re;
	re.insert(std::make_pair(std::string("png"), picture_type::png));
	re.insert(std::make_pair(std::string("jpg"), picture_type::jpg));
	re.insert(std::make_pair(std::string("pbm"), picture_type::pbm));
	re.insert(std::make_pair(std::string("pgm"), picture_type::pgm));
	re.insert(std::make_pair(std::string("ppm"), picture_type::ppm));
	this->convert_table_ = std::move(re);
}

picture_type to_picture_type::operator()(const std::string & ext) const{
	return this->convert_table_.at(ext);
}

namespace win32api_wrap {
	static std::string get_current_directory() {
		std::string re(MAX_PATH + 10, '\0');
		const auto len = GetCurrentDirectoryA(re.size() - 1, &re.front());
		re.resize(len);
		return re;
	}
}

std::string determine_filename(const uintmax_t roop_turn, const PROCESS_CONF& conf) {
	size_t num_to_avoid_conflict = 0;
	std::string path;
	do {
		std::stringstream ss;
		ss << roop_turn << "_" << std::setw(2) << std::setfill('0') << num_to_avoid_conflict;
		path = win32api_wrap::get_current_directory() + "\\" + conf.o_file.create(ss.str());
	} while (PathFileExistsA(path.c_str()));
	return path;
}
namespace strtonum {
	template<typename T_> using limit = std::numeric_limits<T_>;
	template<bool condition, typename T = void>
	using enable_if_type = typename std::enable_if<condition, T>::type;
	template<typename T_, enable_if_type<std::is_signed<T_>{}, std::nullptr_t> = nullptr> 
	T_ purseInt(const char* str, const T_ max = limit<T_>::max(), const T_ min = limit<T_>::lowest()) {
		errno = 0;
		const auto re = std::strtoll(str, nullptr, 10);
		if (!errno || re < min || max < re) throw std::runtime_error("変換に失敗しました");
		return static_cast<T_>(re);
	}
	template<typename T_, enable_if_type<std::is_unsigned<T_>::value, std::nullptr_t> = nullptr> 
	T_ purseUint(const char* str, const T_ max = limit<T_>::max(), const T_ min = limit<T_>::lowest()) {
		errno = 0;
		const auto re = std::strtoull(str, nullptr, 10);
		if (!errno || re < min || max < re) throw std::runtime_error("変換に失敗しました");
		return static_cast<T_>(re);
	}

	template<typename T_, enable_if_type<std::is_floating_point<T_>{}, std::nullptr_t> = nullptr> 
	T_ purseDouble(const char* str, const T_ max = limit<T_>::max(), const T_ min = limit<T_>::lowest()) {
		errno = 0;
		const auto re = std::strtod(str, nullptr);
		if (!errno || re < min || max < re) throw std::runtime_error("変換に失敗しました");
		return static_cast<T_>(re);
	}
}
PROCESS_CONF commandline_analyzer(int argc, char* argv[]) {
	using namespace strtonum;
	PROCESS_CONF re = {};
	re.threshold = 0.0;
	re.process_mode = colormode_edit::none;
	re.frame_num = 1;
	re.fps = 4.5;

	std::unordered_map<std::string, std::function<void(const char *)>> cases = {
		{
			"-f",
			[&re](const char* arg){
				try {
					re.frame_num = purseUint<uintmax_t>(arg);
				}
				catch (const std::exception& er) {
					re.frame_num = 1;
				}
			}
		},
		{
			"-fps",
			[&re](const char* arg) {
				try {
					re.fps = purseDouble<double>(arg);
				}
				catch (const std::exception& er) {
					re.fps = 4.5;
				}
			}
		},
		{
			"-n",
			[&re](const char* arg) {

			}
		}
	};
}

template<typename O, typename T> class unorderd_map_getOrDefault {
public:
	unorderd_map_getOrDefault(const O& key, const T& val) : key_(key), val_(val) {}
private:
	template<typename O, typename T> friend T getOrDefault(const std::unordered_map<O, T>& map, const unorderd_map_getOrDefault& arg);
	O key_;
	T val_;
};
template<typename O, typename T> unorderd_map_getOrDefault getOrDefault(const O& key, const T& val) {
	return unorderd_map_getOrDefault(key, val);
}
template<typename O, typename T> T operator|(const std::unordered_map<O, T>& map, unorderd_map_getOrDefault&& arg) {
	return (map.count(arg.key_)) ? map[key] : arg.val_;
}



