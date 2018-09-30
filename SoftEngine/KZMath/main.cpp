#include"vector.h"
#include"matrix.h"
#include"quat.h"

int main() {
	KZMath::KZVector4D<float> v1(1, 2, 1, 1);
	KZMath::KZVector4D<float> v2(2, 1, 1, 1);
	KZMath::KZVector4D<float> v3(3, 1, 1, 2);
	KZMath::KZVector4D<float> v4;
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
	cout << matrix1.IsIdentity() << endl;
	matrix1.Identity();
	cout << matrix1 << endl;
	matrix2.Transpose(matrix3);
	cout << matrix3 << endl;
	cout << matrix2.Det(4) << endl;
	matrix2.Inverse(matrix3);
	cout << matrix3 << endl;
	matrix1.Translation(v1);
	cout << matrix1 << endl;
	matrix1.TranslationInverse();
	cout << matrix1 << endl;
	matrix1.Scaling(v2);
	cout << matrix1 << endl;
	matrix1.ScalingInverse();
	cout << matrix1 << endl;
	float angle = 30;
	matrix1.RotationZ(KZMath::AngleToRadian(30));
	cout << matrix1 << endl;
	system("pause");
}

//KZMath::KZQuat q1(1, 2, 3, 4);
//	KZMath::KZQuat q2;
//	cout << q2 << endl;
//	q1.Conjugate(q2);
// 	cout << q2 << endl;
//	cout << "q1 norm: " << q1.Norm() << endl;
//	q1.Normalize();
//	cout << q1 << endl;
//	cout << "q1 norm: " << q1.Norm() << endl;
//	q1.Inverse(q2);
//	cout << q2 << endl;

//KZMath::KZVector4D v1(1, 0, 1, 1);
//KZMath::KZVector4D v2(2, 1, 2, 1);
//KZMath::KZVector4D v3;
//KZMath::KZVector4D v4;
//v1 *= v2;
//cout << v1 << endl;
//cout << v1.Vector3AngleBetweenVectors(v2) << endl;
//v1.SplatOne();
//cout << v1 << endl;
//v1.Vector3Orthogonal(v3);
//cout << v3.Vector3Dot(v1) << endl;
//v1.Vector3Cross(v4, v2);
//cout << v4 << endl;
//v4.Set(2, 2, 2);
//cout << v4 << endl;
//v4.Vector3Normalize();
//cout << v4 << " length: " << v4.Vector3LengthSq() << endl;
//v2.Replicate(2.24);
//cout << v2 << endl;
//v3 = -v2;
//cout << v3 << endl;
//v3 /= 2;
//cout << v3 << endl;
//cout << (v3 == v2) << endl;

