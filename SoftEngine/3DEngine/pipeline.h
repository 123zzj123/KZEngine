#pragma once
#ifndef KZ_PIPELINE
#define KZ_PIPELINE

#include<windows.h>
#include<vector>;
#include"../KZMath/fix32.h"
#include"camera.h"
#include"material.h"
#include "bhv.h"
#include"bsp.h"
#include "pass.h"
#include<list>
#include <future>
#include <limits.h>
//#include<thread>
//#include<mutex>

namespace KZEngine {
	enum class Projection
	{
		PERSPECTIVE,
		ORTHOGONAL,
	};

	enum class RenderMode
	{
		NORMAL,
		WIREFRAME,
	};

	enum class CalCulateShadow
	{
		NONE,
		SHADOWTEXTURE,
		VERTEXMAPPING,
	};

	enum class SceneManage
	{
		NONE,
		BHV,
		BSP
	};

	class KZPipeLine
	{
		friend class KZBHV;
		friend class KZBSP;
	public:
		//获取或删除单实例
		static KZPipeLine* GetInstance() {
			if (p_instance_ == nullptr) {
				p_instance_ = new KZPipeLine();
			}
			return p_instance_;
		}

		//删除单实例
		static void DeleteInstance() {
			if (p_instance_) {
				delete p_instance_;
			}
		}
		//每帧更新
		void FrameUpdate();
		//设置fps
		void SetGameFps();
		//从文件中加载
		void LoadFromFile(const char* file_name, int32_t pass_id = 0, float alpha = 1.0f,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(),
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO,
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1),
			string shadow_map = "");
		//创建cube
		void CreateCube(float width = 1.0f, float length = 1.0f, float height = 1.0f,
			bool light_cube = false,
			int32_t pass_id = -1,
			bool is_light = false,
			float alpha = 1.0f,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(0, 0, -5),
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO,
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1),
			string shadow_map = "");
		//创建圆柱
		void CreateCylinder(float top_radius, float bottom_radius, float height, uint32_t stack, uint32_t slice,
			const KZEngine::Color& ini_color,
			int32_t pass_id = -1,
			bool is_light = false,
			float alpha = 1.0f,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(0,0,0),
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO, 
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1),
			string shadow_map = "");
		//创建球体
		void CreateSphere(float radius, uint32_t stack, uint32_t slice,
			const KZEngine::Color& ini_color,
			int32_t pass_id = -1,
			bool is_light = false,
			float alpha = 1.0f,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(0, 0, 0),
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO,
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1),
			string shadow_map = "");
		//创建三棱锥
		void CreatePyramid(
			int32_t pass_id = -1,
			bool is_light = true,
			float alpha = 1.0f,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(2, 0, -7),
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO,
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1),
			string shadow_map = "");
		//创建地形
		void Create_Terrain(float width, float height, float vscale, 
			const char* height_map_file_name, const char* texture_map_file_name, 
			const KZEngine::Color& ini_color, 
			int32_t pass_id = -1,
			bool is_light = false,
			float alpha = 1.0f,
			bool is_static = true,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(), 
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO
			);
		//创建四叉树地形
		void Create_QuadTerrain(float width, float height, float vscale,
			const char* height_map_file_name, const char* texture_map_file_name,
			const KZEngine::Color& ini_color,
			int32_t pass_id = -1,
			bool is_light = false,
			float alpha = 1.0f,
			bool is_static = true,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(),
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO
		);
		//改变光状态
		void ChangeLight(uint32_t light_index);
		//设置计算阴影的方式
		inline void SetCalculateShadow(const CalCulateShadow& calculate_shadow)
		{
			calculate_shadow_ = calculate_shadow;
		}
		//获取hwnd
		inline HWND GetWindowHwnd() {
			return hwnd_;
		};
		inline void SetWindowHwnd(HWND hwnd) {
			hwnd_ = hwnd;
		}
		//获取窗口宽度
		inline uint32_t GetWindowWidth() {
			return view_width_;
		}
		//获取窗口高度
		inline uint32_t GetWindowHeight() {
			return view_height_;
		}
		//获取帧缓冲
		inline unsigned char* GetFrameBuffer() {
			return frame_buffer_;
		}
		inline void SetSceneManage(SceneManage scene_manage) {
			scene_manage_ = scene_manage;
			switch (scene_manage_)
			{
			case KZEngine::SceneManage::NONE:
				break;
			case KZEngine::SceneManage::BHV:
				KZBHV::BuildSceneBHVTree(bhv_root_, 0, 2);
				break;
			case KZEngine::SceneManage::BSP:
				break;
			default:
				break;
			}
		}
		void AddSkyBox(const char* front_img, const char* back_img, const char* left_img, const char* right_img, const char* top_img, const char* bottom_img);
	public:
		//主摄像机
		KZCamera* main_camera_;
		//鼠标上一位置的x
		int last_pos_x_;
		//鼠标上一位置的y
		int last_pos_y_;
		//是否首次按下鼠标左键
		bool first_mouse_ = false;
	protected:
		//场景剔除：物体剔除，背面消除
		void SceneManageCulling(int32_t pass_id);
		//物体消除，包围球测试
		void OcclusionCulling(int32_t pass_id);
		//背面消除
		void BackfaceCulling(int32_t pass_id);
		//三角形裁剪
		void PolyCulling(int32_t pass_id);
		//转化到世界坐标
		void TransformModelToWorld(int32_t pass_id);
		//转化到相机坐标
		void TransformWorldToPer(int32_t pass_id, Projection projection = Projection::PERSPECTIVE);
		//转化到视口坐标
		void TransformPerToViewPort(int32_t pass_id);
		//浮点数版本光栅化与深度测试
		void RasterizationDepthTest();
		//整数版本光栅化与深度测试
		void RasterizationDepthTestFast();
		//浮点数版本光栅化平底三角形
		void DrawBottomTri(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id = -1, float alpha = 1.0f);
		//整数版本光栅化平底三角形
		void DrawBottomTriFast(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id = -1, float alpha = 1.0f);
		//浮点数版本光栅化平顶三角形
		void DrawTopTri(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id = -1, float alpha = 1.0f);
		//整数版本光栅化平顶三角形
		void DrawTopTriFast(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id = -1, float alpha = 1.0f);
		//双缓冲交换
		void SwapBuffer();
		//增加阴影位图物体
		void addShadowObj(const string& shadow_name, const string& shadow_map_texture, const KZMath::KZVector4D<float>& ori_world_pos, const float radius);
		//获取场景AABB包围盒
		void getSceneAABB(bool include_transparent = false);
		//更新动态Mesh
		void UpdateObjMesh(int32_t pass_id);
	private:
		//多少次pass
		int32_t pass_num_ = 0;
		//当前pass
		int32_t pass_idx_ = -1;
		//Pass数组，记录Pass属性
		vector<Pass*> pass_vec_;
		//光源数组
		vector<LightBase*> light_vec_;
		//bool数组
		vector<bool> light_active_vec_;
		//视口宽度
		uint32_t view_width_ = 0;
		//视口高度
		uint32_t view_height_ = 0;
		//shadow map buffer高度
		uint32_t shadow_map_width_ = 0;
		//shadow map buffer宽度
		uint32_t shadow_map_height_ = 0;
		
		//深度缓冲
		float* z_buffer_ = nullptr;
		//深度贴图缓冲
		float* depth_map_buffer_ = nullptr;
		//失败的多线程光栅化
		//mutex 缓冲
		//mutex* mutex_buffer_;
		
		//帧缓冲
		unsigned char* frame_buffer_ = nullptr;
		//渲染窗口句柄
		HWND hwnd_;
		//平均fps
		float fps_ = 0.0f;
		//记录的fps数量
		float fps_count_ = 0.0f;
		//fps记录时间
		DWORD record_time_ = 0;
		//材质数组
		vector<KZEngine::KZMaterial> mat_vec_;
		//对象id
		uint32_t obj_id_ = 1;
		//渲染管线计算阴影的方式 0表示不计算，1表示平面网格阴影映射，2表示shadow mapping
		CalCulateShadow calculate_shadow_ = CalCulateShadow::NONE;
		//AABB min
		KZMath::KZVector4D<float> aabb_min_ = KZMath::KZVector4D<float>(0, 0, 0, 1);
		//AABB max
		KZMath::KZVector4D<float> aabb_max_ = KZMath::KZVector4D<float>(0, 0, 0, 1);
		//BHV树根节点
		BHVNodePtr bhv_root_ = nullptr;
		//场景管理方式
		SceneManage scene_manage_ = SceneManage::BHV;
		//渲染模式，正常or线框
		RenderMode render_mode_ = RenderMode::WIREFRAME;
		//多线程
		future<void>* future_pass_arrs_;
		bool mutiple_thread_ = false;
	private:
		//单实例
		static KZPipeLine* p_instance_;
		//构造函数
		KZPipeLine(uint32_t renderlist_num = 2);
		//析构函数
		~KZPipeLine();
		//增加材质
		uint32_t AddMaterial(const KZEngine::KZMaterial &);
	};
}
#endif // !KZ_PIPELINE
