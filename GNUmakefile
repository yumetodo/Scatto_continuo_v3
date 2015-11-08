# Makefile for prime_number_generator.exe

TARGET 	= Scatto_continuo_v3.exe
SRCS 	= ./Scatto_continuo_v3/commandline_read.cpp ./Scatto_continuo_v3/convert.cpp ./Scatto_continuo_v3/fileinfo.cpp ./Scatto_continuo_v3/main.cpp ./Scatto_continuo_v3/print_version.cpp ./Scatto_continuo_v3/win32api_wrap.cpp


# 基本コマンド
RM 		:= rm
CXX 	:= g++
CC 		:= gcc

ADDITIONALPOTIONS := 
# デバッグ時とリリース時の微調整
CXX_DEBUG_FLAGS		=	-g -O0 -Wall
CXX_RELEASE_FLAGS	=	-O2

# 基本オプション
CPPFLAGS = -std=c++14 -fvisibility=hidden -msse -msse2 -msse4.1 -msse4.2 -ffast-math -mfpmath=sse 
ifeq ($(CXX),g++)
CPPFLAGS += --input-charset=utf-8
endif
# -fexec-charset=CP932

LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
   CPPFLAGS+= 
else
   CPPFLAGS+= -m32
endif

#OpenCV
OCV_DEBUG_HOME   = ~/ocv$(LBITS)d
OCV_RELEASE_HOME = ~/ocv$(LBITS)

#include
OCV_DEBUG_INC_DIR   = $(OCV_DEBUG_HOME)/include
OCV_RELEASE_INC_DIR = $(OCV_RELEASE_HOME)/include

# lib
CXX_DEBUG_LIB_DIR   = $(OCV_DEBUG_HOME)/lib
CXX_RELEASE_LIB_DIR = $(OCV_RELEASE_HOME)/lib
CXX_COMMON_LIBS  = -lshlwapi -lgdi32
CXX_DEBUG_LIBS   = -lopencv_cored -lopencv_highguid -lopencv_imgprocd -lopencv_objdetectd -lopencv_imgcodecsd 
CXX_RELEASE_LIBS = -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -lopencv_imgcodecs 

# make
# debug
.PHONY: Debug
Debug: CXXFLAGS+=$(CXX_DEBUG_FLAGS) -I"$(OCV_DEBUG_INC_DIR)" -L"$(CXX_DEBUG_LIB_DIR)" $(CXX_DEBUG_LIBS)
Debug: all
# release
.PHONY: Release
Release: CXXFLAGS+=$(CXX_RELEASE_FLAGS) -I"$(OCV_RELEASE_INC_DIR)" -L"$(CXX_DEBUG_LIB_DIR)" $(CXX_DEBUG_LIBS)
Release: all

.PHONY: all
all: $(TARGET)
$(TARGET): $(SRCS)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(CXX_COMMON_LIBS) $(CPPFLAGS) $(ADDITIONALPOTIONS)

# make clean
.PHONY: clean
clean:
	$(RM) -f *.exe