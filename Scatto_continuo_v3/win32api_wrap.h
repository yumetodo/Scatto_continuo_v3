#pragma once
#include <string>
namespace win32api_wrap {
	static std::string get_current_directory();
	bool path_exist(const std::string & path);
	bool file_exist(const std::string & fullpath);
}