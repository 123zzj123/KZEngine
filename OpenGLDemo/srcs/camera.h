#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include<glad\glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float g_YAW = -90.0f;
const float g_PITCH = 0.0f;
const float g_SPEED = 2.5f;
const float g_SENSITIVITY = 0.1f;
const float g_ZOOM = 45.0;


class Camera
{
public:
	glm::vec3 position_;
	glm::vec3 front_;
	glm::vec3 up_;
	glm::vec3 right_;
	glm::vec3 world_up_;

	float yaw_;
	float pitch_;

	float movement_speed_;
	float mouse_sensity_;
	float zoom_;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = g_YAW, float pitch = g_PITCH);

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	glm::mat4 GetViewMatrix();

	void ProcessKeyboard(CameraMovement direction, float deltaTime);

	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	void ProcessMouseScroll(float yoffset);

private:
	void UpdateCameraVectors();

};

#endif // !CAMERA_H
