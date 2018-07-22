#include"vector.h"
using namespace KZMath;

//������ֵ
KZVector4D& KZVector4D::operator= (const KZVector4D& v1) {
	x_ = v1.x_;
	y_ = v1.y_;
	z_ = v1.z_;
	w_ = v1.w_;
	return *this;
}

//���ظ�������
KZVector4D KZVector4D::operator- () const {
	return KZVector4D(-x_, -y_, -z_);
}

//�����ӷ���������
KZVector4D KZVector4D::operator+ (const KZVector4D& other) const {
	KZVector4D v;
	v.x_ = x_ + other.x_;
	v.y_ = y_ + other.y_;
	v.z_ = z_ + other.z_;
	return v;
}

//����������������
KZVector4D KZVector4D::operator- (const KZVector4D& other) const {
	KZVector4D v;
	v.x_ = x_ - other.x_;
	v.y_ = y_ - other.y_;
	v.z_ = z_ - other.z_;
	return v;
}

//�����������˷�
float KZVector4D::operator* (const KZVector4D& other) const {
	KZVector4D v;
	v.x_ = x_ * other.x_;
	v.y_ = y_ * other.y_;
	v.z_ = z_ * other.z_;
	v.w_ = w_ * other.w_;
	return v.x_ + v.y_ + v.z_ + v.w_;
}

//���������������������
KZVector4D KZVector4D::operator/ (float num) {
	KZVector4D v;
	v.x_ = x_ / num;
	v.y_ = y_ / num;
	v.z_ = z_ / num;
	return v;
}

//����������˷���������
KZVector4D KZVector4D::operator* (float num) {
	KZVector4D v;
	v.x_ = x_ * num;
	v.y_ = y_ * num;
	v.z_ = z_ * num;
	return v;
}

//�����ӷ�
void KZVector4D::operator+= (const KZVector4D& other) {
	x_ += other.x_;
	y_ += other.y_;
	z_ += other.z_;
	return;
}

//��������
void KZVector4D::operator-= (const KZVector4D& other) {
	x_ -= other.x_;
	y_ -= other.y_;
	z_ -= other.z_;
	return;
}

//����������˷�(vector)
void KZVector4D::operator*= (const KZVector4D& other) {
	x_ *= other.x_;
	y_ *= other.y_;
	z_ *= other.z_;
	return;
}

//����������˷�(float)
void KZVector4D::operator*= (float num) {
	x_ *= num;
	y_ *= num;
	z_ *= num;
	return;
}

//�������������
void KZVector4D::operator/= (float num) {
	x_ /= num;
	y_ /= num;
	z_ /= num;
	return;
}

//�ж�����������
bool KZVector4D::operator== (const KZVector4D& v) {
	return x_ == v.x_ && y_== v.y_ && z_ == v.z_;
}

//�ж������������
bool KZVector4D::operator!=(const KZVector4D& v) {
	return x_ != v.x_ || y_ != v.y_ || z_ != v.z_;
}

//��ӡ�������ڵ���
ostream& KZMath::operator << (ostream & o, KZVector4D & v)
{
	o << v.x_ << " " << v.y_ << " " << v.z_ << " " << v.w_;
	return o;
}

//����������
void KZVector4D::Zero() {
	x_ = 0;
	y_ = 0;
	z_ = 0;
	w_ = 1;
	return;
}

//����(1,1,1,1)����
void KZVector4D::SplatOne()
{
	x_ = 1;
	y_ = 1;
	z_ = 1;
	w_ = 1;
	return;
}

//���ò�����(x,y,z,1)����
void KZVector4D::Set(float x, float y, float z)
{
	x_ = x;
	y_ = y;
	z_ = z;
	w_ = 1;
	return;
}

//���ò�����(s,s,s,1)����
void KZVector4D::Replicate(float s)
{
	x_ = s;
	y_ = s;
	z_ = s;
	w_ = 1;
	return;
}

//�����������ȵ�ƽ��
float KZVector4D::Vector3Length() const
{
	return x_ * x_ + y_ * y_ + z_ * z_;
}

//������������
float KZVector4D::Vector3LengthSq() const
{
	return sqrt(Vector3Length());
}

//�������
float KZVector4D::Vector3Dot(const KZVector4D& v2) const
{
	return x_ * v2.x_ + y_ * v2.y_ + z_ * v2.z_;
}

//�������
void KZVector4D::Vector3Cross(KZVector4D& ans_v, const KZVector4D& v2) const
{
	ans_v.x_ = y_*v2.z_ - z_*v2.y_;
	ans_v.y_ = z_*v2.x_ - x_*v2.z_;
	ans_v.z_ = x_*v2.y_ - y_*v2.x_;
	return ;
}

//������׼��(��һ��)
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

//������������
void KZVector4D::Vector3Orthogonal(KZVector4D& ans_v) const
{
	KZVector4D v1 = KZVector4D(x_, 2 * y_, 3 * z_);
	Vector3Cross(ans_v, v1);
	return;
}

//���������н�(�Ƕ�)
float KZVector4D::Vector3AngleBetweenVectors(const KZVector4D& v2) const
{
	float dot = Vector3Dot(v2);
	float cos_num = dot / (Vector3LengthSq() * v2.Vector3LengthSq());
	return RadianToAngle(acos(cos_num));
}

//����ת�Ƕ�
float KZMath::RadianToAngle(float rad) {
	return rad * 180.0f / M_PI;
}

//�Ƕ�ת����
float KZMath::AngleToRadian(float angle) {
	return angle * M_PI / 180.0f;
}