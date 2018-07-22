#include "quat.h"
using namespace KZMath;

//����w,x,y,z��ʼ��
KZQuat::KZQuat(float w_in, float x_in, float y_in, float z_in) {
	w_ = w_in;
	x_ = x_in;
	y_ = y_in;
	z_ = z_in;
}

//����ʵ�������������鲿
KZQuat::KZQuat(float q0_in, KZVector3D qv_in) {
	q0_ = q0_in;
	qv_ = qv_in;
}

//��ӡ��Ԫ�����ڵ���
ostream & KZMath::operator<<(ostream & o, KZQuat & v)
{
	o << v.w_ << " " << v.x_ << " " << v.y_ << " " << v.z_;
	return o;
}

//������ת�ᣬ��ת�Ƕȵõ���Ԫ��
void KZQuat::SetFromVector3DTheta(const KZVector3D& axis, float angle) {
	float half_angle = angle / 2;
	float sin_theta = sin(half_angle);
	x_ = sin_theta * axis.x_;
	y_ = sin_theta * axis.y_;
	z_ = sin_theta * axis.z_;
	w_ = cos(half_angle);
	return;
}

//����ŷ���ǵõ���Ԫ��
void KZQuat::SetFromEulerZYX(float theta_z, float theta_y, float theta_x) {
	float cos_half_theta_z = cos(theta_z * 0.5f);
	float cos_half_theta_y = cos(theta_y * 0.5f);
	float cos_half_theta_x = cos(theta_x * 0.5f);

	float sin_half_theta_z = sin(theta_z * 0.5f);
	float sin_half_theta_y = sin(theta_y * 0.5f);
	float sin_half_theta_x = sin(theta_x * 0.5f);

	w_ = cos_half_theta_z * cos_half_theta_y * cos_half_theta_x + sin_half_theta_z * sin_half_theta_y * sin_half_theta_x;
	x_ = cos_half_theta_z * cos_half_theta_y * sin_half_theta_x - sin_half_theta_z * sin_half_theta_y * cos_half_theta_x;
	y_ = cos_half_theta_z * sin_half_theta_y * cos_half_theta_x + sin_half_theta_z * cos_half_theta_y * sin_half_theta_x;
	z_ = sin_half_theta_z * cos_half_theta_y * cos_half_theta_x - cos_half_theta_z * sin_half_theta_y * sin_half_theta_x;
}

//��Ԫ���ӷ�
KZQuat KZQuat::operator+(const KZQuat& other) const {
	KZQuat quat;
	quat.w_ = w_ + other.w_;
	quat.x_ = x_ + other.x_;
	quat.y_ = y_ + other.y_;
	quat.z_ = z_ + other.z_;
	return quat;
}

//��Ԫ������
KZQuat KZQuat::operator-(const KZQuat& other) const{
	KZQuat quat;
	quat.w_ = w_ - other.w_;
	quat.x_ = x_ - other.x_;
	quat.y_ = y_ - other.y_;
	quat.z_ = z_ - other.z_;
	return quat;
}

//��Ԫ���˷�
KZQuat KZQuat::operator*(const KZQuat& other) const{
	float prd_0 = (z_ - y_) * (other.y_ - other.z_);
	float prd_1 = (w_ + x_) * (other.w_ + other.x_);
	float prd_2 = (w_ - x_) * (other.y_ + other.z_);
	float prd_3 = (y_ + z_) * (other.w_ - other.x_);
	float prd_4 = (z_ - x_) * (other.x_ - other.y_);
	float prd_5 = (z_ + x_) * (other.x_ + other.y_);
	float prd_6 = (w_ + y_) * (other.w_ - other.z_);
	float prd_7 = (w_ - y_) * (other.w_ + other.z_);
	float prd_8 = prd_5 + prd_6 + prd_7;
	float prd_9 = 0.5f * (prd_4 + prd_8);

	KZQuat ans_quat;
	ans_quat.w_= prd_0 + prd_9 - prd_5;
	ans_quat.x_ = prd_1 + prd_9 - prd_8;
	ans_quat.y_ = prd_2 + prd_9 - prd_7;
	ans_quat.z_ = prd_3 + prd_9 - prd_6;

	return ans_quat;
}

//������Ԫ��
void KZQuat::Conjugate(KZQuat& ans_quat) const {
	ans_quat.w_ = w_;
	ans_quat.x_ = -x_;
	ans_quat.y_ = -y_;
	ans_quat.z_ = -z_;
	return;
}

//��λ��Ԫ���������ù�����Ԫ��������
void KZQuat::UnitInverse(KZQuat& ans_quat) const {
	Conjugate(ans_quat);
	return;
}

//������Ԫ������
void KZQuat::Inverse(KZQuat& ans_quat) const{
	KZQuat conj_quat;
	Conjugate(conj_quat);
	float inverse_norm = 1/Norm();
	ans_quat.w_ = conj_quat.w_ * inverse_norm;
	ans_quat.x_ = conj_quat.x_ * inverse_norm;
	ans_quat.y_ = conj_quat.y_ * inverse_norm;
	ans_quat.z_ = conj_quat.z_ * inverse_norm;
}

//��Ԫ������ƽ��
float KZQuat::Norm() const {
	KZQuat conj_quat, product_quat;
	Conjugate(conj_quat);
	product_quat = conj_quat * (*this);
	return product_quat.w_;
}

//��Ԫ����׼��
void KZQuat::Normalize() {
	float inverse_sqrt_norm = 1/sqrt(Norm());
	w_ *= inverse_sqrt_norm;
	x_ *= inverse_sqrt_norm;
	y_ *= inverse_sqrt_norm;
	z_ *= inverse_sqrt_norm;
	return;
}

//3����Ԫ����ˣ�������ת
void KZQuat::TripleProduct(const KZQuat& q2, const KZQuat& q3, KZQuat& quat_result) const {
	quat_result = (*this) * q2 * q3;
	return;
}