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
		//构造函数
		KZVector4D(float x_in = 0, float y_in = 0, float z_in = 0, float w_in = 1):x(x_in),y(y_in),z(z_in),w(w_in){};
		//构造函数
		KZVector4D(const KZVector4D& v1) :x(v1.x), y(v1.y), z(v1.z), w(v1.w) {};
		//向量加法返回向量
		KZVector4D operator+ (const KZVector4D& other);
		//向量减法返回向量
		KZVector4D operator- (const KZVector4D& other);
		//向量与向量点乘
		float operator* (const KZVector4D& other);
		//向量与标量乘法返回向量
		KZVector4D operator* (float num);
		//向量与标量除法返回向量
		KZVector4D operator/ (float num);
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
	public:
		float x;
		float y;
		float z;
		float w;
	};
	//打印向量用于调试
	ostream& operator<< (ostream& o, KZVector4D& v);
	//创建零向量
	void KZVector4DZero(KZVector4D& ans_v);
	//创建(1,1,1,1)向量
	void KZVector4DSplatOne(KZVector4D& ans_v);
	//设置并返回(x,y,z,1)向量
	void KZVector4DSet(KZVector4D& ans_v, float x, float y, float z);
	//设置并返回(s,s,s,1)向量
	void KZVector4DReplicate(KZVector4D& ans_v, float s);
	//返回向量长度的平方
	float KZVector4D3Length(const KZVector4D& v);
	//返回向量长度
	float KZVector4D3LengthSq(const KZVector4D& v);
	//向量点乘
	float KZVector4D3Dot(const KZVector4D& v1,const KZVector4D& v2);
	//向量叉乘
	void KZVector4D3Cross(KZVector4D& ans_v, const KZVector4D& v1, const KZVector4D& v2);
	//向量标准化(归一化)
	void KZVector4D3Normalize(KZVector4D& v);
	//返回正交向量
	void KZVector4D3Orthogonal(KZVector4D& ans_v, const KZVector4D& v);
	//返回向量夹角(角度)
	float KZVector4D3AngleBetweenVectors(const KZVector4D& v1, const KZVector4D& v2);
	//弧度转角度
	float RadianToAngle(float rad);
	//角度转弧度
	float AngleToRadian(float angle);
};

#endif // !KZVECTOR4D

/*
ToDo
KZVector4D3LengthFast()使用泰勒级数计算向量长度
FastSin()使用查表和泰勒级数计算
*/