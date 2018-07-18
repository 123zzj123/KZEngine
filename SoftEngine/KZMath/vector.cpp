#include"vector.h"
using namespace KZMath;

//������ֵ
KZVector4D& KZVector4D::operator= (const KZVector4D& v1) {
	x = v1.x;
	y = v1.y;
	z = v1.z;
	w = v1.w;
	return *this;
}

//���ظ�������
KZVector4D KZVector4D::operator- () const {
	return KZVector4D(-x, -y, -z);
}

//�����ӷ���������
KZVector4D KZVector4D::operator+ (const KZVector4D& other) {
	KZVector4D v;
	v.x = x + other.x;
	v.y = y + other.y;
	v.z = z + other.z;
	return v;
}

//����������������
KZVector4D KZVector4D::operator- (const KZVector4D& other) {
	KZVector4D v;
	v.x = x - other.x;
	v.y = y - other.y;
	v.z = z - other.z;
	return v;
}

//�������������
float KZVector4D::operator* (const KZVector4D& other) {
	KZVector4D v;
	v.x = x * other.x;
	v.y = y * other.y;
	v.z = z * other.z;
	v.w = w * other.w;
	return v.x + v.y + v.z + v.w;
}

//���������������������
KZVector4D KZVector4D::operator/ (float num) {
	KZVector4D v;
	v.x = x / num;
	v.y = y / num;
	v.z = z / num;
	return v;
}

//����������˷���������
KZVector4D KZVector4D::operator* (float num) {
	KZVector4D v;
	v.x = x * num;
	v.y = y * num;
	v.z = z * num;
	return v;
}

//�����ӷ�
void KZVector4D::operator+= (const KZVector4D& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return;
}

//��������
void KZVector4D::operator-= (const KZVector4D& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return;
}

//����������˷�(vector)
void KZVector4D::operator*= (const KZVector4D& other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;
	return;
}

//����������˷�(float)
void KZVector4D::operator*= (float num) {
	x *= num;
	y *= num;
	z *= num;
	return;
}

//�������������
void KZVector4D::operator/= (float num) {
	x /= num;
	y /= num;
	z /= num;
	return;
}

//�ж�����������
bool KZVector4D::operator== (const KZVector4D& v) {
	return x == v.x && y == v.y && z == v.z;
}

//�ж������������
bool KZVector4D::operator!=(const KZVector4D& v) {
	return x != v.x || y != v.y || z != v.z;
}

//��ӡ�������ڵ���
ostream& KZMath::operator << (ostream & o, KZVector4D & v)
{
	o << v.x << " " << v.y << " " << v.z << " " << v.w;
	return o;
}

//����������
void KZMath::KZVector4DZero(KZVector4D& ans_v) {
	ans_v.x = 0;
	ans_v.y = 0;
	ans_v.z = 0;
	ans_v.w = 1;
	return;
}

//����(1,1,1,1)����
void KZMath::KZVector4DSplatOne(KZVector4D& ans_v)
{
	ans_v.x = 1;
	ans_v.y = 1;
	ans_v.z = 1;
	ans_v.w = 1;
	return;
}

//���ò�����(x,y,z,1)����
void KZMath::KZVector4DSet(KZVector4D& ans_v, float x, float y, float z)
{
	ans_v.x = x;
	ans_v.y = y;
	ans_v.z = z;
	ans_v.w = 1;
	return;
}

//���ò�����(s,s,s,1)����
void KZMath::KZVector4DReplicate(KZVector4D& ans_v, float s)
{
	ans_v.x = s;
	ans_v.y = s;
	ans_v.z = s;
	ans_v.w = 1;
	return;
}

//�����������ȵ�ƽ��
float KZMath::KZVector4D3Length(const KZVector4D& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

//������������
float KZMath::KZVector4D3LengthSq(const KZVector4D& v)
{
	return sqrt(KZVector4D3Length(v));
}

//�������
float KZMath::KZVector4D3Dot(const KZVector4D& v1, const KZVector4D& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

//�������
void KZMath::KZVector4D3Cross(KZVector4D& ans_v, const KZVector4D& v1, const KZVector4D& v2)
{
	ans_v.x = v1.y*v2.z - v1.z*v2.y;
	ans_v.y = v1.z*v2.x - v1.x*v2.z;
	ans_v.z = v1.x*v2.y - v1.y*v2.x;
	return ;
}

//������׼��(��һ��)
void KZMath::KZVector4D3Normalize(KZVector4D& v)
{
	float length = KZVector4D3LengthSq(v);
	if (length > 0.0) {
		v.x /= length;
		v.y /= length;
		v.z /= length;
	}
	return;
}

//������������
void KZMath::KZVector4D3Orthogonal(KZVector4D& ans_v, const KZVector4D& v)
{
	KZVector4D v1 = KZVector4D(v.x, 2 * v.y, 3 * v.z);
	KZVector4D3Cross(ans_v, v, v1);
	return;
}

//���������н�(�Ƕ�)
float KZMath::KZVector4D3AngleBetweenVectors(const KZVector4D& v1, const KZVector4D& v2)
{
	float dot = KZVector4D3Dot(v1, v2);
	float cos_num = dot / (KZVector4D3LengthSq(v1) * KZVector4D3LengthSq(v2));
	return RadianToAngle(acos(cos_num));
}

//����ת�Ƕ�
float KZMath::RadianToAngle(float rad) {
	return rad * 180 / M_PI;
}

//�Ƕ�ת����
float KZMath::AngleToRadian(float angle) {
	return angle * M_PI / 180;
}