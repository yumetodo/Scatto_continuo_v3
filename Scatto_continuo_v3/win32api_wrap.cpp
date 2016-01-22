#include "win32api_wrap.h"
#include <Windows.h>
#include <Shlwapi.h>//need -lShlwapi

namespace win32api_wrap {
	std::string get_current_directory() {
		std::string re(MAX_PATH + 10, '\0');
		const auto len = GetCurrentDirectoryA(static_cast<DWORD>(re.size()) - 1, &re.front());
		re.resize(len);
		return re;
	}
	bool path_exist(const std::string& path) {
		return 0 != PathIsDirectoryA(path.c_str());
	}
	bool file_exist(const std::string& fullpath) {
		return (!fullpath.empty()) && (0 != PathFileExistsA(fullpath.c_str()));
	}
	bool beep(uint32_t Hz, uint32_t mmtime){
		return 0 != Beep(Hz, mmtime);
	}
	HWND get_desktop_window_handle() {
		return GetDesktopWindow();
	}
	static BITMAPINFO make_BITMAPINFO(const RECT& rc) {
		BITMAPINFO bmi = {};
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = rc.right;
		bmi.bmiHeader.biHeight = rc.bottom;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = rc.bottom * ((3 * rc.right + 3) / 4) * 4;
		return bmi;
	}

	capture_c::capture_c(HWND hwnd) : hwnd_(hwnd) {
		this->cap_handle_update(this->hwnd_);
		this->hdc_ = ::CreateCompatibleDC(nullptr);
		this->hbmp_ = CreateDIBSection(nullptr, &this->bmi_, DIB_RGB_COLORS, &this->lpPixel_, nullptr, 0);
		SelectObject(this->hdc_, this->hbmp_);
	}

	capture_c::~capture_c() noexcept {
		DeleteObject(this->hbmp_);
		DeleteDC(this->hdc_);
	}

	void capture_c::cap_handle_update(HWND hwnd){
		RECT rc;
		GetWindowRect(hwnd, &rc);
		this->rc_ = rc;
		this->bmi_ = make_BITMAPINFO(rc);
	}

	void capture_c::draw_cursor(){
		CURSORINFO cursorInfo;
		ICONINFO   iconInfo;

		cursorInfo.cbSize = sizeof(CURSORINFO);
		GetCursorInfo(&cursorInfo);

		GetIconInfo(cursorInfo.hCursor, &iconInfo);

		const int x = cursorInfo.ptScreenPos.x - iconInfo.xHotspot;
		const int y = cursorInfo.ptScreenPos.y - iconInfo.yHotspot;
		DrawIcon(this->hdc_, x, y, cursorInfo.hCursor);
	}

	BITMAP capture_c::capture(){
		BitBlt(this->hdc_, 0, 0, this->rc_.right, this->rc_.bottom, GetWindowDC(this->hwnd_), 0, 0, SRCCOPY);//これがキャプチャー部分
		this->draw_cursor();
		BITMAP  bm;
		GetObject(this->hbmp_, sizeof(BITMAP), &bm);
		return bm;
	}

}
