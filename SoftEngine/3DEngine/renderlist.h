#pragma once
#ifndef RENDERLIST_H
#define RENDERLIST_H
#include"object.h"

namespace KZEngine {
	//�����νṹ��
	typedef struct TriangleType{
		Vertex vertex_list[3];
		KZMath::KZVector4D<float> face_normal;
		uint32_t material = -1;
		float alpha = 1.0f;
	}Triangle, *TrianglePtr;

	//��Ⱦ�б�
	class KZRenderList
	{
	public:
		//�������ȡ��ʵ��
		static KZRenderList* GetInstance() {
			if (r_instance_ == nullptr) {
				r_instance_ = new KZRenderList();
			}
			return r_instance_;
		}

		//ɾ����ʵ��
		void DeleteInstance() {
			if (r_instance_) {
				delete r_instance_;
			}
		}
	public:
		//����ת��
		void Transform(const KZMath::KZMatrix44& matrix);
		//����������
		uint32_t num_poly_;
		//͸������������
		uint32_t transparent_num_poly_;
		//�������б�
		vector<Triangle> tri_list_;
		//͸���������б�
		vector<Triangle> transparent_tri_list_;
		//��Ծ�������б�,�ü���ʶ
		vector<bool> active_tri_;
		//��Ծ�������б�,�ü���ʶ
		vector<bool> transparent_active_tri_;
	private:
		static KZRenderList* r_instance_;
		KZRenderList() {};
	};
}
#endif // !RENDERLIST_H
