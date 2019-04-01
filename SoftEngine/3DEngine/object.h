#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "mesh.h"

namespace KZEngine {

	//物体类
	class KZObject
	{
	public:
		KZObject() = default;
		//从obj文件中读取数据，暂时不读取材质文件
		KZObject(const char* file_name, 
			const KZMath::KZVector4D<float>& world_pos = KZMath::KZVector4D<float>(), 
			const KZMath::KZQuat& quat = KZMath::KZQuat::ZERO, 
			const KZMath::KZVector4D<float>& scale = KZMath::KZVector4D<float>());
		//物体通过矩阵从模型坐标系转世界坐标系
		void TransformModelToWorldMatrix();
		//物体通过数学公式从模型坐标系转世界坐标系
		void TransformModelToWorldMath();
		//通过矩阵缩放
		void ScaleMatrix(const KZMath::KZVector4D<float>& vector);
		//通过数学公式缩放
		void ScaleMath(const KZMath::KZVector4D<float>& vector);
		//通过矩阵旋转
		void RotationMatrix(const KZMath::KZMatrix44& matrix);
		//通过四元数旋转
		void RotationQuat(const KZMath::KZQuat& quat);
		//计算包围球半径
		void CalculateRadian();
		//预先计算面法线和顶点法线
		void CalculateNormal(bool need_vertex = true);
		//获取物体包围盒
		void GetObjectAABB(KZMath::KZVector4D<float>&vec_min, KZMath::KZVector4D<float>& vec_max) const;
		//矩阵转换
		void Transform(const KZMath::KZMatrix44& matrix);
		//更新mesh
		virtual void UpdateMesh() {};
		//从obj文件读取模型数据

	public:
		//物体id
		int32_t id_ = -1;
		//物体名字
		string name_ = "";
		//最大半径，计算包围球
		float max_radius_ = 0.0f;
		//物体世界坐标位置
		KZMath::KZVector4D<float> world_pos_;
		//物体朝向
		//KZMath::KZVector4D dir_;
		//物体局部坐标轴
		KZMath::KZVector4D<float> ux_ = KZMath::KZVector4D<float>(1, 0, 0, 0), uy_ = KZMath::KZVector4D<float>(0, 1, 0, 0), uz_ = KZMath::KZVector4D<float>(0, 0, 1, 0);
		//物体mesh
		KZMesh mesh;
		//是否光照计算
		bool is_light_ = false;
		//物体alpha属性
		float alpha_ = 1.0f;
		//是否具有阴影
		bool has_shadow = true;
		//材质id
		vector<int32_t> mat_id_;
		//是否活跃
		bool active_ = true;
		//Pass ID
		int32_t pass_id_ = -1;
		//静态mesh or 动态mesh
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
