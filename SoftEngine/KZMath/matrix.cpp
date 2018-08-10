#include "matrix.h"
using namespace KZMath;

KZMatrix44::KZMatrix44() {
	for (uint32_t i = 0; i < 4; ++i) {
		for (uint32_t j = 0; j < 4; ++j) {
			m_[i][j] = 0.0f;
		}
	}
}

//构造函数
KZMatrix44::KZMatrix44(const KZVector4D& v1, const KZVector4D& v2, const KZVector4D& v3, const KZVector4D& v4) {
	m_[0][0] = v1.x_;
	m_[0][1] = v1.y_;
	m_[0][2] = v1.z_;
	m_[0][3] = v1.w_;

	m_[1][0] = v2.x_;
	m_[1][1] = v2.y_;
	m_[1][2] = v2.z_;
	m_[1][3] = v2.w_;

	m_[2][0] = v3.x_;
	m_[2][1] = v3.y_;
	m_[2][2] = v3.z_;
	m_[2][3] = v3.w_;

	m_[3][0] = v4.x_;
	m_[3][1] = v4.y_;
	m_[3][2] = v4.z_;
	m_[3][3] = v4.w_;
}

//构造函数
KZMatrix44::KZMatrix44(const float* pArray) {
	for (uint32_t i = 0; i < 4; ++i) {
		for (uint32_t j = 0; j < 4; ++j) {
			m_[i][j] = pArray[i * 4 + j];
		}
	}
}

//矩阵乘法返回矩阵
KZMatrix44 KZMatrix44::operator*(const KZMatrix44& nm) const{
	KZMatrix44 m1;
	for (uint32_t i = 0; i < 4; ++i) {
		KZVector4D row;
		row.x_ = m_[i][0];
		row.y_ = m_[i][1];
		row.z_ = m_[i][2];
		row.w_ = m_[i][3];
		for (uint32_t j = 0; j < 4; ++j) {
			KZVector4D col;
			col.x_ = nm.m_[0][j];
			col.y_ = nm.m_[1][j];
			col.z_ = nm.m_[2][j];
			col.w_ = nm.m_[3][j];
			m1.m_[i][j] = row * col;
		}
	}
	return m1;
}

//矩阵与向量乘法
KZVector4D KZMatrix44::operator*(const KZVector4D& nv) const{
	KZVector4D res_vec;
	res_vec.x_ = v1_ * nv;
	res_vec.y_ = v2_ * nv;
	res_vec.z_ = v3_ * nv;
	res_vec.w_ = v4_ * nv;
	return res_vec;
}

//矩阵乘法
void KZMatrix44::operator*=(const KZMatrix44& nm) {
	KZMatrix44 m1;
	for (uint32_t i = 0; i < 4; ++i) {
		KZVector4D row;
		row.x_ = m_[i][0];
		row.y_ = m_[i][1];
		row.z_ = m_[i][2];
		row.w_ = m_[i][3];
		for (uint32_t j = 0; j < 4; ++j) {
			KZVector4D col;
			col.x_ = nm.m_[0][j];
			col.y_ = nm.m_[1][j];
			col.z_ = nm.m_[2][j];
			col.w_ = nm.m_[3][j];

			m1.m_[i][j] = row * col;
		}
	}
	*this = m1;
}

//矩阵拷贝
KZMatrix44& KZMatrix44::operator=(const KZMatrix44& nm) {
	for (uint32_t i = 0; i < 4; ++i) {
		for (uint32_t j = 0; j < 4; ++j) {
			m_[i][j] = nm.m_[i][j];
		}
	}
	return *this;
}

//选取矩阵某个值
float KZMath::KZMatrix44::operator()(uint32_t row, uint32_t col)
{
	return m_[row][col];
}

//打印矩阵用于调试
ostream & KZMath::operator<<(ostream & o, KZMatrix44 & v)
{
	o << v.m00_ << " " << v.m01_ << " " << v.m02_ << " " << v.m03_ << "\n"
		<< v.m10_ << " " << v.m11_ << " " << v.m12_ << " " << v.m13_ << "\n"
		<< v.m20_ << " " << v.m21_ << " " << v.m22_ << " " << v.m23_ << "\n"
		<< v.m30_ << " " << v.m31_ << " " << v.m32_ << " " << v.m33_ << "\n";
	return o;
}

