#pragma once
#ifndef RENDERLIST_H
#define RENDERLIST_H
#include"object.h"

namespace KZEngine {
	//三角形结构体
	typedef struct TriangleType{
		Vertex vertex_list[3];
		KZMath::KZVector4D<float> face_normal;
		uint32_t material = -1;
		float alpha = 1.0f;
		bool active = false;
		bool bsp_split = false;
		uint32_t bsp_split_id = 0;
	}Triangle, *TrianglePtr;

	//渲染列表
	class KZRenderList
	{
		friend class Pass;
	private:
		KZRenderList() {};
		~KZRenderList();
		//矩阵转换
		void Transform(const KZMath::KZMatrix44& matrix);
	public:
		//三角形列表
		vector<TrianglePtr> tri_list_;
		//是否透明物体渲染队列
		bool transparent = false;
	};
}
#endif // !RENDERLIST_H
