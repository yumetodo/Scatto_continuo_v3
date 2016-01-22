#include <iostream>
#include <sstream>  
#include <iomanip>
#include <opencv2/highgui/highgui.hpp>
#include "commandline_read.h"
#include "print_version.h"
#include "win32api_wrap.h"
#include <unordered_map>
#include <functional>
#include <cerrno>
#include <cstring>
#include <limits>
#include <type_traits>
#include <stdexcept>
#include <exception>
static std::string create_string_to_avoid_conflict(const uintmax_t roop_turn, const size_t num_to_avoid_conflict) {
	std::stringstream ss;
	ss << roop_turn << "_" << std::setw(2) << std::setfill('0') << num_to_avoid_conflict;
	return ss.str();
}

std::string determine_filename(const uintmax_t roop_turn, const PROCESS_CONF& conf) {
	size_t num_to_avoid_conflict = 0;
	std::string string_to_avoid_conflict;
	do {
		++num_to_avoid_conflict;
		string_to_avoid_conflict = create_string_to_avoid_conflict(roop_turn, num_to_avoid_conflict);
	} while (win32api_wrap::file_exist(conf.o_file.create_fullpath(string_to_avoid_conflict)));
	return conf.o_file.create(string_to_avoid_conflict);
}
namespace strtonum {
	template<typename T_> using limit = std::numeric_limits<T_>;
	template<bool condition, typename T = void>
	using enable_if_type = typename std::enable_if<condition, T>::type;
	template<typename T_, enable_if_type<std::is_signed<T_>::value, std::nullptr_t> = nullptr> 
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

	template<typename T_, enable_if_type<std::is_floating_point<T_>::value, std::nullptr_t> = nullptr> 
	T_ purseDouble(const char* str, const T_ max = limit<T_>::max(), const T_ min = limit<T_>::lowest()) {
		errno = 0;
		const auto re = std::strtod(str, nullptr);
		if (!errno || re < min || max < re) throw std::runtime_error("変換に失敗しました");
		return static_cast<T_>(re);
	}
}

static std::pair<std::string, std::string> split_name(std::string&& name) {
	const char* rep_str = "$(n)";
	const auto split_i = name.find(rep_str);
	if (std::string::npos == split_i) {
		return{ std::move(name) , "" };
	}
	else {
		std::pair<std::string, std::string> re;
		re.second = name.substr(split_i + std::strlen(rep_str) + 1);
		name.erase(0, split_i);
		re.first = std::move(name);
		return re;
	}
}

static inline filename_c process_purse_o_option(const std::string& path) {
	using std::move;
	const auto p_last = path.find_last_of("\\");
	if (0 == p_last
		|| std::string::npos == p_last
		|| !win32api_wrap::path_exist(path.substr(0, p_last))
		) {
		const auto n_f_i = (std::string::npos == p_last) ? 0 : p_last;
		return filename_c(split_name(path.substr(n_f_i, path.find_last_of('.') - n_f_i)), path.substr(path.find_last_of(".") + 1));
	}
	else {
		auto n = split_name(path.substr(p_last + 1, path.find_last_of('.') - p_last));
		return filename_c(path.substr(0, p_last) + "\\", move(n), path.substr(path.find_last_of(".") + 1));
	}
}

void print_help() {
	using std::endl;
	std::cout
		<< "Scatto_continuo.exe [options...]" << endl
		<< "<options>" << endl
		<< "-h, --help          : print help and exit" << endl
		<< "-v, --version       : print version" << endl
		<< "--opencv-info       : print cv::getBuildInformation()" << endl
		<< endl
		<< "-n   [num :uint64]  : set how many times you want to capture" << endl
		<< "-fps [num : double] : set fps(0.0 - 60.0)." << endl
		<< "-o   [file : string]: name or place the output into [file]" << endl
		<< "  The output image format is chosen based on the filename extension." << endl
		<< "  the following file formats are supported:" << endl
		<< "   *.jpg, *.png, *.pbm, *.pgm, *.ppm, *.bmp" << endl
		<< "  BTIMAP(*.bmp) is deprecated because of disk access speed." << endl
		<< "  To avoid file collisions, $(n) will be replaced."
		<< "  If $(n) does not exist, it will be inserted at the end of the filename."
		<< "  ex.)" << endl
		<< "   - C:\\path\\to\\source\\code\\cropped.jpg" << endl
		<< "     --> C:\\path\\to\\source\\code\\cropped1_01.jpg" << endl
		<< "   - capture$(n)_mine.png --> capture1_01_mine.png" << endl
		<< "   - caputure.jpg --> capture1_01.jpg" << endl
		<< endl
		<< "--thresholding      : threshold image" << endl
		<< "--to_gray_scale     : convert to gray scaled image"
		<< "--disable-algorithm-otu [thresh : double] : disable otu's algorithm and set threshold value(1.0 - 254.0)" << endl
		<< endl
		<< "--pxm-binary        : change pxm(ppm, pgm, pbm) save mode to binary mode(P1-P3)" << endl
		<< "--png-compression [quality : int] : set PNG compression level(0 - 9 default:3)" << endl
		<< "--jpeg-quality    [level : int]   : set JPG quality(0 - 100 default:95)" << endl
		<< endl
		<< "--no-countdown-timer: disable 5 sec. count down timer before starting capture." << endl;

}

