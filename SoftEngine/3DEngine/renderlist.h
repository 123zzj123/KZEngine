#pragma once
#ifndef RENDERLIST_H
#define RENDERLIST_H
#include"object.h"

namespace KZEngine {

	//�����νṹ��
	typedef struct TriangleType{
		//��������
		Vertex vertex_list[3];
		//����
		KZMath::KZVector4D<float> face_normal;
		//����id
		uint32_t material = -1;
		//͸����
		float alpha = 1.0f;
		//�����βü���Ծ���
		bool active = false;
		//trueΪ�ṹ����,����bsp��falseΪϸ����
		bool bsp_split = false;
		//bsp�ָ���id
		uint32_t bsp_split_id = 0;
	}Triangle, *TrianglePtr;

	//��Ⱦ�б�
	class KZRenderList
	{
		friend class Pass;
	private:
		KZRenderList() {};
		~KZRenderList();
		//����ת��
		void Transform(const KZMath::KZMatrix44& matrix);
	public:
		//�������б�
		vector<TrianglePtr> tri_list_;
		//�Ƿ�͸��������Ⱦ����
		bool transparent = false;
	};
}
#endif // !RENDERLIST_H
