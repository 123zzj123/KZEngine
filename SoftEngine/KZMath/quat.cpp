#include "quat.h"
using namespace KZMath;

//按照w,x,y,z初始化
KZQuat::KZQuat(float w_in, float x_in, float y_in, float z_in) {
	w = w_in;
	x = x_in;
	y = y_in;
	z = z_in;
}

//按照实部加向量部分虚部
KZQuat::KZQuat(float q0_in, KZVector3D qv_in) {
	q0 = q0_in;
	qv = qv_in;
}

//打印四元数用于调试
ostream & KZMath::operator<<(ostream & o, KZQuat & v)
{
	o << v.w << " " << v.x << " " << v.y << " " << v.z;
	return o;
}

//根据旋转轴，旋转角度得到四元数
void KZMath::Vector3D_Theta_To_QUAT(KZQuat& quat, const KZVector3D& axis, float angle) {
	float half_angle = angle / 2;
	float sin_theta = sin(angle);
	quat.x = sin_theta * axis.x;
	quat.y = sin_theta * axis.y;
	quat.z = sin_theta * axis.z;
	quat.w = cos(half_angle);
	return;
}

//根据欧拉角得到四元数
void KZMath::EulerZYX_To_QUAT(KZQuat& quat, float theta_z, float theta_y, float theta_x) {
	float cos_half_theta_z = cos(theta_z * 0.5);
	float cos_half_theta_y = cos(theta_y * 0.5);
	float cos_half_theta_x = cos(theta_x * 0.5);

	float sin_half_theta_z = sin(theta_z * 0.5);
	float sin_half_theta_y = sin(theta_y * 0.5);
	float sin_half_theta_x = sin(theta_x * 0.5);

	quat.w = cos_half_theta_z * cos_half_theta_y * cos_half_theta_x + sin_half_theta_z * sin_half_theta_y * sin_half_theta_x;
	quat.x = cos_half_theta_z * cos_half_theta_y * sin_half_theta_x - sin_half_theta_z * sin_half_theta_y * cos_half_theta_x;
	quat.y = cos_half_theta_z * sin_half_theta_y * cos_half_theta_x + sin_half_theta_z * cos_half_theta_y * sin_half_theta_x;
	quat.z = sin_half_theta_z * cos_half_theta_y * cos_half_theta_x - cos_half_theta_z * sin_half_theta_y * sin_half_theta_x;
}

//四元数加法
KZQuat KZQuat::operator+(const KZQuat& other) {
	KZQuat quat;
	quat.w = w + other.w;
	quat.x = x + other.x;
	quat.y = y + other.y;
	quat.z = z + other.z;
	return quat;
}

//四元数减法
KZQuat KZQuat::operator-(const KZQuat& other) {
	KZQuat quat;
	quat.w = w - other.w;
	quat.x = x - other.x;
	quat.y = y - other.y;
	quat.z = z - other.z;
	return quat;
}

//四元数乘法
KZQuat KZQuat::operator*(const KZQuat& other) {
	float prd_0 = (z - y) * (other.y - other.z);
	float prd_1 = (w + x) * (other.w + other.x);
	float prd_2 = (w - x) * (other.y + other.z);
	float prd_3 = (y + z) * (other.w - other.x);
	float prd_4 = (z - x) * (other.x - other.y);
	float prd_5 = (z + x) * (other.x + other.y);
	float prd_6 = (w + y) * (other.w - other.z);
	float prd_7 = (w - y) * (other.w + other.z);
	float prd_8 = prd_5 + prd_6 + prd_7;
	float prd_9 = 0.5 * (prd_4 + prd_8);

	KZQuat ans_quat;
	ans_quat.w = prd_0 + prd_9 - prd_5;
	ans_quat.x = prd_1 + prd_9 - prd_8;
	ans_quat.y = prd_2 + prd_9 - prd_7;
	ans_quat.z = prd_3 + prd_9 - prd_6;

	return ans_quat;
}

//共轭四元数
void KZMath::Quat_Conjugate(KZQuat& ans_quat, const KZQuat& ori_quat) {
	ans_quat.w = ori_quat.w;
	ans_quat.x = -ori_quat.x;
	ans_quat.y = -ori_quat.y;
	ans_quat.z = -ori_quat.z;
	return;
}

//单位四元数倒数利用共轭四元数即倒数
void KZMath::UnitQuat_Inverse(KZQuat& ans_quat, const KZQuat& ori_quat) {
	Quat_Conjugate(ans_quat, ori_quat);
	return;
}

//任意四元数倒数
void KZMath::Quat_Inverse(KZQuat& ans_quat, const KZQuat& ori_quat) {
	KZQuat conj_quat;
	Quat_Conjugate(conj_quat, ori_quat);
	float inverse_norm = 1/Quat_Norm(ori_quat);
	conj_quat.w *= inverse_norm;
	conj_quat.x *= inverse_norm;
	conj_quat.y *= inverse_norm;
	conj_quat.z *= inverse_norm;
}

//四元数范数平方
float KZMath::Quat_Norm(const KZQuat& ori_quat) {
	KZQuat conj_quat, product_quat;
	Quat_Conjugate(conj_quat, ori_quat);
	product_quat = conj_quat*ori_quat;
	return product_quat.w;
}

//四元数标准化
void KZMath::Quat_Normalize(KZQuat& quat) {
	float inverse_sqrt_norm = 1/sqrt(Quat_Norm(quat));
	quat.w *= inverse_sqrt_norm;
	quat.x *= inverse_sqrt_norm;
	quat.y *= inverse_sqrt_norm;
	quat.z *= inverse_sqrt_norm;
	return;
}

//3个四元数相乘，用于旋转
void KZMath::Quat_Triple_Product(KZQuat& q1, KZQuat& q2, KZQuat& q3, KZQuat& quat_result) {
	quat_result = q1 * q2 * q3;
	return;
}