PROCESS_CONF commandline_analyzer(int argc, char* argv[]) noexcept(false) {
	using namespace strtonum;
	PROCESS_CONF re = {};
	re.threshold = 0.0;
	re.process_mode = colormode_edit::none;
	re.frame_num = 1;
	re.fps = 4.5;

	std::unordered_map<std::string, std::function<void(const char *)>> cases = {
		{ "-n", [&re](const char* arg) noexcept {
			try {
				re.frame_num = purseUint<uintmax_t>(arg);
			}
			catch (const std::exception&) {
				re.frame_num = 1;
			}
		}},
		{ "-fps", [&re](const char* arg) noexcept {
			try {
				re.fps = purseDouble(arg, 60.0, 0.0);
			}
			catch (const std::exception&) {
				re.fps = 4.5;
			}
		}},
		{ "-o",	[&re](const char* arg) {
			try {
				re.o_file = process_purse_o_option(arg);
				switch (re.o_file.get_pic_type()){
				case picture_type::pbm:
					re.process_mode = colormode_edit::thresholding;
					break;
				case picture_type::pgm:
					re.process_mode = colormode_edit::gray_scaled;
					break;
				default:
					break;
				}
			}
			catch (const std::exception&) {
				re.o_file = filename_c();
			}
		}},
		{ "--png-compression", [&re](const char* arg) noexcept {
			try {
				auto tmp = std::vector<int>(2);
				tmp[0] = cv::IMWRITE_PNG_COMPRESSION;
				tmp[1] = purseInt(arg, 9, 0);//default:3
				re.param = std::move(tmp);
			}
			catch (const std::exception&) {
			}
		}},
		{ "--jpeg-quality", [&re](const char* arg) noexcept {
			try {
				auto tmp = std::vector<int>(2);
				tmp[0] = cv::IMWRITE_JPEG_QUALITY;
				tmp[1] = purseInt(arg, 100, 0);//default:95
				re.param = std::move(tmp);
			}
			catch (const std::exception&) {
			}
		}},
		{ "--disable-algorithm-otu", [&re](const char* arg) noexcept {
			try {
				re.threshold = purseDouble(arg, 254.0, 1.0);
			}
			catch (const std::exception&) {
				re.threshold = 0.0;
			}
		}}
	};
	std::unordered_map<std::string, std::function<void(void)>> cases_no_arg = {
		{ "--thresholding",	[&re]() noexcept {
			if (colormode_edit::none != re.process_mode) re.process_mode = colormode_edit::thresholding;
		}},
		{ "--to_gray_scale", [&re]() noexcept {
			if (colormode_edit::none != re.process_mode) re.process_mode = colormode_edit::gray_scaled;
		}},
		{ "--pxm-binary", [&re]() {
			re.param = std::vector<int>{cv::IMWRITE_PXM_BINARY, 0 };//binary mode(P1-P3)
		}},
		{ "--no-countdown-timer", [&re]() {
			re.no_countdown_timer = true;
		}}
	};

	std::unordered_map<std::string, std::function<void(void)>> case_exist = {
		{ "-v",	[]() noexcept(false) {
			print_version();
			throw successful_termination("option -v");
		}},
		{ "--version", []() noexcept(false) {
			print_version();
			throw successful_termination("option --version");
		}},
		{ "-h",	[]() noexcept(false) {
			print_help();
			throw successful_termination("option -h");
		}},
		{ "--help",	[]() noexcept(false) {
			print_help();
			throw successful_termination("option --help");
		}},
		{ "--opencv-info", []() noexcept(false) {
			std::cout << cv::getBuildInformation() << std::endl;
			throw successful_termination("option --opencv-info");
		}}
	};

	for (int i = 1; i < argc; ++i) {
		if (cases.count(argv[i])) {
			if (i + 1 >= argc) break;
			cases[argv[i]](argv[i + 1]);
			++i;
		}
		else if (cases_no_arg.count(argv[i])) cases_no_arg[argv[i]]();
		else if (case_exist.count(argv[i])) case_exist[argv[i]]();//throw successful_termination
		else {
			print_help();
			throw std::runtime_error("unknown option");
		}
	}
	return re;
}

successful_termination::successful_termination(const std::string & what_arg) : std::runtime_error("successful_termination : " + what_arg) {}

successful_termination::successful_termination(const char * what_arg) : std::runtime_error(std::string("successful_termination : ") + what_arg) {}
