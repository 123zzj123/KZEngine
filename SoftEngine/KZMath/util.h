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

	//2D向量或点
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

	//3D向量(不含w)，或点
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
		//2D线段起点
		KZPoint2D p0;
		//2D线段终点
		KZPoint2D p1;
		//2D线段方向向量
		KZVector2D v;
	}KZLine2D, *KZLine2D_Ptr;

	//参数化3D线段
	typedef struct Line3D_TYP {
		//3D线段起点
		KZPoint3D p0;
		//3D线段终点
		KZPoint3D p1;
		//3D线段方向向量
		KZVector3D v;
	}KZLine3D, *KZLine3D_Ptr;

	//参数化3D平面
	typedef struct PlANE3D_TYP
	{
		//平面上的点
		KZPoint3D p0;
		//平面法线
		KZVector3D n;
	}KZPlane3D, *KZPlane3D_Ptr;
	//2D参数线段相交
	bool KZLine2DIntersect(KZLine2D_Ptr p1, KZLine2D_Ptr p2, KZPoint2D_Ptr pt);
	//判断点和平面的关系
	float ComputePoint3DInPLane3D(KZPoint3D_Ptr pt, KZPlane3D_Ptr plane);
	//3D线段与平面相交
	bool IntersetLine3DInPlane3D(KZLine3D_Ptr pline, KZPlane3D_Ptr plane, KZPoint3D_Ptr pt);
	//向量点乘
	float KZVector3DDot(KZVector3D& v1, KZVector3D& v2);
}
#endif