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
	}Triangle, *TrianglePtr;

	//渲染列表
	class KZRenderList
	{
	public:
		//创建或获取单实例
		static KZRenderList* GetInstance() {
			if (r_instance_ == nullptr) {
				r_instance_ = new KZRenderList();
			}
			return r_instance_;
		}

		//删除单实例
		void DeleteInstance() {
			if (r_instance_) {
				delete r_instance_;
			}
		}
	public:
		//矩阵转换
		void Transform(const KZMath::KZMatrix44& matrix);
		//三角形面数
		uint32_t num_poly_;
		//透明三角形面数
		uint32_t transparent_num_poly_;
		//三角形列表
		vector<Triangle> tri_list_;
		//透明三角形列表
		vector<Triangle> transparent_tri_list_;
		//活跃三角形列表,裁剪标识
		vector<bool> active_tri_;
		//活跃三角形列表,裁剪标识
		vector<bool> transparent_active_tri_;
	private:
		static KZRenderList* r_instance_;
		KZRenderList() {};
	};
}
#endif // !RENDERLIST_H