//返回零矩阵
void KZMatrix44::Zero() {
	for (uint32_t i = 0; i < 4; ++i) {
		for (uint32_t j = 0; j < 4; ++j) {
			m_[i][j] = 0;
		}
	}
	return;
}

//返回单位矩阵
void KZMatrix44::Identity() {
	for (uint32_t i = 0; i < 4; ++i) {
		for (uint32_t j = 0; j < 4; ++j) {
			if (i == j) {
				m_[i][j] = 1;
			}
			else {
				m_[i][j] = 0;
			}
		}
	}
	return;
}

//判断矩阵是否单位矩阵
bool KZMatrix44::IsIdentity() const {
	for (uint32_t i = 0; i < 4; ++i) {
		for (uint32_t j = 0; j < 4; ++j) {
			if (i == j) {
				if (m_[i][j] != 1) {
					return false;
				}
			}
			else {
				if (m_[i][j] != 0) {
					return false;
				}
			}
		}
	}
	return true;
}

//矩阵转置
void KZMatrix44::Transpose(KZMatrix44& matrix) const {
	for (uint32_t i = 0; i < 4; ++i) {
		for (uint32_t j = 0; j < 4; ++j) {
			matrix.m_[j][i] = m_[i][j];
		}
	}
	return;
}

//矩阵行列式
float KZMatrix44::Det(uint32_t order) const{
	if (order == 1) {
		return m_[0][0];
	}
	float ans = 0.0f;
	KZMatrix44 new_matrix;
	for (uint32_t i = 0; i < order; ++i) {

		for (uint32_t j = 0; j < order - 1; ++j) {
			for (uint32_t k = 0; k < order - 1; ++k) {
				new_matrix.m_[j][k] = m_[j + 1][(k >= i) ? k + 1 : k];
			}
		}

		float det = new_matrix.Det(order - 1);
		if (i % 2 == 0) {
			ans += m_[0][i] * det;
		}
		else
		{
			ans -= m_[0][i] * det;
		}
	}
	return ans;
}

//返回逆矩阵
void KZMatrix44::Inverse(KZMatrix44& matrix) const{
	uint32_t order = 4;
	float det = Det(order);
	if (det == 0) {
		return ;
	}
	Adjoint(matrix);
	for (uint32_t i = 0; i < order; ++i) {
		for (uint32_t j = 0; j < order; ++j) {
			matrix.m_[i][j] /= det;
		}
	}
	return;
}

//返回伴随矩阵
void KZMatrix44::Adjoint(KZMatrix44& matrix) const{
	KZMatrix44 temp_matrix;
	uint32_t order = 4;
	for (uint32_t i = 0; i < order; ++i) {
		for (uint32_t j = 0; j < order; ++j) {

			for (uint32_t k = 0; k < order - 1; ++k) {
				for (uint32_t t = 0; t < order - 1; ++t) {
					temp_matrix.m_[k][t] = m_[(k >= i) ? k + 1 : k][(t >= j) ? t + 1 : t];
				}
			}

			matrix.m_[j][i] = temp_matrix.Det(order - 1);
			if ((i + j) % 2 == 1) {
				matrix.m_[j][i] = -matrix.m_[j][i];
			}
		}
	}

	return;
}

//返回绕x轴旋转逆矩阵
void KZMatrix44::RotationXInverse() {
	m12_ = -m12_;
	m21_ = -m21_;
	return;
}

//返回绕x轴旋转矩阵
void KZMatrix44::RotationX(float angle) {
	Identity();
	m11_ = cos(KZMath::AngleToRadian(angle));
	m12_ = -sin(KZMath::AngleToRadian(angle));
	m21_ = sin(KZMath::AngleToRadian(angle));
	m22_ = cos(KZMath::AngleToRadian(angle));
	return;
}

//返回绕y轴旋转逆矩阵
void KZMatrix44::RotationYInverse() {
	m02_ = -m02_;
	m20_ = -m20_;
	return;
}

