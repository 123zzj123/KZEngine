#pragma once
#ifndef KZ_MATRIX44
#define KZ_MATRIX44

#include "vector.h"
namespace KZMath {
	/*
	*	including matrix 4*4 definition and operation
	*/

	class KZMatrix44
	{
	public:
		KZMatrix44();
		//构造函数
		KZMatrix44(const KZVector4D& v1, const KZVector4D& v2, const KZVector4D& v3, const KZVector4D& v4);
		KZMatrix44(const float* pArray);
		//矩阵乘法返回矩阵
		KZMatrix44 operator*(const KZMatrix44& nm) const;
		//矩阵与向量乘法
		KZVector4D operator*(const KZVector4D& nv) const;
		//矩阵乘法
		void operator*=(const KZMatrix44& nm);
		//矩阵拷贝
		KZMatrix44& operator=(const KZMatrix44& nm);
		//选取矩阵某个值
		float operator()(uint32_t row, uint32_t col);
		//返回零矩阵
		void Zero();
		//返回单位矩阵
		void Identity();
		//判断矩阵是否单位矩阵
		bool IsIdentity() const;
		//矩阵转置
		void Transpose(KZMatrix44& matrix) const;
		//矩阵行列式
		float Det(uint32_t order) const;
		//矩阵逆
		void Inverse(KZMatrix44& matrix) const;
		//返回伴随矩阵
		void Adjoint(KZMatrix44& matrix) const;
		//返回绕x轴旋转逆矩阵
		void RotationXInverse();
		//返回绕x轴旋转矩阵
		void RotationX(float angle);
		//返回绕y轴旋转逆矩阵
		void RotationYInverse();
		//返回绕y轴旋转矩阵
		void RotationY(float angle);
		//返回绕z轴旋转逆矩阵
		void RotationZInverse();
		//返回绕z轴旋转矩阵
		void RotationZ(float angle);
		//返回绕任意轴旋转矩阵
		void RotationAxis(const KZVector4D& axis, float angle);
		//平移矩阵逆矩阵
		void TranslationInverse();
		//返回平移矩阵(float x,y,z)
		void Translation(float x, float y, float z);
		//返回平移矩阵(vector)
		void Translation(const KZVector4D& offset);
		//缩放矩阵逆矩阵
		void ScalingInverse();
		//返回缩放矩阵(float x,y,z)
		void Scaling(float scale_x, float scale_y, float scale_z);
		//返回缩放矩阵(vector)
		void Scaling(const KZVector4D& scale);
	public:
		union
		{
			float m_[4][4];
			struct
			{
				KZVector4D v1_;
				KZVector4D v2_;
				KZVector4D v3_;
				KZVector4D v4_;
			};
			struct
			{
				float m00_, m01_, m02_, m03_;
				float m10_, m11_, m12_, m13_;
				float m20_, m21_, m22_, m23_;
				float m30_, m31_, m32_, m33_;
			};
		};
	};
	//打印矩阵用于调试
	ostream& operator<< (ostream& o, KZMatrix44& v);
}
#endif