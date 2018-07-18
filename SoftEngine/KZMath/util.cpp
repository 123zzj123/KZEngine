#include "util.h"
using namespace KZMath;

//2D参数线段相交
bool KZMath::KZLine2DIntersect(KZLine2D_Ptr p1, KZLine2D_Ptr p2, KZPoint2D_Ptr pt) {
	float det_p1p2 = (p1->v.x * p2->v.y - p1->v.y * p2->v.x);
	if (fabs(det_p1p2) <= EPSION_E5) {
		return false;
	}
	float t1, t2;
	t1 = (p2->v.x * (p1->p0.y - p2->p0.y) - p2->v.y * (p1->p0.x - p2->p0.x));
	t2 = (p1->v.x * (p1->p0.y - p2->p0.y) - p1->v.y * (p1->p0.x - p2->p0.x));

	if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1) {
		pt->x = p1->p0.x + t1 * p1->v.x;
		pt->y = p1->p0.y + t1 * p1->v.y;
		return true;
	}
	else
	{
		return false;
	}
}

//判断点和平面的关系
float KZMath::ComputePoint3DInPLane3D(KZPoint3D_Ptr pt, KZPlane3D_Ptr plane) {
	float hs = plane->n.x * (pt->x - plane->p0.x) + plane->n.y * (pt->y - plane->p0.y) + plane->n.z * (pt->z - plane->p0.z);
	return hs;
}

//3D线段与平面相交
bool KZMath::IntersetLine3DInPlane3D(KZLine3D_Ptr pline, KZPlane3D_Ptr plane, KZPoint3D_Ptr pt) {
	float plane_dot_line = KZVector3DDot(plane->n, pline->v);

	if (fabs(plane_dot_line) <= EPSION_E5) {
		if (fabs(ComputePoint3DInPLane3D(&pline->p0, plane)) <= EPSION_E5) {
			pt->x = pline->p0.x;
			pt->y = pline->p0.y;
			pt->z = pline->p0.z;
			return true;
		}
		else
		{
			return false;
		}
	}

	float t = -(plane->n.x * pline->p0.x +
		plane->n.y * pline->p0.y +
		plane->n.z * pline->p0.z -
		plane->n.x * plane->p0.x -
		plane->n.y * plane->p0.y -
		plane->n.z * plane->p0.z)/ plane_dot_line;

	if (t >= 0.0 && t <= 1.0) {
		pt->x = pline->p0.x + t * pline->v.x;
		pt->y = pline->p0.y + t * pline->v.y;
		pt->z = pline->p0.z + t * pline->v.z;
		return true;
	}
	else
	{
		return false;
	}
}

//向量点乘
float KZMath::KZVector3DDot(KZVector3D& v1, KZVector3D& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}