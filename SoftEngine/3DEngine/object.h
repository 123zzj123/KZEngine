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
	//顶点结构体
	typedef struct VertexType {
		KZMath::KZVector4D<float> pos;
		KZMath::KZVector4D<float> normal;
		KZMath::KZPoint2D uv;
		Color color;
	}Vertex, *VertexPtr;

	//物体类
	class KZObject
	{
	public:
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
	public:
		//物体id
		int id_;
		//物体名字
		string name_;
		//最大半径，计算包围球
		float max_radius_;
		//物体世界坐标位置
		KZMath::KZVector4D<float> world_pos_;
		//物体朝向
		//KZMath::KZVector4D dir_;
		//物体局部坐标轴
		KZMath::KZVector4D<float> ux_, uy_, uz_;
		//顶点数
		uint32_t num_vertices_;
		//原始顶点列表
		vector<Vertex> vlist_local_;
		//变换顶点列表
		vector<Vertex> vlist_tran_;
		//面法线
		vector<KZMath::KZVector4D<float>> face_normal_;
		//面数量
		uint32_t num_face_;
		//索引数量
		uint32_t num_index_;
		//索引数组
		vector<uint32_t> index_;
		//是否光照计算
		bool is_light_ = false;
		//材质id
		vector<uint32_t> mat_id_;
	private:
		//
	};
}

#endif // !OBJECT_H
