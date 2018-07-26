#pragma once
#ifndef KZ_RENDERLIST
#define KZ_RENDERLIST
#include"object.h"

namespace KZEngine {
	//�����νṹ��
	typedef struct TriangleType{
		Vertex vertex_list[3];
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
		//�������б�
		vector<Triangle> tri_list_;
	private:
		static KZRenderList* r_instance_;
		KZRenderList() {};
	};
}
#endif // !KZ_RENDERLIST
