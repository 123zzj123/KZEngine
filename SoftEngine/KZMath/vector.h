#pragma once
#ifndef KZ_VECTOR4D

#define KZ_VECTOR4D

#include<cmath>
#include<iostream>
using namespace std;

namespace KZMath {
	/*
	*	including vector4d definition and operation
	*/
	template <typename T>
	class KZVector4D
	{
	public:
		//构造函数
		explicit KZVector4D(T x = 0, T y = 0, T z = 0, T w = 1):x_(x),y_(y),z_(z),w_(w){};
		//向量加法返回向量
		KZVector4D<T> operator+ (const KZVector4D<T>& other) const;
		//向量减法返回向量
		KZVector4D<T> operator- (const KZVector4D<T>& other) const;
		//向量与向量点乘
		T operator* (const KZVector4D<T>& other) const;
		//向量与标量乘法返回向量
		KZVector4D<T> operator* (T num) const;
		//向量与标量除法返回向量
		KZVector4D<T> operator/ (T num) const;
		//向量赋值
		KZVector4D<T>& operator= (const KZVector4D<T>& v1);
		//返回负的向量
		KZVector4D<T> operator- () const;
		//向量加法
		void operator+= (const KZVector4D<T>& other);
		//向量减法
		void operator-= (const KZVector4D<T>& other);
		//向量与标量乘法(vector)
		void operator*= (const KZVector4D<T>& other);
		//向量与标量乘法(float)
		void operator*= (T num);
		//向量与标量除法
		void operator/= (T num);
		//判断向量相等与否
		bool operator== (const KZVector4D<T>& v) const;
		//判断向量不等与否
		bool operator!= (const KZVector4D<T>& v) const;
		//创建零向量
		void Zero();
		//创建(1,1,1,1)向量
		void SplatOne();
		//设置并返回(x,y,z,1)向量
		void Set(T x, T y, T z);
		//设置并返回(s,s,s,1)向量
		void Replicate(T s);
		//返回向量长度的平方
		T Vector3Length() const;
		//返回向量长度
		float Vector3LengthSq() const;
		//向量点乘
		T Vector3Dot(const KZVector4D<T>& v2) const;
		//向量叉乘
		void Vector3Cross(KZVector4D<T>& ans_v, const KZVector4D<T>& v2) const;
		//向量标准化(归一化)
		void Vector3Normalize();
		//返回正交向量
		void Vector3Orthogonal(KZVector4D<T>& ans_v) const;
		//返回向量夹角(角度)
		float Vector3AngleBetweenVectors(const KZVector4D<T>& v2) const;
	public:
		T x_;
		T y_;
		T z_;
		T w_;
	};
	//打印向量用于调试
	template<typename T>
	ostream& operator<< (ostream& o, KZVector4D<T>& v);
	//弧度转角度
	float RadianToAngle(float rad);
	//角度转弧度
	float AngleToRadian(float angle);
};

namespace KZMath {
	//向量赋值
	template<typename T>
	inline KZVector4D<T>& KZVector4D<T>::operator= (const KZVector4D<T>& v1) {
		x_ = v1.x_;
		y_ = v1.y_;
		z_ = v1.z_;
		w_ = v1.w_;
		return *this;
	}

	//返回负的向量
	template<typename T>
	inline KZVector4D<T> KZVector4D<T>::operator- () const {
		return KZVector4D<T>(-x_, -y_, -z_);
	}

	//向量加法返回向量
	template<typename T>
	inline KZVector4D<T> KZVector4D<T>::operator+ (const KZVector4D<T>& other) const {
		KZVector4D<T> v;
		v.x_ = x_ + other.x_;
		v.y_ = y_ + other.y_;
		v.z_ = z_ + other.z_;
		return v;
	}

	//向量减法返回向量
	template<typename T>
	inline KZVector4D<T> KZVector4D<T>::operator- (const KZVector4D<T>& other) const {
		KZVector4D<T> v;
		v.x_ = x_ - other.x_;
		v.y_ = y_ - other.y_;
		v.z_ = z_ - other.z_;
		return v;
	}

	//向量与向量乘法
	template<typename T>
	inline T KZVector4D<T>::operator* (const KZVector4D<T>& other) const {
		KZVector4D<T> v;
		v.x_ = x_ * other.x_;
		v.y_ = y_ * other.y_;
		v.z_ = z_ * other.z_;
		v.w_ = w_ * other.w_;
		return v.x_ + v.y_ + v.z_ + v.w_;
	}

	//向量与标量除法返回向量
	template<typename T>
	inline KZVector4D<T> KZVector4D<T>::operator/ (T num) const {
		KZVector4D<T> v;
		T one_over_num = 1 / num;
		v.x_ = x_ * one_over_num;
		v.y_ = y_ * one_over_num;
		v.z_ = z_ * one_over_num;
		return v;
	}

	//向量与标量乘法返回向量
	template<typename T>
	inline KZVector4D<T> KZVector4D<T>::operator* (T num) const {
		KZVector4D v;
		v.x_ = x_ * num;
		v.y_ = y_ * num;
		v.z_ = z_ * num;
		return v;
	}

