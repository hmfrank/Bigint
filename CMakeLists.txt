cmake_minimum_required(VERSION 3.6)
project(Bigint2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp
		Bigint.cpp Bigint.h)

add_executable(Bigint2 ${SOURCE_FILES})