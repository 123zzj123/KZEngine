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
#include "BHV.h"
#include<list>
#include <limits.h>
//#include<thread>
//#include<mutex>

namespace KZEngine {
	enum class Projection
	{
		PERSPECTIVE,
		ORTHOGONAL,
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
		void LoadFromFile();
		//����cube
		void CreateCube(float width = 1.0f, float length = 1.0f, float height = 1.0f,
			bool light_cube = false,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(0, 0, -5), 
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO, 
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1), 
			bool is_light = false,
			float alpha = 1.0f,
			string shadow_map = "");
		//����Բ��
		void CreateCylinder(float top_radius, float bottom_radius, float height, uint32_t stack, uint32_t slice,
			const KZEngine::Color& ini_color,
			bool is_light = false,
			float alpha = 1.0f,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(0,0,0),
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO, 
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1),
			string shadow_map = "");
		//��������
		void CreateSphere(float radius, uint32_t stack, uint32_t slice,
			const KZEngine::Color& ini_color,
			bool is_light = false,
			float alpha = 1.0f,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(0, 0, 0),
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO,
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1),
			string shadow_map = "");
		//��������׶
		void CreatePyramid(const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(2, 0, -7), 
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO, 
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1), 
			bool is_light = true,
			float alpha = 1.0f,
			string shadow_map = "");
		//��������
		void Create_Terrain(float width, float height, float vscale, 
			const char* height_map_file_name, const char* texture_map_file_name, 
			const KZEngine::Color& ini_color, 
			bool is_light = false,
			float alpha = 1.0f,
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
	public:
		//�������
		KZCamera main_camera_;
		//�����һλ�õ�x
		int last_pos_x_;
		//�����һλ�õ�y
		int last_pos_y_;
		//�Ƿ��״ΰ���������
		bool first_mouse_ = false;
	protected:
		//�����޳��������޳�����������
		void SceneManageCulling();
		//������������Χ�����
		void OcclusionCulling();
		//��������
		void BackfaceCulling();
		//�����βü�
		void PolyCulling();
		//ת������������
		void TransformModelToWorld();
		//ת�����������
		void TransformWorldToPer(Projection projection = Projection::PERSPECTIVE);
		//ת�����ӿ�����
		void TransformPerToViewPort();
		//��ʼ��
		void Initial();
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
	private:
		//��ռ�任����
		KZMath::KZMatrix44 light_space_matrix_;
		//��Դ����
		vector<LightBase*> light_vec_;
		//bool����
		vector<bool> light_active_vec_;
		//�ӿڿ��
		uint32_t view_width_ = -1;
		//�ӿڸ߶�
		uint32_t view_height_ = -1;
		//shadow map buffer�߶�
		uint32_t shadow_map_width_ = -1;
		//shadow map buffer���
		uint32_t shadow_map_height_ = -1;
		//������Ŀ
		uint32_t object_num_ = -1;
		//͸����������
		uint32_t transparent_object_num_ = -1;
		//����������
		uint32_t tri_num_ = -1;
		//͸������������
		uint32_t transparent_tri_num_ = -1;
		//�洢��ͨ��������
		vector<KZObject> object_vec_;
		//�洢͸����������
		vector<KZObject> transparent_object_vec_;
		//��Ⱦ����
		KZRenderList* render_list_ = nullptr;
		//�������
		vector<bool> object_active_;
		//͸���������
		vector<bool> transparent_object_active_;
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
		float fps_;
		//��¼��fps����
		float fps_count_;
		//fps��¼ʱ��
		DWORD record_time_;
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
		BHVNodePtr bhv_root_;
		//ͶӰ��ʽ
		Projection projection_ = Projection::PERSPECTIVE;
		//��������ʽ
		SceneManage scene_manage_ = SceneManage::BHV;
	private:
		//��ʵ��
		static KZPipeLine* p_instance_;
		//���캯��
		KZPipeLine();
		//��������
		~KZPipeLine();
		//���Ӳ���
		uint32_t AddMaterial(const KZEngine::KZMaterial &);
	};
}
#endif // !KZ_PIPELINE
