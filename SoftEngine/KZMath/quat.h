#pragma once
#ifndef KZ_QUAT
#define KZ_QUAT

#include<cmath>
#include "vector.h"
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
		KZQuat(float w = 0.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f);
		KZQuat(float q0, KZVector3D qv);
		//四元数加法
		KZQuat operator+(const KZQuat& other) const;
		//四元数减法
		KZQuat operator-(const KZQuat& other) const;
		//四元数乘法
		KZQuat operator*(const KZQuat& other) const;
		//设置函数
		inline bool operator == (const KZQuat& other) const  {
			if (w_ == other.w_ && x_ == other.x_ && y_ == other.y_ && z_ == other.z_)
			{
				return true;
			}
			return false;
		}
		inline bool operator != (const KZQuat& other) const {
			if (w_ != other.w_ || x_ != other.x_ || y_ != other.y_ || z_ != other.z_)
			{
				return true;
			}
			return false;
		}
		void Set(float w = 0.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f);
		//根据旋转轴，旋转角度得到四元数
		void SetFromVector3DTheta(const KZVector3D& axis, float angle);
		//根据欧拉角得到四元数
		void SetFromEulerZYX(float z, float y, float x);
		//共轭四元数
		void Conjugate(KZQuat& ans_quat) const;
		//单位四元数倒数利用共轭四元数即倒数
		void UnitInverse(KZQuat& ans_quat) const;
		//任意四元数倒数
		void Inverse(KZQuat& ans_quat) const;
		//四元数范数平方
		float Norm() const;
		//四元数标准化
		void Normalize();
		//3个四元数相乘，用于旋转
		void TripleProduct(const KZQuat& q2, const KZQuat& q3, KZQuat& quat_result) const;

	public:
		static const KZQuat ZERO;
	public:
		union
		{
			float m_[4];
			struct
			{
				float q0_;
				KZVector3D qv_;
			};
			struct
			{
				float w_, x_, y_, z_;
			};
		};
	};
	//打印四元数用于调试
	ostream& operator<< (ostream& o, KZQuat& v);
}

#endif // !KZ_QUAT
