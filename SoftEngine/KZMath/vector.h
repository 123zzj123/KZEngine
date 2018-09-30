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
		//���캯��
		explicit KZVector4D(T x = 0, T y = 0, T z = 0, T w = 1):x_(x),y_(y),z_(z),w_(w){};
		//�����ӷ���������
		KZVector4D<T> operator+ (const KZVector4D<T>& other) const;
		//����������������
		KZVector4D<T> operator- (const KZVector4D<T>& other) const;
		//�������������
		T operator* (const KZVector4D<T>& other) const;
		//����������˷���������
		KZVector4D<T> operator* (T num) const;
		//���������������������
		KZVector4D<T> operator/ (T num) const;
		//������ֵ
		KZVector4D<T>& operator= (const KZVector4D<T>& v1);
		//���ظ�������
		KZVector4D<T> operator- () const;
		//�����ӷ�
		void operator+= (const KZVector4D<T>& other);
		//��������
		void operator-= (const KZVector4D<T>& other);
		//����������˷�(vector)
		void operator*= (const KZVector4D<T>& other);
		//����������˷�(float)
		void operator*= (T num);
		//�������������
		void operator/= (T num);
		//�ж�����������
		bool operator== (const KZVector4D<T>& v) const;
		//�ж������������
		bool operator!= (const KZVector4D<T>& v) const;
		//����������
		void Zero();
		//����(1,1,1,1)����
		void SplatOne();
		//���ò�����(x,y,z,1)����
		void Set(T x, T y, T z);
		//���ò�����(s,s,s,1)����
		void Replicate(T s);
		//�����������ȵ�ƽ��
		T Vector3Length() const;
		//������������
		float Vector3LengthSq() const;
		//�������
		T Vector3Dot(const KZVector4D<T>& v2) const;
		//�������
		void Vector3Cross(KZVector4D<T>& ans_v, const KZVector4D<T>& v2) const;
		//������׼��(��һ��)
		void Vector3Normalize();
		//������������
		void Vector3Orthogonal(KZVector4D<T>& ans_v) const;
		//���������н�(�Ƕ�)
		float Vector3AngleBetweenVectors(const KZVector4D<T>& v2) const;
	public:
		T x_;
		T y_;
		T z_;
		T w_;
	};
	//��ӡ�������ڵ���
	template<typename T>
	ostream& operator<< (ostream& o, KZVector4D<T>& v);
	//����ת�Ƕ�
	float RadianToAngle(float rad);
	//�Ƕ�ת����
	float AngleToRadian(float angle);
};

namespace KZMath {
	//������ֵ
	template<typename T>
	inline KZVector4D<T>& KZVector4D<T>::operator= (const KZVector4D<T>& v1) {
		x_ = v1.x_;
		y_ = v1.y_;
		z_ = v1.z_;
		w_ = v1.w_;
		return *this;
	}

	//���ظ�������
	template<typename T>
	inline KZVector4D<T> KZVector4D<T>::operator- () const {
		return KZVector4D<T>(-x_, -y_, -z_);
	}

	//�����ӷ���������
	template<typename T>
	inline KZVector4D<T> KZVector4D<T>::operator+ (const KZVector4D<T>& other) const {
		KZVector4D<T> v;
		v.x_ = x_ + other.x_;
		v.y_ = y_ + other.y_;
		v.z_ = z_ + other.z_;
		return v;
	}

	//����������������
	template<typename T>
	inline KZVector4D<T> KZVector4D<T>::operator- (const KZVector4D<T>& other) const {
		KZVector4D<T> v;
		v.x_ = x_ - other.x_;
		v.y_ = y_ - other.y_;
		v.z_ = z_ - other.z_;
		return v;
	}

	//�����������˷�
	template<typename T>
	inline T KZVector4D<T>::operator* (const KZVector4D<T>& other) const {
		KZVector4D<T> v;
		v.x_ = x_ * other.x_;
		v.y_ = y_ * other.y_;
		v.z_ = z_ * other.z_;
		v.w_ = w_ * other.w_;
		return v.x_ + v.y_ + v.z_ + v.w_;
	}

	//���������������������
	template<typename T>
	inline KZVector4D<T> KZVector4D<T>::operator/ (T num) const {
		KZVector4D<T> v;
		T one_over_num = 1 / num;
		v.x_ = x_ * one_over_num;
		v.y_ = y_ * one_over_num;
		v.z_ = z_ * one_over_num;
		return v;
	}

	//����������˷���������
	template<typename T>
	inline KZVector4D<T> KZVector4D<T>::operator* (T num) const {
		KZVector4D v;
		v.x_ = x_ * num;
		v.y_ = y_ * num;
		v.z_ = z_ * num;
		return v;
	}

