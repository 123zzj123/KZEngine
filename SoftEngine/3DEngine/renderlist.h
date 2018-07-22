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
		KZRenderList* GetInstance() {
			if (instance_ == nullptr) {
				instance_ = new KZRenderList();
			}
			return instance_;
		}

		//删除单实例
		void DeleteInstance() {
			if (instance_) {
				delete instance_;
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
		KZRenderList* instance_ = nullptr;
		KZRenderList();
	};
}
#endif // !KZRENDERLIST
