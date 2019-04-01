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
		//��ȡ��ɾ����ʵ��
		static KZPipeLine* GetInstance() {
			if (p_instance_ == nullptr) {
				p_instance_ = new KZPipeLine();
			}
			return p_instance_;
		}

		//ɾ����ʵ��
		static void DeleteInstance() {
			if (p_instance_) {
				delete p_instance_;
			}
		}
		//ÿ֡����
		void FrameUpdate();
		//����fps
		void SetGameFps();
		//���ļ��м���
		void LoadFromFile(const char* file_name, int32_t pass_id = 0, float alpha = 1.0f,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(),
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO,
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1),
			string shadow_map = "");
		//����cube
		void CreateCube(float width = 1.0f, float length = 1.0f, float height = 1.0f,
			bool light_cube = false,
			int32_t pass_id = -1,
			bool is_light = false,
			float alpha = 1.0f,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(0, 0, -5),
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO,
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1),
			string shadow_map = "");
		//����Բ��
		void CreateCylinder(float top_radius, float bottom_radius, float height, uint32_t stack, uint32_t slice,
			const KZEngine::Color& ini_color,
			int32_t pass_id = -1,
			bool is_light = false,
			float alpha = 1.0f,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(0,0,0),
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO, 
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1),
			string shadow_map = "");
		//��������
		void CreateSphere(float radius, uint32_t stack, uint32_t slice,
			const KZEngine::Color& ini_color,
			int32_t pass_id = -1,
			bool is_light = false,
			float alpha = 1.0f,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(0, 0, 0),
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO,
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1),
			string shadow_map = "");
		//��������׶
		void CreatePyramid(
			int32_t pass_id = -1,
			bool is_light = true,
			float alpha = 1.0f,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(2, 0, -7),
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO,
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1),
			string shadow_map = "");
		//��������
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
		//�����Ĳ�������
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
		//�ı��״̬
		void ChangeLight(uint32_t light_index);
		//���ü�����Ӱ�ķ�ʽ
		inline void SetCalculateShadow(const CalCulateShadow& calculate_shadow)
		{
			calculate_shadow_ = calculate_shadow;
		}
		//��ȡhwnd
		inline HWND GetWindowHwnd() {
			return hwnd_;
		};
		inline void SetWindowHwnd(HWND hwnd) {
			hwnd_ = hwnd;
		}
		//��ȡ���ڿ��
		inline uint32_t GetWindowWidth() {
			return view_width_;
		}
		//��ȡ���ڸ߶�
		inline uint32_t GetWindowHeight() {
			return view_height_;
		}
		//��ȡ֡����
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
		//�������
		KZCamera* main_camera_;
		//�����һλ�õ�x
		int last_pos_x_;
		//�����һλ�õ�y
		int last_pos_y_;
		//�Ƿ��״ΰ���������
		bool first_mouse_ = false;
	protected:
		//�����޳��������޳�����������
		void SceneManageCulling(int32_t pass_id);
		//������������Χ�����
		void OcclusionCulling(int32_t pass_id);
		//��������
		void BackfaceCulling(int32_t pass_id);
		//�����βü�
		void PolyCulling(int32_t pass_id);
		//ת������������
		void TransformModelToWorld(int32_t pass_id);
		//ת�����������
		void TransformWorldToPer(int32_t pass_id, Projection projection = Projection::PERSPECTIVE);
		//ת�����ӿ�����
		void TransformPerToViewPort(int32_t pass_id);
		//�������汾��դ������Ȳ���
		void RasterizationDepthTest();
		//�����汾��դ������Ȳ���
		void RasterizationDepthTestFast();
		//�������汾��դ��ƽ��������
		void DrawBottomTri(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id = -1, float alpha = 1.0f);
		//�����汾��դ��ƽ��������
		void DrawBottomTriFast(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id = -1, float alpha = 1.0f);
		//�������汾��դ��ƽ��������
		void DrawTopTri(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id = -1, float alpha = 1.0f);
		//�����汾��դ��ƽ��������
		void DrawTopTriFast(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id = -1, float alpha = 1.0f);
		//˫���彻��
		void SwapBuffer();
		//������Ӱλͼ����
		void addShadowObj(const string& shadow_name, const string& shadow_map_texture, const KZMath::KZVector4D<float>& ori_world_pos, const float radius);
		//��ȡ����AABB��Χ��
		void getSceneAABB(bool include_transparent = false);
		//���¶�̬Mesh
		void UpdateObjMesh(int32_t pass_id);
	private:
		//���ٴ�pass
		int32_t pass_num_ = 0;
		//��ǰpass
		int32_t pass_idx_ = -1;
		//Pass���飬��¼Pass����
		vector<Pass*> pass_vec_;
		//��Դ����
		vector<LightBase*> light_vec_;
		//bool����
		vector<bool> light_active_vec_;
		//�ӿڿ��
		uint32_t view_width_ = 0;
		//�ӿڸ߶�
		uint32_t view_height_ = 0;
		//shadow map buffer�߶�
		uint32_t shadow_map_width_ = 0;
		//shadow map buffer���
		uint32_t shadow_map_height_ = 0;
		
		//��Ȼ���
		float* z_buffer_ = nullptr;
		//�����ͼ����
		float* depth_map_buffer_ = nullptr;
		//ʧ�ܵĶ��̹߳�դ��
		//mutex ����
		//mutex* mutex_buffer_;
		
		//֡����
		unsigned char* frame_buffer_ = nullptr;
		//��Ⱦ���ھ��
		HWND hwnd_;
		//ƽ��fps
		float fps_ = 0.0f;
		//��¼��fps����
		float fps_count_ = 0.0f;
		//fps��¼ʱ��
		DWORD record_time_ = 0;
		//��������
		vector<KZEngine::KZMaterial> mat_vec_;
		//����id
		uint32_t obj_id_ = 1;
		//��Ⱦ���߼�����Ӱ�ķ�ʽ 0��ʾ�����㣬1��ʾƽ��������Ӱӳ�䣬2��ʾshadow mapping
		CalCulateShadow calculate_shadow_ = CalCulateShadow::NONE;
		//AABB min
		KZMath::KZVector4D<float> aabb_min_ = KZMath::KZVector4D<float>(0, 0, 0, 1);
		//AABB max
		KZMath::KZVector4D<float> aabb_max_ = KZMath::KZVector4D<float>(0, 0, 0, 1);
		//BHV�����ڵ�
		BHVNodePtr bhv_root_ = nullptr;
		//��������ʽ
		SceneManage scene_manage_ = SceneManage::BHV;
		//��Ⱦģʽ������or�߿�
		RenderMode render_mode_ = RenderMode::WIREFRAME;
		//���߳�
		future<void>* future_pass_arrs_;
		bool mutiple_thread_ = false;
	private:
		//��ʵ��
		static KZPipeLine* p_instance_;
		//���캯��
		KZPipeLine(uint32_t renderlist_num = 2);
		//��������
		~KZPipeLine();
		//���Ӳ���
		uint32_t AddMaterial(const KZEngine::KZMaterial &);
	};
}
#endif // !KZ_PIPELINE
