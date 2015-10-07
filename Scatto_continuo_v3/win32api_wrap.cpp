#include "win32api_wrap.h"
#include <Windows.h>
#include <Shlwapi.h>//need -lShlwapi

namespace win32api_wrap {
	std::string get_current_directory() {
		std::string re(MAX_PATH + 10, '\0');
		const auto len = GetCurrentDirectoryA(re.size() - 1, &re.front());
		re.resize(len);
		return re;
	}
	bool path_exist(const std::string& path) {
		return 0 != PathIsDirectoryA(path.c_str());
	}
	bool file_exist(const std::string& fullpath) {
		return (!fullpath.empty()) && (0 != PathFileExistsA(fullpath.c_str()));
	}
	bool beep(uint32_t nBufferLength, uint32_t lpBuffer){
		return 0 != Beep(nBufferLength, lpBuffer);
	}
	capture_c::capture_c(const BITMAPINFO & bmi) : bmi_(bmi) {
		this->hdc_ = ::CreateCompatibleDC(nullptr);
		this->hbmp_ = CreateDIBSection(nullptr, &this->bmi_, DIB_RGB_COLORS, &this->lpPixel_, nullptr, 0);
		SelectObject(this->hdc_, this->hbmp_);
	}

	capture_c::~capture_c() {
		DeleteObject(this->hbmp_);
		DeleteDC(this->hdc_);
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

	BITMAP capture_c::capture(HWND hwndDesk, const RECT& rc){
		BitBlt(this->hdc_, 0, 0, rc.right, rc.bottom, GetWindowDC(hwndDesk), 0, 0, SRCCOPY);//これがキャプチャー部分
		this->draw_cursor();
		BITMAP  bm;
		GetObject(this->hbmp_, sizeof(BITMAP), &bm);
		return bm;
	}

}
