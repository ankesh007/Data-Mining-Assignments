CC = g++
CXXFLAGS = -std=c++11 -I include/

BUILD_TYPE = test

ifeq ($(BUILD_TYPE), debug)
	CXXFLAGS += -g
else
	CXXFLAGS += -O3
endif

all : src/*.cpp
	$(CC) src/*.cpp $(CXXFLAGS) -o fpm

clean : 
	rm -f fpm *.txt callgrind*
