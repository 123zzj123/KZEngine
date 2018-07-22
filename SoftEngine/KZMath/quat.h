#pragma once
#ifndef KZ_QUAT
#define KZ_QUAT

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
		KZQuat(float w = 0.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f);
		KZQuat(float q0, KZVector3D qv);
		//��Ԫ���ӷ�
		KZQuat operator+(const KZQuat& other) const;
		//��Ԫ������
		KZQuat operator-(const KZQuat& other) const;
		//��Ԫ���˷�
		KZQuat operator*(const KZQuat& other) const;
		//���ú���
		void Set(float w = 0.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f);
		//������ת�ᣬ��ת�Ƕȵõ���Ԫ��
		void SetFromVector3DTheta(const KZVector3D& axis, float angle);
		//����ŷ���ǵõ���Ԫ��
		void SetFromEulerZYX(float z, float y, float x);
		//������Ԫ��
		void Conjugate(KZQuat& ans_quat) const;
		//��λ��Ԫ���������ù�����Ԫ��������
		void UnitInverse(KZQuat& ans_quat) const;
		//������Ԫ������
		void Inverse(KZQuat& ans_quat) const;
		//��Ԫ������ƽ��
		float Norm() const;
		//��Ԫ����׼��
		void Normalize();
		//3����Ԫ����ˣ�������ת
		void TripleProduct(const KZQuat& q2, const KZQuat& q3, KZQuat& quat_result) const;
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
	//��ӡ��Ԫ�����ڵ���
	ostream& operator<< (ostream& o, KZQuat& v);
}

#endif // !KZ_QUAT
