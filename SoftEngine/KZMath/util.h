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
	typedef struct VECTOR2D_TYP 
	{
		union
		{
			float m[2];
			struct
			{
				float x, y;
			};
		};
	}KZVector2D, KZPoint2D, *KZVector2D_Ptr, *KZPoint2D_Ptr;

	//3D����(����w)�����
	typedef struct VECTOR3D_TYP
	{
		union
		{
			float m[3];
			struct
			{
				float x, y, z;
			};
		};
	}KZVector3D, KZPoint3D, *KZVector3D_Ptr, *KZPoint3D_Ptr;

	typedef struct Line2D_TYP {
		//2D�߶����
		KZPoint2D p0;
		//2D�߶��յ�
		KZPoint2D p1;
		//2D�߶η�������
		KZVector2D v;
	}KZLine2D, *KZLine2D_Ptr;

	//������3D�߶�
	typedef struct Line3D_TYP {
		//3D�߶����
		KZPoint3D p0;
		//3D�߶��յ�
		KZPoint3D p1;
		//3D�߶η�������
		KZVector3D v;
	}KZLine3D, *KZLine3D_Ptr;

	//������3Dƽ��
	typedef struct PlANE3D_TYP
	{
		//ƽ���ϵĵ�
		KZPoint3D p0;
		//ƽ�淨��
		KZVector3D n;
	}KZPlane3D, *KZPlane3D_Ptr;
	//2D�����߶��ཻ
	bool KZLine2DIntersect(KZLine2D_Ptr p1, KZLine2D_Ptr p2, KZPoint2D_Ptr pt);
	//�жϵ��ƽ��Ĺ�ϵ
	float ComputePoint3DInPLane3D(KZPoint3D_Ptr pt, KZPlane3D_Ptr plane);
	//3D�߶���ƽ���ཻ
	bool IntersetLine3DInPlane3D(KZLine3D_Ptr pline, KZPlane3D_Ptr plane, KZPoint3D_Ptr pt);
	//�������
	float KZVector3DDot(KZVector3D& v1, KZVector3D& v2);
}
#endif