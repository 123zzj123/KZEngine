#include"camera.h"
using namespace KZEngine;

//构造函数
KZCamera::KZCamera(const KZMath::KZVector4D& pos, float fov, float aspect, float near, float far) :n_(KZMath::KZVector4D(0, 0, -1, 0)), movement_speed_(kSpeed), mouse_sensity_(kSensitivity),yaw_(kYaw),pitch_(kPitch) {
	pos_ = pos;
	fov_ = fov;
	aspect_ratio_ = aspect;
	near_clip_z_ = -near;
	far_clip_z_ = -far;
	top_ = near * tan(KZMath::AngleToRadian(fov * 0.5f));
	bottom_ = -top_;
	right_ = top_ * aspect_ratio_;
	left_ = -right_;

	left_clip_plane_.n_ = KZMath::Vector3DTyp(-near_clip_z_, 0.0f, left_);
	left_clip_plane_.p0_ = KZMath::Vector3DTyp(0, 0, 0);
	right_clip_plane_.n_ = KZMath::Vector3DTyp(near_clip_z_, 0.0f, -right_);
	right_clip_plane_.p0_ = KZMath::Vector3DTyp(0, 0, 0);
	top_clip_plane_.n_ = KZMath::Vector3DTyp(0, near_clip_z_, -top_);
	top_clip_plane_.p0_ = KZMath::Vector3DTyp(0, 0, 0);
	bottom_clip_plane_.n_ = KZMath::Vector3DTyp(0, -near_clip_z_, bottom_);
	bottom_clip_plane_.p0_ = KZMath::Vector3DTyp(0, 0, 0);
	UpdateCameraVectors();
}

//获取相机变换矩阵
void KZCamera::GetViewMatrix(KZMath::KZMatrix44& matrix) {
	KZMath::KZVector4D t_vec = -pos_;
	matrix.Identity();
	matrix.m00_ = u_.x_;
	matrix.m01_ = u_.y_;
	matrix.m02_ = u_.z_;
	matrix.m03_ = u_.Vector3Dot(t_vec);
	matrix.m10_ = v_.x_;
	matrix.m11_ = v_.y_;
	matrix.m12_ = v_.z_;
	matrix.m13_ = v_.Vector3Dot(t_vec);
	matrix.m20_ = n_.x_;
	matrix.m21_ = n_.y_;
	matrix.m22_ = n_.z_;
	matrix.m23_ = n_.Vector3Dot(t_vec);
	return;
}

//获取正交矩阵
void KZCamera::GetOrthogonalMatrix(KZMath::KZMatrix44& matrix) {
	matrix.Identity();
	matrix.m00_ = 2 / (right_ - left_);
	matrix.m03_ = -(right_ + left_) / (right_ - left_);
	matrix.m11_ = 2 / (top_ - bottom_);
	matrix.m13_ = -(top_ + bottom_) / (top_ - bottom_);
	matrix.m22_ = 2 / (near_clip_z_ - far_clip_z_);
	matrix.m23_ = (near_clip_z_ + far_clip_z_) / (near_clip_z_ - far_clip_z_);
}

//获取透视投影矩阵
void KZCamera::GetPerspectiveMatrix(KZMath::KZMatrix44& matrix) {
	matrix.Zero();
	matrix.m00_ = - 2 * near_clip_z_ / (right_ - left_);
	matrix.m02_ = (right_ + left_) / (right_ - left_);
	matrix.m11_ = - 2 * near_clip_z_ / (top_ - bottom_);
	matrix.m12_ = (top_ + bottom_) / (top_ - bottom_);
	matrix.m22_ = - (-far_clip_z_ - near_clip_z_) / (-far_clip_z_ + near_clip_z_);
	matrix.m23_ = -2 * far_clip_z_ * near_clip_z_ / (-far_clip_z_ + near_clip_z_);
	matrix.m32_ = -1;
	return;
}

//处理键盘输入
void KZCamera::ProcessKeyboard(CameraMovement direction, float delta_time) {
	float velocity = movement_speed_ * delta_time;
	switch (direction)
	{
	case CameraMovement::FORWARD:
		pos_ += -n_ * velocity;
		break;
	case CameraMovement::BACKWARD:
		pos_ -= -n_ * velocity;
		break;
	case CameraMovement::LEFT:
		pos_ -= u_ * velocity;
		break;
	case CameraMovement::RIGHT:
		pos_ += u_ * velocity;
	default:
		break;
	}
}

//处理鼠标移动
void KZCamera::ProcessMouseMovement(float xoffset, float yoffset, bool constrain) {
	xoffset *= mouse_sensity_;
	yoffset *= mouse_sensity_;

	yaw_ += xoffset;
	pitch_ += yoffset;

	if (constrain) {
		if (pitch_ > 89.0f) {
			pitch_ = 89.0;
		}
		else if(pitch_ < -89.0)
		{
			pitch_ = -89.0;
		}
	}
	UpdateCameraVectors();
}

//处理鼠标缩放
void KZCamera::ProcessMouseScoll(float scroll_offset) {
	//to do about fov
}

//更新uvn
void KZCamera::UpdateCameraVectors() {
	n_.x_ = -cos(KZMath::AngleToRadian(pitch_)) * cos(KZMath::AngleToRadian(yaw_));
	n_.y_ = -sin(KZMath::AngleToRadian(pitch_));
	n_.z_ = -cos(KZMath::AngleToRadian(pitch_)) * sin(KZMath::AngleToRadian(yaw_));
	n_.Vector3Normalize();
	world_up_.Vector3Cross(u_, n_);
	n_.Vector3Cross(v_, u_);
}