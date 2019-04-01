#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "mesh.h"

namespace KZEngine {

	//������
	class KZObject
	{
	public:
		KZObject() = default;
		//��obj�ļ��ж�ȡ���ݣ���ʱ����ȡ�����ļ�
		KZObject(const char* file_name, 
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(), 
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO, 
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>());
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
		//����mesh
		virtual void UpdateMesh() {};
		//��obj�ļ���ȡģ������

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
		//����mesh
		KZMesh mesh;
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
		//��̬mesh or ��̬mesh
		bool static_mesh_ = true;
	private:
		//
	};

	class KZCube final:public KZObject 
	{
	public:
		KZCube(float width, float length, float height, bool light_cube, 
			const KZMath::KZVector4D<float>& world_pos, 
			const KZMath::KZQuat& quat, 
			const KZMath::KZVector4D<float>& scale);
	};

	class KZCylinder final : public KZObject
	{
	public:
		KZCylinder(float top_radius, float bottom_radius, float height, uint32_t stack, uint32_t slice, const KZEngine::Color& ini_color, 
			const KZMath::KZVector4D<float>& world_pos,
			const KZMath::KZQuat& quat,
			const KZMath::KZVector4D<float>& scale);
	};

	class KZPyramid final : public KZObject
	{
	public:
		KZPyramid(const KZMath::KZVector4D<float>& world_pos, const KZMath::KZQuat& quat, const KZMath::KZVector4D<float>& scale);
	};

	class KZSphere final : public KZObject
	{
	public:
		KZSphere(float radius, uint32_t stack, uint32_t slice, const KZEngine::Color& ini_color,
			const KZMath::KZVector4D<float>& world_pos,
			const KZMath::KZQuat& quat,
			const KZMath::KZVector4D<float>& scale);
	};

	class KZSkyBox final : public KZObject
	{
	public:
		KZSkyBox();
	};
}

#endif // !OBJECT_H
