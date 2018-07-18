#pragma once
#ifndef KZMATRIX44
#define KZMATRIX44

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
		KZMatrix44(KZVector4D& v1, KZVector4D& v2, KZVector4D& v3, KZVector4D& v4);
		KZMatrix44(const float* pArray);
		//矩阵乘法返回矩阵
		KZMatrix44 operator*(const KZMatrix44& nm);
		//矩阵乘法
		void operator*=(const KZMatrix44& nm);
		//矩阵拷贝
		KZMatrix44& operator=(const KZMatrix44& nm);
		//选取矩阵某个值
		float operator()(uint32_t row, uint32_t col);
	public:
		union
		{
			float m[4][4];

			struct
			{
				float m00, m01, m02, m03;
				float m10, m11, m12, m13;
				float m20, m21, m22, m23;
				float m30, m31, m32, m33;
			};
		};
	};
	//打印矩阵用于调试
	ostream& operator<< (ostream& o, KZMatrix44& v);
	//返回单位矩阵
	void KZMatrix44Identity(KZMatrix44& matrix);
	//判断矩阵是否单位矩阵
	bool KZMatrix44IsIdentity(const KZMatrix44& matrix);
	//矩阵转置
	void KZMATRIX44Transpose(KZMatrix44& matrix, const KZMatrix44& ori_matrix);
	//矩阵行列式
	float KZMatrix44Det(const KZMatrix44& matrix, uint32_t order);
	//矩阵逆
	void KZMatrix44Inverse(KZMatrix44& matrix, const KZMatrix44& ori_matrix);
	//返回伴随矩阵
	void KZMatrix44Adjoint(KZMatrix44& matrix, const KZMatrix44& ori_matrix);
	//返回绕x轴旋转逆矩阵
	void KZMatrix44RotationXInverse(KZMatrix44& matrix);
	//返回绕x轴旋转矩阵
	void KZMatrix44RotationX(KZMatrix44& matrix, float angle);
	//返回绕y轴旋转逆矩阵
	void KZMatrix44RotationYInverse(KZMatrix44& matrix);
	//返回绕y轴旋转矩阵
	void KZMatrix44RotationY(KZMatrix44& matrix, float angle);
	//返回绕z轴旋转逆矩阵
	void KZMatrix44RotationZInverse(KZMatrix44& matrix);
	//返回绕z轴旋转矩阵
	void KZMatrix44RotationZ(KZMatrix44& matrix, float angle);
	//返回绕任意轴旋转矩阵
	void KZMatrix44RotationAxis(KZMatrix44& matrix, KZVector4D axis, float angle);
	//平移矩阵逆矩阵
	void KZMatrix44TranslationInverse(KZMatrix44& matrix);
	//返回平移矩阵(float x,y,z)
	void KZMatrix44Translation(KZMatrix44& matrix, float x, float y, float z);
	//返回平移矩阵(vector)
	void KZMatrix44Translation(KZMatrix44& matrix, KZVector4D offset);
	//缩放矩阵逆矩阵
	void KZMatrix44ScalingInverse(KZMatrix44& matrix);
	//返回缩放矩阵(float x,y,z)
	void KZMatrix44Scaling(KZMatrix44& matrix, float scale_x, float scale_y, float scale_z);
	//返回缩放矩阵(vector)
	void KZMatrix44Scaling(KZMatrix44& matrix, KZVector4D scale);
}
#endif