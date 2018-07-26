#include"camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : front_(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed_(g_SPEED), mouse_sensity_(g_SENSITIVITY), zoom_(g_ZOOM)
{
	position_ = position;
	world_up_ = up;
	yaw_ = yaw;
	pitch_ = pitch;
	UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : front_(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed_(g_SPEED), mouse_sensity_(g_SENSITIVITY), zoom_(g_ZOOM)
{
	position_ = glm::vec3(posX, posY, posZ);
	world_up_ = glm::vec3(upX, upY, upZ);
	yaw_ = yaw;
	pitch_ = pitch;
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(position_, position_ + front_, up_);
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = movement_speed_ * deltaTime;
	if (direction == FORWARD)
		position_ += front_ * velocity;
	if (direction == BACKWARD)
		position_ -= front_ * velocity;
	if (direction == LEFT)
		position_ -= right_ * velocity;
	if (direction == RIGHT)
		position_ += right_ * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= mouse_sensity_;
	yoffset *= mouse_sensity_;

	yaw_ += xoffset;
	pitch_ += yoffset;

	if (constrainPitch)
	{
		if (pitch_ > 89.0f)
			pitch_ = 89.0f;
		if (pitch_ < -89.0f)
			pitch_ = -89.0f;
	}

	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	if (zoom_ >= 1.0f && zoom_ <= 45.0f)
		zoom_ -= yoffset;
	if (zoom_ <= 1.0f)
		zoom_ = 1.0f;
	if (zoom_ >= 45.0f)
		zoom_ = 45.0f;
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front.y = sin(glm::radians(pitch_));
	front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front_ = glm::normalize(front);
	right_ = glm::normalize(glm::cross(front_, world_up_));
	up_ = glm::normalize(glm::cross(right_, front_));
}