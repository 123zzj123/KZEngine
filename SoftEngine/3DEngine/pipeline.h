#pragma once
#ifndef KZ_PIPELINE
#define KZ_PIPELINE

#include<windows.h>
#include<vector>;
#include"../KZMath/fix32.h"
#include"object.h"
#include"renderlist.h"
#include"camera.h"
#include"material.h"
//#include<thread>
//#include<mutex>

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
		void LoadFromFile();
		//创建cube
		void CreateCube(float width = 1.0f, float length = 1.0f, float height = 1.0f,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(0, 0, -5), 
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO, 
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1), 
			bool is_light = false);
		//创建圆柱
		void CreateCylinder(float top_radius, float bottom_radius, float height, uint32_t stack, uint32_t slice,
			const KZEngine::Color& ini_color,
			bool is_light = false,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(0,0,0),
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO, 
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1));
		//创建球体
		void CreateSphere(float radius, uint32_t stack, uint32_t slice,
			const KZEngine::Color& ini_color,
			bool is_light = false,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(0, 0, 0),
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO,
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1));
		//创建三棱锥
		void CreatePyramid(const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(2, 0, -7), 
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO, 
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1), 
			bool is_light = true);
		//创建地形
		void Create_Terrain(float width, float height, float vscale, 
			const char* height_map_file_name, const char* texture_map_file_name, 
			const KZEngine::Color& ini_color, 
			bool is_light = false,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(), 
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO
			);
		//改变光状态
		void ChangeLight(uint32_t light_index);
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
	public:
		//主摄像机
		KZCamera main_camera_;
		//鼠标上一位置的x
		int last_pos_x_;
		//鼠标上一位置的y
		int last_pos_y_;
		//是否首次按下鼠标左键
		bool first_mouse_;
	protected:
		//物体消除，包围球测试
		void OcclusionCulling();
		//背面消除
		void BackfaceCulling();
		//三角形裁剪
		void PolyCulling();
		//转化到世界坐标
		void TransformModelToWorld();
		//转化到相机坐标
		void TransformWorldToPer(Projection projection = Projection::PERSPECTIVE);
		//转化到视口坐标
		void TransformPerToViewPort();
		//初始化
		void Initial();
		//浮点数版本光栅化与深度测试
		void RasterizationDepthTest();
		//整数版本光栅化与深度测试
		void RasterizationDepthTestFast();
		//浮点数版本光栅化平底三角形
		void DrawBottomTri(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id = -1);
		//整数版本光栅化平底三角形
		void DrawBottomTriFast(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id = -1);
		//浮点数版本光栅化平顶三角形
		void DrawTopTri(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id = -1);
		//整数版本光栅化平顶三角形
		void DrawTopTriFast(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id = -1);
		//双缓冲交换
		void SwapBuffer();
	private:
		//光源数组
		vector<LightBase*> light_vec_;
		//bool数组
		vector<bool> light_active_vec_;
		//视口宽度
		uint32_t view_width_;
		//视口高度
		uint32_t view_height_;
		//物体数目
		uint32_t object_num_;
		//三角形数量
		uint32_t tri_num_;
		//存储物体数组
		vector<KZObject> object_vec_;
		//渲染队列
		KZRenderList* render_list_;
		//对象积极
		vector<bool> object_active_;
		//深度缓冲
		float* z_buffer_;
		//失败的多线程光栅化
		//mutex 缓冲
		//mutex* mutex_buffer_;
		//帧缓冲
		unsigned char* frame_buffer_;
		//渲染窗口句柄
		HWND hwnd_;
		//平均fps
		float fps_;
		//记录的fps数量
		float fps_count_;
		//fps记录时间
		DWORD record_time_;
		//材质数组
		vector<KZEngine::KZMaterial> mat_vec_;
		//对象id
		uint32_t obj_id_ = 1;
	private:
		//单实例
		static KZPipeLine* p_instance_;
		//构造函数
		KZPipeLine();
		//析构函数
		~KZPipeLine();
		//增加材质
		uint32_t AddMaterial(const KZEngine::KZMaterial &);
	};
}
#endif // !KZ_PIPELINE
