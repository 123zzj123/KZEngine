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
		KZRenderList* GetInstance() {
			if (instance_ == nullptr) {
				instance_ = new KZRenderList();
			}
			return instance_;
		}

		//ɾ����ʵ��
		void DeleteInstance() {
			if (instance_) {
				delete instance_;
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
		KZRenderList* instance_ = nullptr;
		KZRenderList();
	};
}
#endif // !KZRENDERLIST
