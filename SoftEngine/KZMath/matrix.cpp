#include "matrix.h"
using namespace KZMath;

KZMatrix44::KZMatrix44() {
	for (uint32_t i = 0; i < 4; ++i) {
		for (uint32_t j = 0; j < 4; ++j) {
			m[i][j] = 0.0f;
		}
	}
}

//构造函数
KZMatrix44::KZMatrix44(KZVector4D& v1, KZVector4D& v2, KZVector4D& v3, KZVector4D& v4) {
	m[0][0] = v1.x;
	m[0][1] = v1.y;
	m[0][2] = v1.z;
	m[0][3] = v1.w;

	m[1][0] = v2.x;
	m[1][1] = v2.y;
	m[1][2] = v2.z;
	m[1][3] = v2.w;

	m[2][0] = v3.x;
	m[2][1] = v3.y;
	m[2][2] = v3.z;
	m[2][3] = v3.w;

	m[3][0] = v4.x;
	m[3][1] = v4.y;
	m[3][2] = v4.z;
	m[3][3] = v4.w;
}

//构造函数
KZMatrix44::KZMatrix44(const float* pArray) {
	for (uint32_t i = 0; i < 4; ++i) {
		for (uint32_t j = 0; j < 4; ++j) {
			m[i][j] = pArray[i * 4 + j];
		}
	}
}

//矩阵乘法返回矩阵
KZMatrix44 KZMatrix44::operator*(const KZMatrix44& nm) {
	KZMatrix44 m1;
	for (uint32_t i = 0; i < 4; ++i) {
		KZVector4D row;
		row.x = m[i][0];
		row.y = m[i][1];
		row.z = m[i][2];
		row.w = m[i][3];
		for (uint32_t j = 0; j < 4; ++j) {
			KZVector4D col;
			col.x = nm.m[0][j];
			col.y = nm.m[1][j];
			col.z = nm.m[2][j];
			col.w = nm.m[3][j];
			m1.m[i][j] = row * col;
		}
	}

	return m1;
}

//矩阵乘法
void KZMatrix44::operator*=(const KZMatrix44& nm) {
	KZMatrix44 m1;
	for (uint32_t i = 0; i < 4; ++i) {
		KZVector4D row;
		row.x = m[i][0];
		row.y = m[i][1];
		row.z = m[i][2];
		row.w = m[i][3];
		for (uint32_t j = 0; j < 4; ++j) {
			KZVector4D col;
			col.x = nm.m[0][j];
			col.y = nm.m[1][j];
			col.z = nm.m[2][j];
			col.w = nm.m[3][j];

			m1.m[i][j] = row * col;
		}
	}
	*this = m1;
}

//矩阵拷贝
KZMatrix44& KZMatrix44::operator=(const KZMatrix44& nm) {
	for (uint32_t i = 0; i < 4; ++i) {
		for (uint32_t j = 0; j < 4; ++j) {
			m[i][j] = nm.m[i][j];
		}
	}
	return *this;
}

//选取矩阵某个值
float KZMath::KZMatrix44::operator()(uint32_t row, uint32_t col)
{
	return m[row][col];
}

//打印矩阵用于调试
ostream & KZMath::operator<<(ostream & o, KZMatrix44 & v)
{
	o << v.m00 << " " << v.m01 << " " << v.m02 << " " << v.m03 << "\n"
		<< v.m10 << " " << v.m11 << " " << v.m12 << " " << v.m13 << "\n"
		<< v.m20 << " " << v.m21 << " " << v.m22 << " " << v.m23 << "\n"
		<< v.m30 << " " << v.m31 << " " << v.m32 << " " << v.m33 << "\n";
	return o;
}

//返回单位矩阵
void KZMath::KZMatrix44Identity(KZMatrix44& matrix) {
	for (uint32_t i = 0; i < 4; ++i) {
		for (uint32_t j = 0; j < 4; ++j) {
			if (i == j) {
				matrix.m[i][j] = 1;
			}
			else {
				matrix.m[i][j] = 0;
			}
		}
	}
	return;
}

