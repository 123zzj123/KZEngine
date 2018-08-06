cmake_minimum_required(VERSION 3.1.0)

include_directories("${PROJECT_SOURCE_DIR}/srcs")
include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_SOURCE_DIR}/lib")

set(SRCS 
	glad.c
	shader.cpp
	stb_image.cpp
	camera.cpp
	main.cpp
	cube_fshader.fs
	cube_vshader.vs
	pyramid_fshader.fs
	pyramid_vshader.vs
)

add_executable(CG_OpenglDemo ${SRCS})
target_link_libraries(CG_OpenglDemo glfw3.lib)
target_link_libraries(CG_OpenglDemo opengl32.lib)