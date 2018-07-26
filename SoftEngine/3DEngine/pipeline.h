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
		//��ȡ��ɾ����ʵ��
		static KZPipeLine* GetInstance() {
			if (p_instance_ == nullptr) {
				p_instance_ = new KZPipeLine();
			}
			return p_instance_;
		}

		//ɾ����ʵ��
		void DeleteInstance() {
			if (p_instance_) {
				delete p_instance_;
			}
		}
		//���ļ��м���
		void LoadFromFile();
		//����cube
		void CreateCube();
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
		//ÿ֡����
		void FrameUpdate();
		//��ʼ��
		void Initial();
		//��դ������Ȳ���
		void RasterizationDepthTest();
	public:
		//�ӿڿ��
		uint32_t view_width;
		//�ӿڸ߶�
		uint32_t view_height;
		//������Ŀ
		uint32_t object_num_;
		//����������
		uint32_t tri_num_;
		//�洢��������
		vector<KZObject> object_vec_;
		//��Ⱦ����
		KZRenderList* render_list_;
		KZCamera main_camera_;
		vector<bool> object_active_;
		float** z_buffer_;
	private:
		//��դ��ƽ��������
		void DrawBottomTri(const KZMath::KZVector4D& v0, const KZMath::KZVector4D& v1, const KZMath::KZVector4D& v2);
		//��դ��ƽ��������
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