//判断矩阵是否单位矩阵
bool KZMath::KZMatrix44IsIdentity(const KZMatrix44& matrix) {
	for (uint32_t i = 0; i < 4; ++i) {
		for (uint32_t j = 0; j < 4; ++j) {
			if (i == j) {
				if (matrix.m[i][j] != 1) {
					return false;
				}
			}
			else {
				if (matrix.m[i][j] != 0) {
					return false;
				}
			}
		}
	}
	return true;
}

//矩阵转置
void KZMath::KZMATRIX44Transpose(KZMatrix44& matrix, const KZMatrix44& ori_matrix) {
	for (uint32_t i = 0; i < 4; ++i) {
		for (uint32_t j = 0; j < 4; ++j) {
			matrix.m[j][i] = ori_matrix.m[i][j];
		}
	}
	return;
}

//矩阵行列式
float KZMath::KZMatrix44Det(const KZMatrix44& matrix, uint32_t order) {
	if (order == 1) {
		return matrix.m[0][0];
	}
	float ans = 0.0f;
	KZMatrix44 new_matrix;
	for (uint32_t i = 0; i < order; ++i) {

		for (uint32_t j = 0; j < order - 1; ++j) {
			for (uint32_t k = 0; k < order - 1; ++k) {
				new_matrix.m[j][k] = matrix.m[j + 1][(k >= i) ? k + 1 : k];
			}
		}

		float det = KZMatrix44Det(new_matrix, order - 1);
		if (i % 2 == 0) {
			ans += matrix.m[0][i] * det;
		}
		else
		{
			ans -= matrix.m[0][i] * det;
		}
	}
	return ans;
}

//返回逆矩阵
void KZMath::KZMatrix44Inverse(KZMatrix44& matrix, const KZMatrix44& ori_matrix) {
	uint32_t order = 4;
	float det = KZMatrix44Det(ori_matrix, order);
	if (det == 0) {
		return ;
	}
	KZMatrix44Adjoint(matrix, ori_matrix);
	for (uint32_t i = 0; i < order; ++i) {
		for (uint32_t j = 0; j < order; ++j) {
			matrix.m[i][j] /= det;
		}
	}
	return;
}

//返回伴随矩阵
void KZMath::KZMatrix44Adjoint(KZMatrix44& matrix, const KZMatrix44& ori_matrix) {
	KZMatrix44 temp_matrix;
	uint32_t order = 4;
	for (uint32_t i = 0; i < order; ++i) {
		for (uint32_t j = 0; j < order; ++j) {

			for (uint32_t k = 0; k < order - 1; ++k) {
				for (uint32_t t = 0; t < order - 1; ++t) {
					temp_matrix.m[k][t] = ori_matrix.m[(k >= i) ? k + 1 : k][(t >= j) ? t + 1 : t];
				}
			}

			matrix.m[j][i] = KZMatrix44Det(temp_matrix, order - 1);
			if ((i + j) % 2 == 1) {
				matrix.m[j][i] = -matrix.m[j][i];
			}
		}
	}

	return;
}

//返回绕x轴旋转逆矩阵
void KZMath::KZMatrix44RotationXInverse(KZMatrix44& matrix) {
	matrix.m12 = -matrix.m12;
	matrix.m21 = -matrix.m21;
	return;
}

//返回绕x轴旋转矩阵
void KZMath::KZMatrix44RotationX(KZMatrix44& matrix, float angle) {
	KZMatrix44Identity(matrix);
	matrix.m11 = cos(angle);
	matrix.m12 = -sin(angle);
	matrix.m21 = sin(angle);
	matrix.m22 = cos(angle);
	return;
}

//返回绕y轴旋转逆矩阵
void KZMath::KZMatrix44RotationYInverse(KZMatrix44& matrix) {
	matrix.m02 = -matrix.m02;
	matrix.m20 = -matrix.m20;
	return;
}

