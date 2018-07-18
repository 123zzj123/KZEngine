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
		//���캯��
		KZMatrix44(KZVector4D& v1, KZVector4D& v2, KZVector4D& v3, KZVector4D& v4);
		KZMatrix44(const float* pArray);
		//����˷����ؾ���
		KZMatrix44 operator*(const KZMatrix44& nm);
		//����˷�
		void operator*=(const KZMatrix44& nm);
		//���󿽱�
		KZMatrix44& operator=(const KZMatrix44& nm);
		//ѡȡ����ĳ��ֵ
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
	//��ӡ�������ڵ���
	ostream& operator<< (ostream& o, KZMatrix44& v);
	//���ص�λ����
	void KZMatrix44Identity(KZMatrix44& matrix);
	//�жϾ����Ƿ�λ����
	bool KZMatrix44IsIdentity(const KZMatrix44& matrix);
	//����ת��
	void KZMATRIX44Transpose(KZMatrix44& matrix, const KZMatrix44& ori_matrix);
	//��������ʽ
	float KZMatrix44Det(const KZMatrix44& matrix, uint32_t order);
	//������
	void KZMatrix44Inverse(KZMatrix44& matrix, const KZMatrix44& ori_matrix);
	//���ذ������
	void KZMatrix44Adjoint(KZMatrix44& matrix, const KZMatrix44& ori_matrix);
	//������x����ת�����
	void KZMatrix44RotationXInverse(KZMatrix44& matrix);
	//������x����ת����
	void KZMatrix44RotationX(KZMatrix44& matrix, float angle);
	//������y����ת�����
	void KZMatrix44RotationYInverse(KZMatrix44& matrix);
	//������y����ת����
	void KZMatrix44RotationY(KZMatrix44& matrix, float angle);
	//������z����ת�����
	void KZMatrix44RotationZInverse(KZMatrix44& matrix);
	//������z����ת����
	void KZMatrix44RotationZ(KZMatrix44& matrix, float angle);
	//��������������ת����
	void KZMatrix44RotationAxis(KZMatrix44& matrix, KZVector4D axis, float angle);
	//ƽ�ƾ��������
	void KZMatrix44TranslationInverse(KZMatrix44& matrix);
	//����ƽ�ƾ���(float x,y,z)
	void KZMatrix44Translation(KZMatrix44& matrix, float x, float y, float z);
	//����ƽ�ƾ���(vector)
	void KZMatrix44Translation(KZMatrix44& matrix, KZVector4D offset);
	//���ž��������
	void KZMatrix44ScalingInverse(KZMatrix44& matrix);
	//�������ž���(float x,y,z)
	void KZMatrix44Scaling(KZMatrix44& matrix, float scale_x, float scale_y, float scale_z);
	//�������ž���(vector)
	void KZMatrix44Scaling(KZMatrix44& matrix, KZVector4D scale);
}
#endif