	//向量加法
	template<typename T>
	inline void KZVector4D<T>::operator+= (const KZVector4D<T>& other) {
		x_ += other.x_;
		y_ += other.y_;
		z_ += other.z_;
		return;
	}

	//向量减法
	template<typename T>
	inline void KZVector4D<T>::operator-= (const KZVector4D<T>& other) {
		x_ -= other.x_;
		y_ -= other.y_;
		z_ -= other.z_;
		return;
	}

	//向量与标量乘法(vector)
	template<typename T>
	inline void KZVector4D<T>::operator*= (const KZVector4D<T>& other) {
		x_ *= other.x_;
		y_ *= other.y_;
		z_ *= other.z_;
		return;
	}

	//向量与标量乘法(float)
	template<typename T>
	inline void KZVector4D<T>::operator*= (T num) {
		x_ *= num;
		y_ *= num;
		z_ *= num;
		return;
	}

	//向量与标量除法
	template<typename T>
	inline void KZVector4D<T>::operator/= (T num) {
		T one_over_num = 1 / num;
		x_ *= one_over_num;
		y_ *= one_over_num;
		z_ *= one_over_num;
		return;
	}

	//判断向量相等与否
	template<typename T>
	inline bool KZVector4D<T>::operator== (const KZVector4D<T>& v) const {
		return x_ == v.x_ && y_ == v.y_ && z_ == v.z_;
	}

	//判断向量不等与否
	template<typename T>
	inline bool KZVector4D<T>::operator!=(const KZVector4D<T>& v) const {
		return x_ != v.x_ || y_ != v.y_ || z_ != v.z_;
	}

	//打印向量用于调试
	template<typename T>
	inline ostream& KZMath::operator << (ostream & o, KZVector4D<T> & v)
	{
		o << v.x_ << " " << v.y_ << " " << v.z_ << " " << v.w_;
		return o;
	}

	//创建零向量
	template<typename T>
	inline void KZVector4D<T>::Zero() {
		x_ = 0;
		y_ = 0;
		z_ = 0;
		w_ = 1;
		return;
	}

	//创建(1,1,1,1)向量
	template<typename T>
	inline void KZVector4D<T>::SplatOne()
	{
		x_ = 1;
		y_ = 1;
		z_ = 1;
		w_ = 1;
		return;
	}

	//设置并返回(x,y,z,1)向量
	template<typename T>
	inline void KZVector4D<T>::Set(T x, T y, T z)
	{
		x_ = x;
		y_ = y;
		z_ = z;
		w_ = 1;
		return;
	}

	//设置并返回(s,s,s,1)向量
	template<typename T>
	inline void KZVector4D<T>::Replicate(T s)
	{
		x_ = s;
		y_ = s;
		z_ = s;
		w_ = 1;
		return;
	}

	//返回向量长度的平方
	template<typename T>
	inline T KZVector4D<T>::Vector3Length() const
	{
		return x_ * x_ + y_ * y_ + z_ * z_;
	}

	//返回向量长度
	template<typename T>
	inline float KZVector4D<T>::Vector3LengthSq() const
	{
		return sqrt(Vector3Length());
	}

	//向量点乘
	template<typename T>
	inline T KZVector4D<T>::Vector3Dot(const KZVector4D<T>& v2) const
	{
		return x_ * v2.x_ + y_ * v2.y_ + z_ * v2.z_;
	}

	//向量叉乘
	template<typename T>
	inline void KZVector4D<T>::Vector3Cross(KZVector4D<T>& ans_v, const KZVector4D<T>& v2) const
	{
		ans_v.x_ = y_*v2.z_ - z_*v2.y_;
		ans_v.y_ = z_*v2.x_ - x_*v2.z_;
		ans_v.z_ = x_*v2.y_ - y_*v2.x_;
		return;
	}

	//向量标准化(归一化)
	template<typename T>
	inline void KZVector4D<T>::Vector3Normalize()
	{
		float length = Vector3LengthSq();
		float one_over_length = 1 / length;
		if (length > 0.0) {
			x_ *= one_over_length;
			y_ *= one_over_length;
			z_ *= one_over_length;
		}
		return;
	}

	//返回正交向量
	template<typename T>
	inline void KZVector4D<T>::Vector3Orthogonal(KZVector4D<T>& ans_v) const
	{
		KZVector4D<T> v1 = KZVector4D<T>(x_, 2 * y_, 3 * z_);
		Vector3Cross(ans_v, v1);
		return;
	}

	//返回向量夹角(角度)
	template<typename T>
	inline float KZVector4D<T>::Vector3AngleBetweenVectors(const KZVector4D<T>& v2) const
	{
		T dot = Vector3Dot(v2);
		float cos_num = dot / (Vector3LengthSq() * v2.Vector3LengthSq());
		return RadianToAngle(acos(cos_num));
	}

	//弧度转角度
	inline float KZMath::RadianToAngle(float rad) {
		return rad * 180.0f / 3.14159f;
	}

	//角度转弧度
	inline float KZMath::AngleToRadian(float angle) {
		return angle * 3.14159f / 180.0f;
	}
}


#endif // !KZ_VECTOR4D

/*
ToDo
KZVector4D3LengthFast()使用泰勒级数计算向量长度
FastSin()使用查表和泰勒级数计算
*/