//返回绕y轴旋转矩阵
void KZMath::KZMatrix44RotationY(KZMatrix44& matrix, float angle) {
	KZMatrix44Identity(matrix);
	matrix.m00 = cos(angle);
	matrix.m02 = sin(angle);
	matrix.m20 = -sin(angle);
	matrix.m22 = cos(angle);
	return;
}

//返回绕z轴旋转逆矩阵
void KZMath::KZMatrix44RotationZInverse(KZMatrix44& matrix) {
	matrix.m01 = -matrix.m01;
	matrix.m10 = -matrix.m10;
	return;
}

//返回绕z轴旋转矩阵
void KZMath::KZMatrix44RotationZ(KZMatrix44& matrix, float angle) {
	KZMatrix44Identity(matrix);
	matrix.m00 = cos(angle);
	matrix.m01 = -sin(angle);
	matrix.m10 = sin(angle);
	matrix.m11 = cos(angle);
	return;
}

//返回绕任意轴旋转矩阵:推导绕x轴旋转到YOZ平面，绕y轴旋转到z轴，绕z轴旋转angle，一二步骤逆操作
void KZMath::KZMatrix44RotationAxis(KZMatrix44& matrix, KZVector4D axis, float angle) {
	KZMatrix44Identity(matrix);
	matrix.m00 = cos(angle) + axis.x * axis.x * (1 - cos(angle));
	matrix.m01 = axis.x * axis.y * (1 - cos(angle)) - axis.z * sin(angle);
	matrix.m02 = axis.x * axis.z * (1 - cos(angle)) + axis.y * sin(angle);

	matrix.m10 = axis.x * axis.y * (1 - cos(angle)) + axis.z * sin(angle);
	matrix.m11 = cos(angle) + axis.y * axis.y * (1 - cos(angle));
	matrix.m12 = axis.y * axis.z * (1 - cos(angle)) - axis.x * sin(angle);

	matrix.m20 = axis.x * axis.z * (1 - cos(angle)) - axis.y * sin(angle);
	matrix.m21 = axis.y * axis.z * (1 - cos(angle)) + axis.x * sin(angle);
	matrix.m22 = cos(angle) + axis.z * axis.z * (1 - cos(angle));
	
	return;
}

void KZMath::KZMatrix44TranslationInverse(KZMatrix44 & matrix)
{
	matrix.m03 = -matrix.m03;
	matrix.m13 = -matrix.m13;
	matrix.m23 = -matrix.m23;
	return;
}

//返回平移矩阵(float x,y,z)
void KZMath::KZMatrix44Translation(KZMatrix44& matrix, float x, float y, float z) {
	KZMatrix44Identity(matrix);
	matrix.m03 = x;
	matrix.m13 = y;
	matrix.m23 = z;
	return;
}

//返回平移矩阵(vector)
void KZMath::KZMatrix44Translation(KZMatrix44& matrix, KZVector4D offset) {
	KZMatrix44Identity(matrix);
	matrix.m03 = offset.x;
	matrix.m13 = offset.y;
	matrix.m23 = offset.z;
	return;
}

void KZMath::KZMatrix44ScalingInverse(KZMatrix44 & matrix)
{
	matrix.m00 = 1 / matrix.m00;
	matrix.m11 = 1 / matrix.m11;
	matrix.m22 = 1 / matrix.m22;
	return;
}

//返回缩放矩阵(float x,y,z)
void KZMath::KZMatrix44Scaling(KZMatrix44& matrix, float scale_x, float scale_y, float scale_z) {
	KZMatrix44Identity(matrix);
	matrix.m00 = scale_x;
	matrix.m11 = scale_y;
	matrix.m22 = scale_z;
	return;
}

//返回缩放矩阵(vector)
void KZMath::KZMatrix44Scaling(KZMatrix44& matrix, KZVector4D scale) {
	KZMatrix44Identity(matrix);
	matrix.m00 = scale.x;
	matrix.m11 = scale.y;
	matrix.m22 = scale.z;
	return;
}