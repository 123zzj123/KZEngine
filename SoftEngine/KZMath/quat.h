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
		//���캯��
		KZQuat(float w_in = 0.0f, float x_in = 0.0f, float y_in = 0.0f, float z_in = 0.0f);
		KZQuat(float q0_in, KZVector3D qv_in);
		//��Ԫ���ӷ�
		KZQuat operator+(const KZQuat& other);
		//��Ԫ������
		KZQuat operator-(const KZQuat& other);
		//��Ԫ���˷�
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
	//��ӡ��Ԫ�����ڵ���
	ostream& operator<< (ostream& o, KZQuat& v);
	//������ת�ᣬ��ת�Ƕȵõ���Ԫ��
	void Vector3D_Theta_To_QUAT(KZQuat& quat, const KZVector3D& axis, float angle);
	//����ŷ���ǵõ���Ԫ��
	void EulerZYX_To_QUAT(KZQuat& quat, float z, float y, float x);
	//������Ԫ��
	void Quat_Conjugate(KZQuat& ans_quat, const KZQuat& ori_quat);
	//��λ��Ԫ���������ù�����Ԫ��������
	void UnitQuat_Inverse(KZQuat& ans_quat, const KZQuat& ori_quat);
	//������Ԫ������
	void Quat_Inverse(KZQuat& ans_quat, const KZQuat& ori_quat);
	//��Ԫ������ƽ��
	float Quat_Norm(const KZQuat& ori_quat);
	//��Ԫ����׼��
	void Quat_Normalize(KZQuat& quat);
	//3����Ԫ����ˣ�������ת
	void Quat_Triple_Product(KZQuat& q1, KZQuat& q2, KZQuat& q3, KZQuat& quat_result);
}

#endif // !KZQUAT
