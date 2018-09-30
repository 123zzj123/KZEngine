#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include"../KZMath/util.h"
#include"../KZMath/vector.h"
#include"../KZMath/matrix.h"
#include"../KZMath/quat.h"
#include<vector>
#include"light.h"

namespace KZEngine {
	//����ṹ��
	typedef struct VertexType {
		KZMath::KZVector4D<float> pos;
		KZMath::KZVector4D<float> normal;
		KZMath::KZPoint2D uv;
		Color color;
	}Vertex, *VertexPtr;

	//������
	class KZObject
	{
	public:
		//����ͨ�������ģ������ϵת��������ϵ
		void TransformModelToWorldMatrix();
		//����ͨ����ѧ��ʽ��ģ������ϵת��������ϵ
		void TransformModelToWorldMath();
		//ͨ����������
		void ScaleMatrix(const KZMath::KZVector4D<float>& vector);
		//ͨ����ѧ��ʽ����
		void ScaleMath(const KZMath::KZVector4D<float>& vector);
		//ͨ��������ת
		void RotationMatrix(const KZMath::KZMatrix44& matrix);
		//ͨ����Ԫ����ת
		void RotationQuat(const KZMath::KZQuat& quat);
		//�����Χ��뾶
		void CalculateRadian();
		//Ԥ�ȼ����淨�ߺͶ��㷨��
		void CalculateNormal(bool need_vertex = true);
	public:
		//����id
		int id_;
		//��������
		string name_;
		//���뾶�������Χ��
		float max_radius_;
		//������������λ��
		KZMath::KZVector4D<float> world_pos_;
		//���峯��
		//KZMath::KZVector4D dir_;
		//����ֲ�������
		KZMath::KZVector4D<float> ux_, uy_, uz_;
		//������
		uint32_t num_vertices_;
		//ԭʼ�����б�
		vector<Vertex> vlist_local_;
		//�任�����б�
		vector<Vertex> vlist_tran_;
		//�淨��
		vector<KZMath::KZVector4D<float>> face_normal_;
		//������
		uint32_t num_face_;
		//��������
		uint32_t num_index_;
		//��������
		vector<uint32_t> index_;
		//�Ƿ���ռ���
		bool is_light_ = false;
		//����id
		vector<uint32_t> mat_id_;
	private:
		//
	};
}

#endif // !OBJECT_H
