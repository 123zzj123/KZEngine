#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include"../KZMath/util.h"
#include"../KZMath/vector.h"
#include"../KZMath/matrix.h"

namespace KZEngine {
	const float kYaw = -90.0f;
	const float kPitch = 0.0f;
	const float kSpeed = 2.5f;
	const float kSensitivity = 0.1f;
	const float kZoom = 45.0f;

	//������ƶ�
	enum CameraMovement
	{
		FORWARD,
		BACKWARD,
		RIGHT,
		LEFT,
	};

	class KZCamera
	{
	public:
		//���캯��
		KZCamera(const KZMath::KZVector4D<float>& pos_in = KZMath::KZVector4D<float>(0, 0, 3), float fov = 45.0f, float aspect = (3.0f/2.0f), float near = 1.0f, float far = 30.0f);
		//��ȡ����任����
		void GetViewMatrix(KZMath::KZMatrix44& matrix);
		//��ȡ͸��ͶӰ����
		void GetPerspectiveMatrix(KZMath::KZMatrix44& matrix);
		//��ȡ��������
		void GetOrthogonalMatrix(KZMath::KZMatrix44& matrix);
		//�����������
		void ProcessKeyboard(CameraMovement direction, float delta_time);
		//��������ƶ�
		void ProcessMouseMovement(float xoffset, float yoffset, bool constrain = true);
		//�����������
		void ProcessMouseScoll(float scroll_offset);
		//���������λ��
		inline KZMath::KZVector4D<float>& GetCameraPos() {
			return pos_;
		}
		//��ȡ�����Զ����
		inline float GetCameraFarClip() {
			return far_clip_z_;
		}
		//��ȡ�����������
		inline float GetCameraNearClip() {
			return near_clip_z_;
		}
		//��ȡͶӰ��top
		inline float GetViewTop() {
			return top_;
		}
		//��ȡͶӰ��right
		inline float GetViewRight() {
			return right_;
		}
		inline float GetParameterA() {
			return -(-far_clip_z_ - near_clip_z_) / (-far_clip_z_ + near_clip_z_);
		}
		inline float GetParameterB() {
			return  -2 * far_clip_z_ * near_clip_z_ / (-far_clip_z_ + near_clip_z_);
		}
	private:
		//�����λ��
		KZMath::KZVector4D<float> pos_;
		//�������������
		KZMath::KZVector4D<float> n_;
		//�������������
		KZMath::KZVector4D<float> v_;
		//�������������
		KZMath::KZVector4D<float> u_;
		//����������������
		KZMath::KZVector4D<float> world_up_ = KZMath::KZVector4D<float>(0, 1, 0);
		//ƫ����
		float yaw_;
		//������
		float pitch_;
		//������ƶ��ٶ�
		float movement_speed_;
		//��������̶�
		float mouse_sensity_;
		//float zoom;

		//��ƽ��
		float near_clip_z_;
		//Զƽ��
		float far_clip_z_;
		//����Ƕ�
		float fov_;
		//��߱�
		float aspect_ratio_;

		//ͶӰ��top
		float top_;
		//ͶӰ��bottom
		float bottom_;
		//ͶӰ��left
		float left_;
		//ͶӰ��right
		float right_;

		//�Ҳü���
		KZMath::KZPlane3D right_clip_plane_;
		//��ü���
		KZMath::KZPlane3D left_clip_plane_;
		//�ϲü���
		KZMath::KZPlane3D top_clip_plane_;
		//�²ü���
		KZMath::KZPlane3D bottom_clip_plane_;
	private:
		//����uvn
		void UpdateCameraVectors();
		//
	};
}
#endif // !CAMERA_H
