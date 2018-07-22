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
		//���캯��
		KZMatrix44(const KZVector4D& v1, const KZVector4D& v2, const KZVector4D& v3, const KZVector4D& v4);
		KZMatrix44(const float* pArray);
		//����˷����ؾ���
		KZMatrix44 operator*(const KZMatrix44& nm) const;
		//�����������˷�
		KZVector4D operator*(const KZVector4D& nv) const;
		//����˷�
		void operator*=(const KZMatrix44& nm);
		//���󿽱�
		KZMatrix44& operator=(const KZMatrix44& nm);
		//ѡȡ����ĳ��ֵ
		float operator()(uint32_t row, uint32_t col);
		//���������
		void Zero();
		//���ص�λ����
		void Identity();
		//�жϾ����Ƿ�λ����
		bool IsIdentity() const;
		//����ת��
		void Transpose(KZMatrix44& matrix) const;
		//��������ʽ
		float Det(uint32_t order) const;
		//������
		void Inverse(KZMatrix44& matrix) const;
		//���ذ������
		void Adjoint(KZMatrix44& matrix) const;
		//������x����ת�����
		void RotationXInverse();
		//������x����ת����
		void RotationX(float angle);
		//������y����ת�����
		void RotationYInverse();
		//������y����ת����
		void RotationY(float angle);
		//������z����ת�����
		void RotationZInverse();
		//������z����ת����
		void RotationZ(float angle);
		//��������������ת����
		void RotationAxis(const KZVector4D& axis, float angle);
		//ƽ�ƾ��������
		void TranslationInverse();
		//����ƽ�ƾ���(float x,y,z)
		void Translation(float x, float y, float z);
		//����ƽ�ƾ���(vector)
		void Translation(const KZVector4D& offset);
		//���ž��������
		void ScalingInverse();
		//�������ž���(float x,y,z)
		void Scaling(float scale_x, float scale_y, float scale_z);
		//�������ž���(vector)
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
	//��ӡ�������ڵ���
	ostream& operator<< (ostream& o, KZMatrix44& v);
}
#endif