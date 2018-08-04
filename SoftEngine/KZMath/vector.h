#pragma once
#ifndef KZ_VECTOR4D

#define KZ_VECTOR4D
#define _USE_MATH_DEFINES

#include<cmath>
#include<iostream>
using namespace std;

namespace KZMath {
	/*
	*	including vector4d definition and operation
	*/
	class KZVector4D
	{
	public:
		//���캯��
		explicit KZVector4D(float x = 0, float y = 0, float z = 0, float w = 1):x_(x),y_(y),z_(z),w_(w){};
		//�����ӷ���������
		KZVector4D operator+ (const KZVector4D& other) const;
		//����������������
		KZVector4D operator- (const KZVector4D& other) const;
		//�������������
		float operator* (const KZVector4D& other) const;
		//����������˷���������
		KZVector4D operator* (float num) const;
		//���������������������
		KZVector4D operator/ (float num) const;
		//������ֵ
		KZVector4D& operator= (const KZVector4D& v1);
		//���ظ�������
		KZVector4D operator- () const;
		//�����ӷ�
		void operator+= (const KZVector4D& other);
		//��������
		void operator-= (const KZVector4D& other);
		//����������˷�(vector)
		void operator*= (const KZVector4D& other);
		//����������˷�(float)
		void operator*= (float num);
		//�������������
		void operator/= (float num);
		//�ж�����������
		bool operator== (const KZVector4D& v);
		//�ж������������
		bool operator!= (const KZVector4D& v);
		//����������
		void Zero();
		//����(1,1,1,1)����
		void SplatOne();
		//���ò�����(x,y,z,1)����
		void Set(float x, float y, float z);
		//���ò�����(s,s,s,1)����
		void Replicate(float s);
		//�����������ȵ�ƽ��
		float Vector3Length() const;
		//������������
		float Vector3LengthSq() const;
		//�������
		float Vector3Dot(const KZVector4D& v2) const;
		//�������
		void Vector3Cross(KZVector4D& ans_v, const KZVector4D& v2) const;
		//������׼��(��һ��)
		void Vector3Normalize();
		//������������
		void Vector3Orthogonal(KZVector4D& ans_v) const;
		//���������н�(�Ƕ�)
		float Vector3AngleBetweenVectors(const KZVector4D& v2) const;
	public:
		float x_;
		float y_;
		float z_;
		float w_;
	};
	//��ӡ�������ڵ���
	ostream& operator<< (ostream& o, KZVector4D& v);
	//����ת�Ƕ�
	float RadianToAngle(float rad);
	//�Ƕ�ת����
	float AngleToRadian(float angle);
};

#endif // !KZ_VECTOR4D

/*
ToDo
KZVector4D3LengthFast()ʹ��̩�ռ���������������
FastSin()ʹ�ò���̩�ռ�������
*/