	//�����ӷ�
	template<typename T>
	inline void KZVector4D<T>::operator+= (const KZVector4D<T>& other) {
		x_ += other.x_;
		y_ += other.y_;
		z_ += other.z_;
		return;
	}

	//��������
	template<typename T>
	inline void KZVector4D<T>::operator-= (const KZVector4D<T>& other) {
		x_ -= other.x_;
		y_ -= other.y_;
		z_ -= other.z_;
		return;
	}

	//����������˷�(vector)
	template<typename T>
	inline void KZVector4D<T>::operator*= (const KZVector4D<T>& other) {
		x_ *= other.x_;
		y_ *= other.y_;
		z_ *= other.z_;
		return;
	}

	//����������˷�(float)
	template<typename T>
	inline void KZVector4D<T>::operator*= (T num) {
		x_ *= num;
		y_ *= num;
		z_ *= num;
		return;
	}

	//�������������
	template<typename T>
	inline void KZVector4D<T>::operator/= (T num) {
		T one_over_num = 1 / num;
		x_ *= one_over_num;
		y_ *= one_over_num;
		z_ *= one_over_num;
		return;
	}

	//�ж�����������
	template<typename T>
	inline bool KZVector4D<T>::operator== (const KZVector4D<T>& v) const {
		return x_ == v.x_ && y_ == v.y_ && z_ == v.z_;
	}

	//�ж������������
	template<typename T>
	inline bool KZVector4D<T>::operator!=(const KZVector4D<T>& v) const {
		return x_ != v.x_ || y_ != v.y_ || z_ != v.z_;
	}

	//��ӡ�������ڵ���
	template<typename T>
	inline ostream& KZMath::operator << (ostream & o, KZVector4D<T> & v)
	{
		o << v.x_ << " " << v.y_ << " " << v.z_ << " " << v.w_;
		return o;
	}

	//����������
	template<typename T>
	inline void KZVector4D<T>::Zero() {
		x_ = 0;
		y_ = 0;
		z_ = 0;
		w_ = 1;
		return;
	}

	//����(1,1,1,1)����
	template<typename T>
	inline void KZVector4D<T>::SplatOne()
	{
		x_ = 1;
		y_ = 1;
		z_ = 1;
		w_ = 1;
		return;
	}

	//���ò�����(x,y,z,1)����
	template<typename T>
	inline void KZVector4D<T>::Set(T x, T y, T z)
	{
		x_ = x;
		y_ = y;
		z_ = z;
		w_ = 1;
		return;
	}

	//���ò�����(s,s,s,1)����
	template<typename T>
	inline void KZVector4D<T>::Replicate(T s)
	{
		x_ = s;
		y_ = s;
		z_ = s;
		w_ = 1;
		return;
	}

	//�����������ȵ�ƽ��
	template<typename T>
	inline T KZVector4D<T>::Vector3Length() const
	{
		return x_ * x_ + y_ * y_ + z_ * z_;
	}

	//������������
	template<typename T>
	inline float KZVector4D<T>::Vector3LengthSq() const
	{
		return sqrt(Vector3Length());
	}

	//�������
	template<typename T>
	inline T KZVector4D<T>::Vector3Dot(const KZVector4D<T>& v2) const
	{
		return x_ * v2.x_ + y_ * v2.y_ + z_ * v2.z_;
	}

	//�������
	template<typename T>
	inline void KZVector4D<T>::Vector3Cross(KZVector4D<T>& ans_v, const KZVector4D<T>& v2) const
	{
		ans_v.x_ = y_*v2.z_ - z_*v2.y_;
		ans_v.y_ = z_*v2.x_ - x_*v2.z_;
		ans_v.z_ = x_*v2.y_ - y_*v2.x_;
		return;
	}

	//������׼��(��һ��)
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

	//������������
	template<typename T>
	inline void KZVector4D<T>::Vector3Orthogonal(KZVector4D<T>& ans_v) const
	{
		KZVector4D<T> v1 = KZVector4D<T>(x_, 2 * y_, 3 * z_);
		Vector3Cross(ans_v, v1);
		return;
	}

	//���������н�(�Ƕ�)
	template<typename T>
	inline float KZVector4D<T>::Vector3AngleBetweenVectors(const KZVector4D<T>& v2) const
	{
		T dot = Vector3Dot(v2);
		float cos_num = dot / (Vector3LengthSq() * v2.Vector3LengthSq());
		return RadianToAngle(acos(cos_num));
	}

	//����ת�Ƕ�
	inline float KZMath::RadianToAngle(float rad) {
		return rad * 180.0f / 3.14159f;
	}

	//�Ƕ�ת����
	inline float KZMath::AngleToRadian(float angle) {
		return angle * 3.14159f / 180.0f;
	}
}


#endif // !KZ_VECTOR4D

/*
ToDo
KZVector4D3LengthFast()ʹ��̩�ռ���������������
FastSin()ʹ�ò���̩�ռ�������
*/