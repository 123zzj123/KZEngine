cmake_minimum_required(VERSION 3.1.0)

include_directories("${PROJECT_SOURCE_DIR}/KZMath")

set(SRCS
	util.cpp
	vector.cpp
	quat.cpp
	matrix.cpp
	main.cpp
)

add_executable(KZMath ${SRCS})