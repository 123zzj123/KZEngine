cmake_minimum_required(VERSION 3.1.0)

include_directories("${PROJECT_SOURCE_DIR}/3DEngine")

set(SRCS
	camera.h
	camera.cpp
	image.h
	image.cpp
	light.h
	light.cpp
	material.h
	material.cpp
	mesh.h
	mesh.cpp
	object.h
	object.cpp
	pipeline.h
	pipeline.cpp
	renderlist.h
	renderlist.cpp
	terrian.h
	terrian.cpp
	bsp.h
	bsp.cpp
	bhv.h
	bhv.cpp
	pass.h
	pass.cpp
	test.cpp
)

add_executable(KZEngine ${SRCS})
target_link_libraries(KZEngine debug ${PROJECT_BINARY_DIR}/KZMath/Debug/KZMath.lib)
target_link_libraries(KZEngine optimized ${PROJECT_BINARY_DIR}/KZMath/Release/KZMath.lib)