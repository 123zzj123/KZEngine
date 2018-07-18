#pragma once
#ifndef KZVECTOR4D

#define KZVECTOR4D
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
		KZVector4D(float x_in = 0, float y_in = 0, float z_in = 0, float w_in = 1):x(x_in),y(y_in),z(z_in),w(w_in){};
		//���캯��
		KZVector4D(const KZVector4D& v1) :x(v1.x), y(v1.y), z(v1.z), w(v1.w) {};
		//�����ӷ���������
		KZVector4D operator+ (const KZVector4D& other);
		//����������������
		KZVector4D operator- (const KZVector4D& other);
		//�������������
		float operator* (const KZVector4D& other);
		//����������˷���������
		KZVector4D operator* (float num);
		//���������������������
		KZVector4D operator/ (float num);
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
	public:
		float x;
		float y;
		float z;
		float w;
	};
	//��ӡ�������ڵ���
	ostream& operator<< (ostream& o, KZVector4D& v);
	//����������
	void KZVector4DZero(KZVector4D& ans_v);
	//����(1,1,1,1)����
	void KZVector4DSplatOne(KZVector4D& ans_v);
	//���ò�����(x,y,z,1)����
	void KZVector4DSet(KZVector4D& ans_v, float x, float y, float z);
	//���ò�����(s,s,s,1)����
	void KZVector4DReplicate(KZVector4D& ans_v, float s);
	//�����������ȵ�ƽ��
	float KZVector4D3Length(const KZVector4D& v);
	//������������
	float KZVector4D3LengthSq(const KZVector4D& v);
	//�������
	float KZVector4D3Dot(const KZVector4D& v1,const KZVector4D& v2);
	//�������
	void KZVector4D3Cross(KZVector4D& ans_v, const KZVector4D& v1, const KZVector4D& v2);
	//������׼��(��һ��)
	void KZVector4D3Normalize(KZVector4D& v);
	//������������
	void KZVector4D3Orthogonal(KZVector4D& ans_v, const KZVector4D& v);
	//���������н�(�Ƕ�)
	float KZVector4D3AngleBetweenVectors(const KZVector4D& v1, const KZVector4D& v2);
	//����ת�Ƕ�
	float RadianToAngle(float rad);
	//�Ƕ�ת����
	float AngleToRadian(float angle);
};

#endif // !KZVECTOR4D

/*
ToDo
KZVector4D3LengthFast()ʹ��̩�ռ���������������
FastSin()ʹ�ò���̩�ռ�������
*/