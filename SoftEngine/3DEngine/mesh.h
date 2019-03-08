#ifndef KZMESH_H
#define KZMESH_H

#include"../KZMath/util.h"
#include"../KZMath/vector.h"
#include"../KZMath/matrix.h"
#include"../KZMath/quat.h"
#include<vector>
#include"light.h"

namespace KZEngine {
	enum class BSPVertexState
	{
		NONE,
		FRONT,
		BEHIND,
		INSIDE,
	};
	//����ṹ��
	typedef struct VertexType {
		KZMath::KZVector4D<float> pos;
		KZMath::KZVector4D<float> normal;
		KZMath::KZPoint2D uv;
		Color color;

		VertexType(float x = 0.0f, float y = 0.0f, float z = 0.0f, unsigned char r = 255, unsigned char g = 255, unsigned char b = 255)
		{
			pos = KZMath::KZVector4D<float>(x, y, z);
			normal = KZMath::KZVector4D<float>();
			uv = KZMath::KZPoint2D();
			color = Color(r, g, b);
		}
	}Vertex, *VertexPtr;

	class KZMesh 
	{
	public:
		//������
		uint32_t num_vertices_ = 0;
		//ԭʼ�����б�
		vector<Vertex> vlist_local_;
		//�任�����б�
		vector<Vertex> vlist_tran_;
		//�淨��
		vector<KZMath::KZVector4D<float>> face_normal_;
		//������
		uint32_t num_face_ = 0;
		//��������
		uint32_t num_index_ = 0;
		//��������
		vector<uint32_t> index_;
		//��̬or��̬
		bool is_static_ = false;
	};
}

#endif