#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <string>
#include <cstdint>
#include <wtypes.h>
#include <wingdi.h>
namespace win32api_wrap {
	std::string get_current_directory();
	bool path_exist(const std::string & path);
	bool file_exist(const std::string & fullpath);
	bool beep(uint32_t Hz, uint32_t mmtime);
	HWND get_desktop_window_handle();
	class capture_c
	{
	public:
		explicit capture_c(HWND hwnd);
		~capture_c() noexcept;
		BITMAP capture();
		void cap_handle_update(HWND hwnd);
	private:
		void draw_cursor();
		BITMAPINFO bmi_;
		HDC hdc_;
		HBITMAP hbmp_;
		HWND hwnd_;
		RECT rc_;
		void* lpPixel_;
	};
}