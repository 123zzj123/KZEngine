#pragma once
#ifndef KZ_PIPELINE
#define KZ_PIPELINE

#include<vector>;
#include"object.h"
#include"renderlist.h"
#include"camera.h"

namespace KZEngine {
	enum Projection
	{
		PERSPECTIVE,
		ORTHOGONAL,
	};
	class KZPipeLine
	{
	public:
		//获取或删除单实例
		static KZPipeLine* GetInstance() {
			if (p_instance_ == nullptr) {
				p_instance_ = new KZPipeLine();
			}
			return p_instance_;
		}

		//删除单实例
		void DeleteInstance() {
			if (p_instance_) {
				delete p_instance_;
			}
		}
		//从文件中加载
		void LoadFromFile();
		//创建cube
		void CreateCube();
		//物体消除，包围球测试
		void CullObject();
		//背面消除
		void RemoveBackface();
		//转化到世界坐标
		void TransformModelToWorld();
		//转化到相机坐标
		void TransformWorldToPer(Projection projection = Projection::PERSPECTIVE);
		//转化到视口坐标
		void TransformPerToViewPort();
		//每帧更新
		void FrameUpdate();
		//初始化
		void Initial();
		//光栅化与深度测试
		void RasterizationDepthTest();
	public:
		//视口宽度
		uint32_t view_width;
		//视口高度
		uint32_t view_height;
		//物体数目
		uint32_t object_num_;
		//三角形数量
		uint32_t tri_num_;
		//存储物体数组
		vector<KZObject> object_vec_;
		//渲染队列
		KZRenderList* render_list_;
		KZCamera main_camera_;
		vector<bool> object_active_;
		float** z_buffer_;
	private:
		//光栅化平底三角形
		void DrawBottomTri(const KZMath::KZVector4D& v0, const KZMath::KZVector4D& v1, const KZMath::KZVector4D& v2);
		//光栅化平顶三角形
		void DrawTopTri(const KZMath::KZVector4D& v0, const KZMath::KZVector4D& v1, const KZMath::KZVector4D& v2);
		static KZPipeLine* p_instance_;
		KZPipeLine() {
			render_list_ = NULL;
			main_camera_.pos_.Set(3.0f, 1.5f, 3);
			Initial();
		};
	};
}
#endif // !KZ_PIPELINE
