#pragma once
#ifndef KZ_RENDERLIST
#define KZ_RENDERLIST
#include"object.h"

namespace KZEngine {
	//三角形结构体
	typedef struct TriangleType{
		Vertex vertex_list[3];
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
		//三角形列表
		vector<Triangle> tri_list_;
	private:
		static KZRenderList* r_instance_;
		KZRenderList() {};
	};
}
#endif // !KZ_RENDERLIST
