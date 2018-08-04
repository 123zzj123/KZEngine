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

	Shader MyShader("MyVShader.vs", "MyFShader.fs");


	float vertices[] = {
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
	};

	unsigned int indices[] = {
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
	uint32_t VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//uint32_t texture;
	//glGenTextures(1, &texture);
	//glBindTexture(GL_TEXTURE_2D, texture);

	////设置贴图参数
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//int width, height, n_chanel;
	//stbi_set_flip_vertically_on_load(true);
	//unsigned char *data = stbi_load("container.jpg", &width, &height, &n_chanel, 0);

	//if (data) {
	//	//创建纹理
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	cout << "Failed to load texture" << endl;
	//}
	//stbi_image_free(data);

	//MyShader.SetInt("texture1", 0);

	while (!glfwWindowShouldClose(window))
	{
		float current_frame = glfwGetTime();
		g_delta_time = current_frame - g_last_frame;
		g_last_frame = current_frame;


		ProcessInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);*/

		MyShader.use();

		glm::mat4 model = glm::mat4(1.0);
		//transform = glm::scale(transform, glm::vec3(0.5, 0.2, 0.3));
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
		//transform = glm::rotate(transform, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		MyShader.SetMat4("model", model);

		glm::mat4 view = g_camera.GetViewMatrix();
		MyShader.SetMat4("view", view);
		glm::mat4 projection = glm::perspective(glm::radians(g_camera.zoom_), (float)g_width / (float)g_height, 1.0f, 20.0f);
		MyShader.SetMat4("projection", projection);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	//g_camera.ProcessMouseScroll(yoffset);
}