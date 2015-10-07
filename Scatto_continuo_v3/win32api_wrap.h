#pragma once
#include <string>
#include <cstdint>
#include <wtypes.h>
#include <wingdi.h>
namespace win32api_wrap {
	std::string get_current_directory();
	bool path_exist(const std::string & path);
	bool file_exist(const std::string & fullpath);
	bool beep(uint32_t nBufferLength, uint32_t lpBuffer);
	class capture_c
	{
	public:
		explicit capture_c(const BITMAPINFO& bmi);
		~capture_c();
		BITMAP capture(HWND hwndDesk, const RECT& rc);
	private:
		void draw_cursor();
		BITMAPINFO bmi_;
		HDC hdc_;
		HBITMAP hbmp_;
		void* lpPixel_;
	};
}