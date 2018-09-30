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
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(0, 0, -5), 
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO, 
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1), 
			bool is_light = false);
		//����Բ��
		void CreateCylinder(float top_radius, float bottom_radius, float height, uint32_t stack, uint32_t slice,
			const KZEngine::Color& ini_color,
			bool is_light = false,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(0,0,0),
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO, 
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1));
		//��������
		void CreateSphere(float radius, uint32_t stack, uint32_t slice,
			const KZEngine::Color& ini_color,
			bool is_light = false,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(0, 0, 0),
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO,
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1));
		//��������׶
		void CreatePyramid(const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(2, 0, -7), 
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO, 
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>(1, 1, 1), 
			bool is_light = true);
		//��������
		void Create_Terrain(float width, float height, float vscale, 
			const char* height_map_file_name, const char* texture_map_file_name, 
			const KZEngine::Color& ini_color, 
			bool is_light = false,
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(), 
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO
			);
		//�ı��״̬
		void ChangeLight(uint32_t light_index);
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
	public:
		//�������
		KZCamera main_camera_;
		//�����һλ�õ�x
		int last_pos_x_;
		//�����һλ�õ�y
		int last_pos_y_;
		//�Ƿ��״ΰ���������
		bool first_mouse_;
	protected:
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
		void DrawBottomTri(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id = -1);
		//�����汾��դ��ƽ��������
		void DrawBottomTriFast(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id = -1);
		//�������汾��դ��ƽ��������
		void DrawTopTri(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id = -1);
		//�����汾��դ��ƽ��������
		void DrawTopTriFast(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id = -1);
		//˫���彻��
		void SwapBuffer();
	private:
		//��Դ����
		vector<LightBase*> light_vec_;
		//bool����
		vector<bool> light_active_vec_;
		//�ӿڿ��
		uint32_t view_width_;
		//�ӿڸ߶�
		uint32_t view_height_;
		//������Ŀ
		uint32_t object_num_;
		//����������
		uint32_t tri_num_;
		//�洢��������
		vector<KZObject> object_vec_;
		//��Ⱦ����
		KZRenderList* render_list_;
		//�������
		vector<bool> object_active_;
		//��Ȼ���
		float* z_buffer_;
		//ʧ�ܵĶ��̹߳�դ��
		//mutex ����
		//mutex* mutex_buffer_;
		//֡����
		unsigned char* frame_buffer_;
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
