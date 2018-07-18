#include"vector.h"
#include"matrix.h"
#include"quat.h"

int main() {
	KZMath::KZQuat q1(1, 2, 3, 4);
 	cout << q1.qv.x << endl;
	system("pause");
}

/*KZMath::KZVector4D v1(1, 0, 1, 1);
	KZMath::KZVector4D v2(2, 1, 2, 1);
	KZMath::KZVector4D v3;
	KZMath::KZVector4D v4;
	v1 *= v2;
	cout << v1 << endl;
	cout << KZMath::KZVector4D3AngleBetweenVectors(v1, v2) << endl;
	KZMath::KZVector4DSplatOne(v1);
	cout << v1 << endl;
	KZMath::KZVector4D3Orthogonal(v3, v1);
	cout << v3 * v1 << endl;
	KZMath::KZVector4D3Cross(v4, v1, v2);
	cout << v4 << endl;
	KZMath::KZVector4DSet(v4, 2, 2, 2);
	cout << v4 << endl;
	KZMath::KZVector4D3Normalize(v4);
	cout << v4 << " length: " << KZMath::KZVector4D3Length(v4) << endl;
	KZMath::KZVector4DReplicate(v2, 2.24);
	cout << v2 << endl;
	v3 = -v2;
	cout << v3 << endl;
	v3 /= 2;
	cout << v3 << endl;
	cout << (v3 == v2) << endl;*/

/*KZMath::KZVector4D v1(1, 2, 1, 1);
	KZMath::KZVector4D v2(2, 1, 1, 1);
	KZMath::KZVector4D v3(3, 1, 1, 2);
	KZMath::KZVector4D v4;
	KZMath::KZMatrix44 matrix1(v1, v2, v3, v4);
	KZMath::KZMatrix44 matrix3 = KZMath::KZMatrix44();
	cout << matrix1 << endl;
	float f_array[16] = { 0,2,3,1,0,0,0,1,1,2,0,0,2,2,3,1 };
	KZMath::KZMatrix44 matrix2(f_array);
	cout << matrix2 << endl;
	cout << matrix1 * matrix2 << endl;
	cout << matrix1 << endl;
	matrix1 *= matrix1;
	cout << matrix1 << endl;
	cout << KZMath::KZMatrix44IsIdentity(matrix1) << endl;
	KZMath::KZMatrix44Identity(matrix1);
	cout << matrix1 << endl;
	KZMath::KZMATRIX44Transpose(matrix3, matrix2);
	cout << matrix3 << endl;
	cout << KZMath::KZMatrix44Det(matrix2, 4) << endl;
	KZMath::KZMatrix44Inverse(matrix3, matrix2);
	cout << matrix3 << endl;
	KZMath::KZMatrix44Translation(matrix1, v1);
	cout << matrix1 << endl;
	KZMath::KZMatrix44TranslationInverse(matrix1);
	cout << matrix1 << endl;
	KZMath::KZMatrix44Scaling(matrix1, v2);
	cout << matrix1 << endl;
	KZMath::KZMatrix44ScalingInverse(matrix1);
	cout << matrix1 << endl;
	float angle = 30;
	KZMath::KZMatrix44RotationX(matrix1, angle);
	cout << matrix1 << endl;*/

/*  
	KZMath::KZVector4D v1(1, 2, 1, 1);
	KZMath::KZVector4D v2(2, 1, 1, 1);
	KZMath::KZVector4D v3(3, 1, 1, 2);
	KZMath::KZVector4D v4;
	KZMath::KZMatrix44 matrix1(v1, v2, v3, v4);
	KZMath::KZMatrix44 matrix3 = KZMath::KZMatrix44();
	cout << matrix1 << endl;
	float f_array[16] = { 0,2,3,1,0,0,0,1,1,2,0,0,2,2,3,1 };
	KZMath::KZMatrix44 matrix2(f_array);
	cout << matrix2 << endl;
	cout << matrix1 * matrix2 << endl;
	cout << matrix1 << endl;
	matrix1 *= matrix1;
	cout << matrix1 << endl;
	cout << KZMath::KZMatrix44IsIdentity(matrix1) << endl;
	KZMath::KZMatrix44Identity(matrix1);
	cout << matrix1 << endl;
	KZMath::KZMATRIX44Transpose(matrix3, matrix2);
	cout << matrix3 << endl;
	cout << KZMath::KZMatrix44Det(matrix2, 4) << endl;
	KZMath::KZMatrix44Inverse(matrix3, matrix2);
	cout << matrix3 << endl;
	KZMath::KZMatrix44Translation(matrix1, v1);
	cout << matrix1 << endl;
	KZMath::KZMatrix44TranslationInverse(matrix1);
	cout << matrix1 << endl;
	KZMath::KZMatrix44Scaling(matrix1, v2);
	cout << matrix1 << endl;
	KZMath::KZMatrix44ScalingInverse(matrix1);
	cout << matrix1 << endl;
	KZVector4DSet(v1, 1, 0, 0);
	cout << v1 << endl;
	float angle = 40;
	KZMath::KZMatrix44RotationAxis(matrix1, v1, angle);
	cout << matrix1 << endl;*/