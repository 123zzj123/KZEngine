#pragma once
#ifndef KZQUAT
#define KZQUAT

#include<cmath>
#include<iostream>
#include "util.h"

using namespace std;

namespace KZMath {

	/*
	*	including quaternion definition and operation
	*/

	class KZQuat
	{
	public:
		//构造函数
		KZQuat(float w_in = 0.0f, float x_in = 0.0f, float y_in = 0.0f, float z_in = 0.0f);
		KZQuat(float q0_in, KZVector3D qv_in);
		//四元数加法
		KZQuat operator+(const KZQuat& other);
		//四元数减法
		KZQuat operator-(const KZQuat& other);
		//四元数乘法
		KZQuat operator*(const KZQuat& other);
	public:
		union
		{
			float m[4];
			struct
			{
				float q0;
				KZVector3D qv;
			};
			struct
			{
				float w, x, y, z;
			};
		};
	};
	//打印四元数用于调试
	ostream& operator<< (ostream& o, KZQuat& v);
	//根据旋转轴，旋转角度得到四元数
	void Vector3D_Theta_To_QUAT(KZQuat& quat, const KZVector3D& axis, float angle);
	//根据欧拉角得到四元数
	void EulerZYX_To_QUAT(KZQuat& quat, float z, float y, float x);
	//共轭四元数
	void Quat_Conjugate(KZQuat& ans_quat, const KZQuat& ori_quat);
	//单位四元数倒数利用共轭四元数即倒数
	void UnitQuat_Inverse(KZQuat& ans_quat, const KZQuat& ori_quat);
	//任意四元数倒数
	void Quat_Inverse(KZQuat& ans_quat, const KZQuat& ori_quat);
	//四元数范数平方
	float Quat_Norm(const KZQuat& ori_quat);
	//四元数标准化
	void Quat_Normalize(KZQuat& quat);
	//3个四元数相乘，用于旋转
	void Quat_Triple_Product(KZQuat& q1, KZQuat& q2, KZQuat& q3, KZQuat& quat_result);
}

#endif // !KZQUAT
