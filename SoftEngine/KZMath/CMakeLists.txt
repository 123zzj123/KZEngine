cmake_minimum_required(VERSION 3.1.0)

include_directories("${PROJECT_SOURCE_DIR}/KZMath")

set(SRCS
	util.h
	util.cpp
	vector.h
	vector.cpp
	quat.h
	quat.cpp
	matrix.h
	matrix.cpp
	fix32.h
	fix32.cpp
	main.cpp
)

add_library(KZMath ${SRCS})