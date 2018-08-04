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
		//构造函数
		explicit KZVector4D(float x = 0, float y = 0, float z = 0, float w = 1):x_(x),y_(y),z_(z),w_(w){};
		//向量加法返回向量
		KZVector4D operator+ (const KZVector4D& other) const;
		//向量减法返回向量
		KZVector4D operator- (const KZVector4D& other) const;
		//向量与向量点乘
		float operator* (const KZVector4D& other) const;
		//向量与标量乘法返回向量
		KZVector4D operator* (float num) const;
		//向量与标量除法返回向量
		KZVector4D operator/ (float num) const;
		//向量赋值
		KZVector4D& operator= (const KZVector4D& v1);
		//返回负的向量
		KZVector4D operator- () const;
		//向量加法
		void operator+= (const KZVector4D& other);
		//向量减法
		void operator-= (const KZVector4D& other);
		//向量与标量乘法(vector)
		void operator*= (const KZVector4D& other);
		//向量与标量乘法(float)
		void operator*= (float num);
		//向量与标量除法
		void operator/= (float num);
		//判断向量相等与否
		bool operator== (const KZVector4D& v);
		//判断向量不等与否
		bool operator!= (const KZVector4D& v);
		//创建零向量
		void Zero();
		//创建(1,1,1,1)向量
		void SplatOne();
		//设置并返回(x,y,z,1)向量
		void Set(float x, float y, float z);
		//设置并返回(s,s,s,1)向量
		void Replicate(float s);
		//返回向量长度的平方
		float Vector3Length() const;
		//返回向量长度
		float Vector3LengthSq() const;
		//向量点乘
		float Vector3Dot(const KZVector4D& v2) const;
		//向量叉乘
		void Vector3Cross(KZVector4D& ans_v, const KZVector4D& v2) const;
		//向量标准化(归一化)
		void Vector3Normalize();
		//返回正交向量
		void Vector3Orthogonal(KZVector4D& ans_v) const;
		//返回向量夹角(角度)
		float Vector3AngleBetweenVectors(const KZVector4D& v2) const;
	public:
		float x_;
		float y_;
		float z_;
		float w_;
	};
	//打印向量用于调试
	ostream& operator<< (ostream& o, KZVector4D& v);
	//弧度转角度
	float RadianToAngle(float rad);
	//角度转弧度
	float AngleToRadian(float angle);
};

#endif // !KZ_VECTOR4D

/*
ToDo
KZVector4D3LengthFast()使用泰勒级数计算向量长度
FastSin()使用查表和泰勒级数计算
*/