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

		VertexType(float x = 0.0f, float y =0.0f, float z = 0.0f, unsigned char r = 255, unsigned char g = 255, unsigned char b = 255)
		{
			pos = KZMath::KZVector4D<float>(x, y, z);
			normal = KZMath::KZVector4D<float>();
			uv = KZMath::KZPoint2D();
			color = Color(r, g, b);
		}
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
		//��ȡ�����Χ��
		void GetObjectAABB(KZMath::KZVector4D<float>&vec_min, KZMath::KZVector4D<float>& vec_max) const;
		//����ת��
		void Transform(const KZMath::KZMatrix44& matrix);
	public:
		//����id
		int32_t id_ = -1;
		//��������
		string name_ = "";
		//���뾶�������Χ��
		float max_radius_ = 0.0f;
		//������������λ��
		KZMath::KZVector4D<float> world_pos_;
		//���峯��
		//KZMath::KZVector4D dir_;
		//����ֲ�������
		KZMath::KZVector4D<float> ux_ = KZMath::KZVector4D<float>(1, 0, 0, 0), uy_ = KZMath::KZVector4D<float>(0, 1, 0, 0), uz_ = KZMath::KZVector4D<float>(0, 0, 1, 0);
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
		//�Ƿ���ռ���
		bool is_light_ = false;
		//����alpha����
		float alpha_ = 1.0f;
		//�Ƿ������Ӱ
		bool has_shadow = true;
		//����id
		vector<int32_t> mat_id_;
		//�Ƿ��Ծ
		bool active_ = true;
		//Pass ID
		int32_t pass_id_ = -1;
	private:
		//
	};
}

#endif // !OBJECT_H
