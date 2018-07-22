#pragma once
#ifndef KZ_PIPELINE
#define KZ_PIPELINE

#include<vector>;
#include"object.h"
#include"renderlist.h"
#include"camera.h"

namespace KZEngine {
	class KZPipeLine
	{
	public:
		//获取或删除单实例
		KZPipeLine* GetInstance() {
			if (instance_ == nullptr) {
				instance_ = new KZPipeLine();
			}
			return instance_;
		}

		//删除单实例
		void DeleteInstance() {
			if (instance_) {
				delete instance_;
			}
		}
		//从文件中加载
		void LoadFromFile();
		//物体消除，包围球测试
		void CullObject();
		//背面消除
		void RemoveBackface();
		void Initial();
	public:
		uint32_t object_num_;
		//存储物体数组
		vector<KZObject> object_vec_;
		//渲染队列
		KZRenderList* render_list_;
		KZCamera main_camera_;
		vector<bool> object_active_;
	private:
		KZPipeLine* instance_ = nullptr;
		KZPipeLine();
	};
}
#endif // !KZ_PIPELINE
