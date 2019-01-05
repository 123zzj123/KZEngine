#pragma once
#ifndef KZUTIL
#define KZUTIL
#define EPSILON_E5 (float)(1E-5)
#include<cmath>

using namespace std;

namespace KZMath {

	/*
	*	math tools utility including vector3d,point3d,line3d, plane3d and etc;
	*/

	//2D向量或点
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
		//构造函数
		Vector2DTyp(float x = 0.0f, float y = 0.0f);
	}KZVector2D, KZPoint2D, *KZVector2DPtr, *KZPoint2DPtr;

	//3D向量(不含w)，或点
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
		//构造函数
		Vector3DTyp(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	}KZVector3D, KZPoint3D, *KZVector3DPtr, *KZPoint3DPtr;

	typedef struct Line2DTyp {
		//2D线段起点
		KZPoint2D p0_;
		//2D线段终点
		KZPoint2D p1_;
		//2D线段方向向量
		KZVector2D v_;
	}KZLine2D, *KZLine2DPtr;

	//参数化3D线段
	typedef struct Line3DTyp {
		//3D线段起点
		KZPoint3D p0_;
		//3D线段终点
		KZPoint3D p1_;
		//3D线段方向向量
		KZVector3D v_;
	}KZLine3D, *KZLine3DPtr;

	//参数化3D平面
	typedef struct Plane3DTyp
	{
		//平面上的点
		KZPoint3D p0_;
		//平面法线
		KZVector3D n_;

		Plane3DTyp(float p0_x = 0.0f, float p0_y = 0.0f, float p0_z = 0.0f, float n_x = 0.0f, float n_y = 0.0f, float n_z = 0.0f) {
			p0_.x_ = p0_x;
			p0_.y_ = p0_y;
			p0_.z_ = p0_z;

			n_.x_ = n_x;
			n_.y_ = n_y;
			n_.z_ = n_z;
		}
	}KZPlane3D, *KZPlane3DPtr;
	//2D参数线段相交
	bool KZLine2DIntersect(KZLine2DPtr p1, KZLine2DPtr p2, KZPoint2DPtr pt);
	//判断点和平面的关系
	float ComputePoint3DInPLane3D(KZPoint3DPtr pt, KZPlane3DPtr plane);
	//3D线段与平面相交
	bool IntersetLine3DInPlane3D(KZLine3DPtr pline, KZPlane3DPtr plane, KZPoint3DPtr pt, float& t);
	//向量点乘
	float KZVector3DDot(KZVector3D& v1, KZVector3D& v2);
}
#endif