//返回绕y轴旋转矩阵
void KZMatrix44::RotationY(float angle) {
	Identity();
	m00_ = cos(KZMath::AngleToRadian(angle));
	m02_ = sin(KZMath::AngleToRadian(angle));
	m20_ = -sin(KZMath::AngleToRadian(angle));
	m22_ = cos(KZMath::AngleToRadian(angle));
	return;
}

//返回绕z轴旋转逆矩阵
void KZMatrix44::RotationZInverse() {
	m01_ = -m01_;
	m10_ = -m10_;
	return;
}

//返回绕z轴旋转矩阵
void KZMatrix44::RotationZ(float angle) {
	Identity();
	m00_ = cos(KZMath::AngleToRadian(angle));
	m01_ = -sin(KZMath::AngleToRadian(angle));
	m10_ = sin(KZMath::AngleToRadian(angle));
	m11_ = cos(KZMath::AngleToRadian(angle));
	return;
}

//返回绕任意轴旋转矩阵:推导绕x轴旋转到YOZ平面，绕y轴旋转到z轴，绕z轴旋转angle，一二步骤逆操作
void KZMatrix44::RotationAxis(const KZVector4D& axis, float angle) {
	Identity();
	m00_ = cos(KZMath::AngleToRadian(angle)) + axis.x_ * axis.x_ * (1 - cos(KZMath::AngleToRadian(angle)));
	m01_ = axis.x_ * axis.y_ * (1 - cos(KZMath::AngleToRadian(angle))) - axis.z_ * sin(KZMath::AngleToRadian(angle));
	m02_ = axis.x_ * axis.z_ * (1 - cos(KZMath::AngleToRadian(angle))) + axis.y_ * sin(KZMath::AngleToRadian(angle));

	m10_ = axis.x_ * axis.y_ * (1 - cos(KZMath::AngleToRadian(angle))) + axis.z_ * sin(KZMath::AngleToRadian(angle));
	m11_ = cos(KZMath::AngleToRadian(angle)) + axis.y_ * axis.y_ * (1 - cos(KZMath::AngleToRadian(angle)));
	m12_ = axis.y_ * axis.z_ * (1 - cos(KZMath::AngleToRadian(angle))) - axis.x_ * sin(KZMath::AngleToRadian(angle));

	m20_ = axis.x_ * axis.z_ * (1 - cos(KZMath::AngleToRadian(angle))) - axis.y_ * sin(KZMath::AngleToRadian(angle));
	m21_ = axis.y_ * axis.z_ * (1 - cos(KZMath::AngleToRadian(angle))) + axis.x_ * sin(KZMath::AngleToRadian(angle));
	m22_ = cos(KZMath::AngleToRadian(angle)) + axis.z_ * axis.z_ * (1 - cos(KZMath::AngleToRadian(angle)));
	
	return;
}

void KZMatrix44::TranslationInverse()
{
	m03_ = -m03_;
	m13_ = -m13_;
	m23_ = -m23_;
	return;
}

//返回平移矩阵(float x,y,z)
void KZMatrix44::Translation(float x, float y, float z) {
	Identity();
	m03_ = x;
	m13_ = y;
	m23_ = z;
	return;
}

//返回平移矩阵(vector)
void KZMatrix44::Translation(const KZVector4D& offset) {
	Identity();
	m03_ = offset.x_;
	m13_ = offset.y_;
	m23_ = offset.z_;
	return;
}

void KZMatrix44::ScalingInverse()
{
	m00_ = 1 / m00_;
	m11_ = 1 / m11_;
	m22_ = 1 / m22_;
	return;
}

//返回缩放矩阵(float x,y,z)
void KZMatrix44::Scaling(float scale_x, float scale_y, float scale_z) {
	Identity();
	m00_ = scale_x;
	m11_ = scale_y;
	m22_ = scale_z;
	return;
}

//返回缩放矩阵(vector)
void KZMatrix44::Scaling(const KZVector4D& scale) {
	Identity();
	m00_ = scale.x_;
	m11_ = scale.y_;
	m22_ = scale.z_;
	return;
}