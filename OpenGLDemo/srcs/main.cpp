#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"stb_image.h"
using namespace std;

const uint32_t g_width = 800;
const uint32_t g_height = 600;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void ProcessInput(GLFWwindow* window);
void LoadImage(const char* file_name);

// camera
Camera g_camera(glm::vec3(0.0f, 1.5f, 3.0f));
float g_last_x = g_width / 2.0f;
float g_last_y = g_height / 2.0f;
bool g_first_mouse = true;

// timing
float g_delta_time = 0.0f;	// time between current frame and last frame
float g_last_frame = 0.0f;


int main() {
	glfwInit();
	//glfw上下文版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//opengl 核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建窗口
	GLFWwindow* window = glfwCreateWindow(g_width, g_height, "Opengl_Demo", NULL, NULL);
	if (!window) {
		cout << "Failed to Create Window" << endl;
		glfwTerminate();
		return 0;
	}

	glfwMakeContextCurrent(window);
	//设置回调
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	//初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to Initial Glad" << endl;
	}

	glEnable(GL_DEPTH_TEST);

	Shader cube_shader("cube_vshader.vs", "cube_fshader.fs");
	Shader pyramid_shader("pyramid_vshader.vs", "pyramid_fshader.fs");

	float cube_vertices[] = {
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, .0f,
		-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, -1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	};

	float pyramid_vertices[] = {
		-1.0, 0.0f, 0.0f, -0.873f, -0.218f, -0.436f,
		1.0f, 0.0f, 0.0f, 0.873f, -0.218f, -0.436f,
		0.0f, 0.0f, 2.0f, 0.0f, -0.0447, 0.894,
		0.0f, 2.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	};

	unsigned int cube_indices[] = {
		0, 2, 1,
		0, 3, 2,
		7, 5, 6,
		7, 4, 5,
		3, 6, 2,
		3, 7, 6,
		4, 1, 5,
		4, 0, 1,
		4, 3, 0,
		4, 7, 3,
		1, 6, 5,
		1, 2, 6
	};

	unsigned int pyramid_indices[] = {
		3, 0, 2,
		3, 2, 1,
		3, 1, 0,
		1, 2, 0,
	};

	//create cube 
	uint32_t cube_vbo, cube_vao, cube_ebo;
	glGenBuffers(1, &cube_vbo);
	glGenVertexArrays(1, &cube_vao);
	glGenBuffers(1, &cube_ebo);

	glBindVertexArray(cube_vao);

	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//create pyramid
	uint32_t pyramid_vbo, pyramid_vao, pyramid_ebo;
	glGenBuffers(1, &pyramid_vbo);
	glGenVertexArrays(1, &pyramid_vao);
	glGenBuffers(1, &pyramid_ebo);

	glBindVertexArray(pyramid_vao);

	glBindBuffer(GL_ARRAY_BUFFER, pyramid_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramid_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramid_indices), pyramid_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//设置贴图参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	LoadImage("container.jpg");

	cube_shader.SetInt("texture1", 0);

	while (!glfwWindowShouldClose(window))
	{
		float current_frame = glfwGetTime();
		g_delta_time = current_frame - g_last_frame;
		g_last_frame = current_frame;


		ProcessInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		cube_shader.Use();

		glm::mat4 cube_model = glm::mat4(1.0f);
		//transform = glm::scale(transform, glm::vec3(0.5, 0.2, 0.3));
		cube_model = glm::translate(cube_model, glm::vec3(0.0f, 0.0f, -5.0f));
		//transform = glm::rotate(transform, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		cube_shader.SetMat4("model", cube_model);

		glm::mat4 view = g_camera.GetViewMatrix();
		cube_shader.SetMat4("view", view);
		glm::mat4 projection = glm::perspective(glm::radians(g_camera.zoom_), (float)g_width / (float)g_height, 1.0f, 20.0f);
		cube_shader.SetMat4("projection", projection);
		glBindVertexArray(cube_vao);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		pyramid_shader.Use();
		glm::vec3 point_light(2.0f, 4.0f, 3.0f);
		pyramid_shader.SetVec3("light.position", point_light);
		pyramid_shader.SetVec3("viewPos", g_camera.position_);

		pyramid_shader.SetVec3("light.ambient", glm::vec3(1.0f,1.0f, 1.0f));
		pyramid_shader.SetVec3("light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		pyramid_shader.SetVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		pyramid_shader.SetVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		pyramid_shader.SetVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
		pyramid_shader.SetVec3("material.specular", glm::vec3(0.3f, 0.5f, 0.4f));
		pyramid_shader.SetFloat("material.shininess", 16.0f);


		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, -7.0f));
		pyramid_shader.SetMat4("model", model);
		pyramid_shader.SetMat4("view", view);
		pyramid_shader.SetMat4("projection", projection);
		glBindVertexArray(pyramid_vao);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &cube_vao);
	glDeleteBuffers(1, &cube_vbo);
	glDeleteBuffers(1, &cube_ebo);

	glfwTerminate();
	return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window) {


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		g_camera.ProcessKeyboard(FORWARD, g_delta_time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		g_camera.ProcessKeyboard(BACKWARD, g_delta_time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		g_camera.ProcessKeyboard(LEFT, g_delta_time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		g_camera.ProcessKeyboard(RIGHT, g_delta_time);


	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (g_first_mouse)
	{
		g_last_x = xpos;
		g_last_y = ypos;
		g_first_mouse = false;
	}

	float xoffset = xpos - g_last_x;
	float yoffset = g_last_y - ypos; // reversed since y-coordinates go from bottom to top

	g_last_x = xpos;
	g_last_y = ypos;

	g_camera.ProcessMouseMovement(xoffset, yoffset);

}

void LoadImage(const char* file_name) {
	int width, height, n_chanel;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(file_name, &width, &height, &n_chanel, 0);

	if (data) {
		//创建纹理
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	//g_camera.ProcessMouseScroll(yoffset);
}