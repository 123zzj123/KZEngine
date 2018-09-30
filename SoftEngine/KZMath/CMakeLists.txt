cmake_minimum_required(VERSION 3.1.0)

include_directories("${PROJECT_SOURCE_DIR}/KZMath")

set(SRCS
	util.cpp
	vector.h
	vector.cpp
	quat.cpp
	matrix.cpp
	fix32.cpp
	main.cpp
)

add_library(KZMath ${SRCS})