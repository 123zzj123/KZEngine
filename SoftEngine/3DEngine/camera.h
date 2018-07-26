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
		//构造函数
		KZCamera(const KZMath::KZVector4D& pos_in = KZMath::KZVector4D(0, 0, 3), float fov = 45.0f, float aspect = (3.0f/2.0f), float near = 1, float far = 20);
		//获取相机变换矩阵
		void GetViewMatrix(KZMath::KZMatrix44& matrix);
		//获取透视投影矩阵
		void GetPerspectiveMatrix(KZMath::KZMatrix44& matrix);
		//获取正交矩阵
		void GetOrthogonalMatrix(KZMath::KZMatrix44& matrix);
		//处理键盘输入
		void ProcessKeyboard(CameraMovement direction, float deltaTime);
		//处理鼠标移动
		void ProcessMouseMovement(float xoffset, float yoffset, bool constrain = true);
		//处理鼠标缩放
		void ProcessMouseScoll(float scroll_offset);
	public:
		//摄像机位置
		KZMath::KZVector4D pos_;
		//摄像机方向向量
		KZMath::KZVector4D n_;
		//摄像机向上向量
		KZMath::KZVector4D v_;
		//摄像机向右向量
		KZMath::KZVector4D u_;
		//辅助世界向上向量
		KZMath::KZVector4D world_up_ = KZMath::KZVector4D(0, 1, 0);
		//偏航角
		float yaw_;
		//俯仰角
		float pitch_;
		//摄像机移动速度
		float movement_speed_;
		//鼠标灵敏程度
		float mouse_sensity_;
		//float zoom;

		//近平面
		float near_clip_z_;
		//远平面
		float far_clip_z_;
		//世界角度
		float fov_;
		//宽高比
		float aspect_ratio_;


		float top_;
		float bottom_;
		float left_;
		float right_;

		//右裁剪面
		KZMath::KZPlane3D right_clip_plane_;
		//左裁剪面
		KZMath::KZPlane3D left_clip_plane_;
		//上裁剪面
		KZMath::KZPlane3D top_clip_plane_;
		//下裁剪面
		KZMath::KZPlane3D bottom_clip_plane_;
	private:
		//更新uvn
		void UpdateCameraVectors();
		//
	};
}
#endif // !KZ_CAMERA
