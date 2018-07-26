cmake_minimum_required(VERSION 3.1.0)

include_directories("${PROJECT_SOURCE_DIR}/srcs")
include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_SOURCE_DIR}/lib")

set(SRCS 
	glad.c
	Shader.cpp
	stb_image.cpp
	camera.cpp
	main.cpp
	MyFShader.fs
	MyVShader.vs
)

add_executable(CG_OpenglDemo ${SRCS})
target_link_libraries(CG_OpenglDemo glfw3.lib)
target_link_libraries(CG_OpenglDemo opengl32.lib)