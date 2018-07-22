#include "util.h"
using namespace KZMath;

//2D参数线段相交
bool KZMath::KZLine2DIntersect(KZLine2DPtr p1, KZLine2DPtr p2, KZPoint2DPtr pt) {
	float det_p1p2 = (p1->v_.x_ * p2->v_.y_ - p1->v_.y_ * p2->v_.x_);
	if (fabs(det_p1p2) <= EPSION_E5) {
		return false;
	}
	float t1, t2;
	t1 = (p2->v_.x_ * (p1->p0_.y_ - p2->p0_.y_) - p2->v_.y_ * (p1->p0_.x_ - p2->p0_.x_));
	t2 = (p1->v_.x_ * (p1->p0_.y_ - p2->p0_.y_) - p1->v_.y_ * (p1->p0_.x_ - p2->p0_.x_));

	if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1) {
		pt->x_ = p1->p0_.x_ + t1 * p1->v_.x_;
		pt->y_ = p1->p0_.y_ + t1 * p1->v_.y_;
		return true;
	}
	else
	{
		return false;
	}
}

//判断点和平面的关系
float KZMath::ComputePoint3DInPLane3D(KZPoint3DPtr pt, KZPlane3DPtr plane) {
	float hs = plane->n_.x_ * (pt->x_ - plane->p0_.x_) + plane->n_.y_ * (pt->y_ - plane->p0_.y_) + plane->n_.z_ * (pt->z_ - plane->p0_.z_);
	return hs;
}

//3D线段与平面相交
bool KZMath::IntersetLine3DInPlane3D(KZLine3DPtr pline, KZPlane3DPtr plane, KZPoint3DPtr pt) {
	float plane_dot_line = KZVector3DDot(plane->n_, pline->v_);

	if (fabs(plane_dot_line) <= EPSION_E5) {
		if (fabs(ComputePoint3DInPLane3D(&pline->p0_, plane)) <= EPSION_E5) {
			pt->x_ = pline->p0_.x_;
			pt->y_ = pline->p0_.y_;
			pt->z_ = pline->p0_.z_;
			return true;
		}
		else
		{
			return false;
		}
	}

	float t = -(plane->n_.x_ * pline->p0_.x_ +
		plane->n_.y_ * pline->p0_.y_ +
		plane->n_.z_ * pline->p0_.z_ -
		plane->n_.x_ * plane->p0_.x_ -
		plane->n_.y_ * plane->p0_.y_ -
		plane->n_.z_ * plane->p0_.z_)/ plane_dot_line;

	if (t >= 0.0 && t <= 1.0) {
		pt->x_ = pline->p0_.x_ + t * pline->v_.x_;
		pt->y_ = pline->p0_.y_ + t * pline->v_.y_;
		pt->z_ = pline->p0_.z_ + t * pline->v_.z_;
		return true;
	}
	else
	{
		return false;
	}
}

//向量点乘
float KZMath::KZVector3DDot(KZVector3D& v1, KZVector3D& v2) {
	return v1.x_ * v2.x_ + v1.y_ * v2.y_ + v1.z_ * v2.z_;
}

//二维向量构造函数
KZMath::Vector2DTyp::Vector2DTyp(float x, float y)
{
	x_ = x;
	y_ = y;
}

//三维向量构造函数
KZMath::Vector3DTyp::Vector3DTyp(float x, float y, float z)
{
	x_ = x;
	y_ = y;
	z_ = z;
}

//KZMath::Line2DTyp::Line2DTyp(const Vector2DTyp& p0, const Vector2DTyp& p1)
//{
//	p0_ = p0;
//	p1_ = p1;
//}
//
//KZMath::Line3DTyp::Line3DTyp(const KZPoint3D& p0, const KZPoint3D& p1, const KZVector3D& v)
//{
//	p0_ = p0;
//	v_ = v;
//}
//
//KZMath::Plane3DTyp::Plane3DTyp(const KZPoint3D& p0, const KZVector3D& n)
//{
//	p0_ = p0;
//	n_ = n;
//}
