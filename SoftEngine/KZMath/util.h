#pragma once
#ifndef KZUTIL
#define KZUTIL
#define EPSION_E5 (float)(1E-5)
#include<cmath>

using namespace std;

namespace KZMath {

	/*
	*	math tools utility including vector3d,point3d,line3d, plane3d and etc;
	*/

	//2D�������
	typedef struct Vector2DTyp
	{
		union
		{
			float m_[2];
			struct
			{
				float x_, y_;
			};
		};
		//���캯��
		Vector2DTyp(float x = 0.0f, float y = 0.0f);
	}KZVector2D, KZPoint2D, *KZVector2DPtr, *KZPoint2DPtr;

	//3D����(����w)�����
	typedef struct Vector3DTyp
	{
		union
		{
			float m_[3];
			struct
			{
				float x_, y_, z_;
			};
		};
		//���캯��
		Vector3DTyp(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	}KZVector3D, KZPoint3D, *KZVector3DPtr, *KZPoint3DPtr;

	typedef struct Line2DTyp {
		//2D�߶����
		KZPoint2D p0_;
		//2D�߶��յ�
		KZPoint2D p1_;
		//2D�߶η�������
		KZVector2D v_;
	}KZLine2D, *KZLine2DPtr;

	//������3D�߶�
	typedef struct Line3DTyp {
		//3D�߶����
		KZPoint3D p0_;
		//3D�߶��յ�
		KZPoint3D p1_;
		//3D�߶η�������
		KZVector3D v_;
	}KZLine3D, *KZLine3DPtr;

	//������3Dƽ��
	typedef struct Plane3DTyp
	{
		//ƽ���ϵĵ�
		KZPoint3D p0_;
		//ƽ�淨��
		KZVector3D n_;
	}KZPlane3D, *KZPlane3DPtr;
	//2D�����߶��ཻ
	bool KZLine2DIntersect(KZLine2DPtr p1, KZLine2DPtr p2, KZPoint2DPtr pt);
	//�жϵ��ƽ��Ĺ�ϵ
	float ComputePoint3DInPLane3D(KZPoint3DPtr pt, KZPlane3DPtr plane);
	//3D�߶���ƽ���ཻ
	bool IntersetLine3DInPlane3D(KZLine3DPtr pline, KZPlane3DPtr plane, KZPoint3DPtr pt);
	//�������
	float KZVector3DDot(KZVector3D& v1, KZVector3D& v2);
}
#endif