cmake_minimum_required(VERSION 3.1.0)

include_directories("${PROJECT_SOURCE_DIR}/3DEngine")

set(SRCS
	camera.cpp
	image.cpp
	light.cpp
	material.cpp
	object.cpp
	pipeline.cpp
	renderlist.cpp
	test.cpp
)

add_executable(KZEngine ${SRCS})
target_link_libraries(KZEngine debug ${PROJECT_BINARY_DIR}/KZMath/Debug/KZMath.lib)
target_link_libraries(KZEngine optimized ${PROJECT_BINARY_DIR}/KZMath/Release/KZMath.lib)