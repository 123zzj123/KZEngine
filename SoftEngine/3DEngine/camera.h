#pragma once
#ifndef KZ_CAMERA
#define KZ_CAMERA
#include"../KZMath/util.h"
#include"../KZMath/vector.h"
#include"../KZMath/matrix.h"

namespace KZEngine {
	const float kYaw = -90.0f;
	const float kPitch = 0.0f;
	const float kSpeed = 2.5f;
	const float kSensitivity = 0.1f;
	const float kZoom = 45.0f;

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
		KZCamera(const KZMath::KZVector4D& pos_in = KZMath::KZVector4D(0, 0, 3), float fov = 45.0f, float aspect = (3.0f/2.0f), float near = 1, float far = 20);
		//��ȡ����任����
		void GetViewMatrix(KZMath::KZMatrix44& matrix);
		//��ȡ͸��ͶӰ����
		void GetPerspectiveMatrix(KZMath::KZMatrix44& matrix);
		//��ȡ��������
		void GetOrthogonalMatrix(KZMath::KZMatrix44& matrix);
		//�����������
		void ProcessKeyboard(CameraMovement direction, float deltaTime);
		//��������ƶ�
		void ProcessMouseMovement(float xoffset, float yoffset, bool constrain = true);
		//�����������
		void ProcessMouseScoll(float scroll_offset);
	public:
		//�����λ��
		KZMath::KZVector4D pos_;
		//�������������
		KZMath::KZVector4D n_;
		//�������������
		KZMath::KZVector4D v_;
		//�������������
		KZMath::KZVector4D u_;
		//����������������
		KZMath::KZVector4D world_up_ = KZMath::KZVector4D(0, 1, 0);
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


		float top_;
		float bottom_;
		float left_;
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
#endif // !KZ_CAMERA
