#pragma once
#ifndef RENDERLIST_H
#define RENDERLIST_H
#include"object.h"

namespace KZEngine {

	//三角形结构体
	typedef struct TriangleType{
		//顶点数组
		Vertex vertex_list[3];
		//法线
		KZMath::KZVector4D<float> face_normal;
		//材质id
		uint32_t material = -1;
		//透明度
		float alpha = 1.0f;
		//三角形裁剪活跃标记
		bool active = false;
		//true为结构化面,参与bsp，false为细节面
		bool bsp_split = false;
		//bsp分割面id
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
