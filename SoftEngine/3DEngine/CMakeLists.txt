cmake_minimum_required(VERSION 3.1.0)

include_directories("${PROJECT_SOURCE_DIR}/3DEngine")
link_directories("${PROJECT_BINARY_DIR}/KZMath/Debug")

set(SRCS
	camera.cpp
	object.cpp
	pipeline.cpp
	renderlist.cpp
	test.cpp
)

add_executable(KZEngine ${SRCS})
target_link_libraries(KZEngine KZMath.lib)