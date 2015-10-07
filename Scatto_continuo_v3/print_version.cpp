#include <iostream>
#include <opencv2/core/version.hpp>
void print_version() {
	using std::endl;
	std::cout
		<< "OpenCV Version : " << CV_VERSION << endl
#if defined (_MSC_VER)
		<< "built on Visual Studio. _MSC_FULL_VER=" << _MSC_FULL_VER << endl
#elif defined(__clang__)
		<< "built on LLVM Clang++. version=" << __clang_major__ << '.' << __clang_minor__ << '.' << __clang_patchlevel__ << endl
#elif defined (__ICC)
		<< "built on Intel ICC. version=" << __ICC << endl
#elif defined (__GNUC__)
		<< "built on GNU G++. version=" << __GNUC__ << '.' << __GNUC_MINOR__ << '.' << __GNUC_PATCHLEVEL__
#else 
		<< "built on unknown compiler." << endl
#endif
		<< endl
		<< "Scatto_continuo.exe version 3.0.0 build" << __DATE__ << " " << __TIME__ << endl;
}
