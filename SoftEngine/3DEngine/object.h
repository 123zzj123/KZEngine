#pragma once
#ifndef KZ_OBJECT
#define KZ_OBJECT

#include"../KZMath/util.h"
#include"../KZMath/vector.h"
#include"../KZMath/matrix.h"
#include"../KZMath/quat.h"
#include<vector>

namespace KZEngine {
	//����ṹ��
	typedef struct VertexType {
		KZMath::KZVector4D pos;
	}Vertex, *VertexPtr;

	//������
	class KZObject
	{
	public:
		void TransformModelToWorldMatrix();
		void TransformModelToWorldMath();
		void ScaleMatrix(const KZMath::KZVector4D& vector);
		void ScaleMath(const KZMath::KZVector4D& vector);
		void RotationMatrix(const KZMath::KZMatrix44& matrix);
		void RotationQuat(const KZMath::KZQuat& quat);
		void CalculateRadian();
	public:
		//����id
		int id_;
		//��������
		string name_;
		//���뾶�������Χ��
		float max_radius_;
		//������������λ��
		KZMath::KZVector4D world_pos_;
		//���峯��
		KZMath::KZVector4D dir_;
		//����ֲ�������
		KZMath::KZVector4D ux_, uy_, uz_;
		//������
		int num_vertices_;
		//ԭʼ�����б�
		vector<Vertex> vlist_local_;
		//�任�����б�
		vector<Vertex> vlist_tran_;

		//��������
		int num_index_;
		//��������
		vector<uint32_t> index_;
	private:
		//
	};
}

#endif // !KZ_OBJECT
