#pragma once
#ifndef KZ_PIPELINE
#define KZ_PIPELINE

#include<vector>;
#include<windows.h>
#include"object.h"
#include"renderlist.h"
#include"camera.h"
#include"material.h"

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
				for (uint32_t i = 0; i < p_instance_->view_width_; ++i) {
					delete[] p_instance_->z_buffer_[i];
				}
				delete[] p_instance_->z_buffer_;
				delete[] p_instance_->frame_buffer_;
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
		void CreateCube();
		//��������׶
		void CreatePyramid();
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
	protected:
		//������������Χ�����
		void CullObject();
		//��������
		void RemoveBackface();
		//ת������������
		void TransformModelToWorld();
		//ת�����������
		void TransformWorldToPer(Projection projection = Projection::PERSPECTIVE);
		//ת�����ӿ�����
		void TransformPerToViewPort();
		//��ʼ��
		void Initial();
		//��դ������Ȳ���
		void RasterizationDepthTest();
		//��դ��ƽ��������
		void DrawBottomTri(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id = -1);
		//��դ��ƽ��������
		void DrawTopTri(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id = -1);
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
		float** z_buffer_;
		//֡����
		unsigned char* frame_buffer_;
		//��Ⱦ���ھ��
		HWND hwnd_;
		//ƽ��fps
		float fps_;
		//fps��¼ʱ��
		DWORD record_time_;
		//��������
		vector<KZEngine::KZMaterial> mat_vec_;
	private:
		//��ʵ��
		static KZPipeLine* p_instance_;
		//���캯��
		KZPipeLine();
		//���Ӳ���
		void AddMaterial(const KZEngine::KZMaterial &);
	};
}
#endif // !KZ_PIPELINE
