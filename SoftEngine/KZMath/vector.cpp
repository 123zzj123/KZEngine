#include"vector.h"
using namespace KZMath;

//向量赋值
KZVector4D& KZVector4D::operator= (const KZVector4D& v1) {
	x_ = v1.x_;
	y_ = v1.y_;
	z_ = v1.z_;
	w_ = v1.w_;
	return *this;
}

//返回负的向量
KZVector4D KZVector4D::operator- () const {
	return KZVector4D(-x_, -y_, -z_);
}

//向量加法返回向量
KZVector4D KZVector4D::operator+ (const KZVector4D& other) const {
	KZVector4D v;
	v.x_ = x_ + other.x_;
	v.y_ = y_ + other.y_;
	v.z_ = z_ + other.z_;
	return v;
}

//向量减法返回向量
KZVector4D KZVector4D::operator- (const KZVector4D& other) const {
	KZVector4D v;
	v.x_ = x_ - other.x_;
	v.y_ = y_ - other.y_;
	v.z_ = z_ - other.z_;
	return v;
}

//向量与向量乘法
float KZVector4D::operator* (const KZVector4D& other) const {
	KZVector4D v;
	v.x_ = x_ * other.x_;
	v.y_ = y_ * other.y_;
	v.z_ = z_ * other.z_;
	v.w_ = w_ * other.w_;
	return v.x_ + v.y_ + v.z_ + v.w_;
}

//向量与标量除法返回向量
KZVector4D KZVector4D::operator/ (float num) {
	KZVector4D v;
	v.x_ = x_ / num;
	v.y_ = y_ / num;
	v.z_ = z_ / num;
	return v;
}

//向量与标量乘法返回向量
KZVector4D KZVector4D::operator* (float num) {
	KZVector4D v;
	v.x_ = x_ * num;
	v.y_ = y_ * num;
	v.z_ = z_ * num;
	return v;
}

//向量加法
void KZVector4D::operator+= (const KZVector4D& other) {
	x_ += other.x_;
	y_ += other.y_;
	z_ += other.z_;
	return;
}

//向量减法
void KZVector4D::operator-= (const KZVector4D& other) {
	x_ -= other.x_;
	y_ -= other.y_;
	z_ -= other.z_;
	return;
}

//向量与标量乘法(vector)
void KZVector4D::operator*= (const KZVector4D& other) {
	x_ *= other.x_;
	y_ *= other.y_;
	z_ *= other.z_;
	return;
}

//向量与标量乘法(float)
void KZVector4D::operator*= (float num) {
	x_ *= num;
	y_ *= num;
	z_ *= num;
	return;
}

//向量与标量除法
void KZVector4D::operator/= (float num) {
	x_ /= num;
	y_ /= num;
	z_ /= num;
	return;
}

//判断向量相等与否
bool KZVector4D::operator== (const KZVector4D& v) {
	return x_ == v.x_ && y_== v.y_ && z_ == v.z_;
}

//判断向量不等与否
bool KZVector4D::operator!=(const KZVector4D& v) {
	return x_ != v.x_ || y_ != v.y_ || z_ != v.z_;
}

//打印向量用于调试
ostream& KZMath::operator << (ostream & o, KZVector4D & v)
{
	o << v.x_ << " " << v.y_ << " " << v.z_ << " " << v.w_;
	return o;
}

//创建零向量
void KZVector4D::Zero() {
	x_ = 0;
	y_ = 0;
	z_ = 0;
	w_ = 1;
	return;
}

//创建(1,1,1,1)向量
void KZVector4D::SplatOne()
{
	x_ = 1;
	y_ = 1;
	z_ = 1;
	w_ = 1;
	return;
}

//设置并返回(x,y,z,1)向量
void KZVector4D::Set(float x, float y, float z)
{
	x_ = x;
	y_ = y;
	z_ = z;
	w_ = 1;
	return;
}

//设置并返回(s,s,s,1)向量
void KZVector4D::Replicate(float s)
{
	x_ = s;
	y_ = s;
	z_ = s;
	w_ = 1;
	return;
}

//返回向量长度的平方
float KZVector4D::Vector3Length() const
{
	return x_ * x_ + y_ * y_ + z_ * z_;
}

//返回向量长度
float KZVector4D::Vector3LengthSq() const
{
	return sqrt(Vector3Length());
}

//向量点乘
float KZVector4D::Vector3Dot(const KZVector4D& v2) const
{
	return x_ * v2.x_ + y_ * v2.y_ + z_ * v2.z_;
}

//向量叉乘
void KZVector4D::Vector3Cross(KZVector4D& ans_v, const KZVector4D& v2) const
{
	ans_v.x_ = y_*v2.z_ - z_*v2.y_;
	ans_v.y_ = z_*v2.x_ - x_*v2.z_;
	ans_v.z_ = x_*v2.y_ - y_*v2.x_;
	return ;
}

//向量标准化(归一化)
void KZVector4D::Vector3Normalize()
{
	float length = Vector3LengthSq();
	if (length > 0.0) {
		x_ /= length;
		y_ /= length;
		z_ /= length;
	}
	return;
}

//返回正交向量
void KZVector4D::Vector3Orthogonal(KZVector4D& ans_v) const
{
	KZVector4D v1 = KZVector4D(x_, 2 * y_, 3 * z_);
	Vector3Cross(ans_v, v1);
	return;
}

//返回向量夹角(角度)
float KZVector4D::Vector3AngleBetweenVectors(const KZVector4D& v2) const
{
	float dot = Vector3Dot(v2);
	float cos_num = dot / (Vector3LengthSq() * v2.Vector3LengthSq());
	return RadianToAngle(acos(cos_num));
}

//弧度转角度
float KZMath::RadianToAngle(float rad) {
	return rad * 180.0f / M_PI;
}

//角度转弧度
float KZMath::AngleToRadian(float angle) {
	return angle * M_PI / 180.0f;
}