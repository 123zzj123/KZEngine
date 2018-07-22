#pragma once
#ifndef KZ_OBJECT
#define KZ_OBJECT

#include"../KZMath/util.h"
#include"../KZMath/vector.h"
#include"../KZMath/matrix.h"
#include"../KZMath/quat.h"
#include<vector>

namespace KZEngine {
	//顶点结构体
	typedef struct VertexType {
		KZMath::KZVector4D pos;
	}Vertex, *VertexPtr;

	//物体类
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
		//物体id
		int id_;
		//物体名字
		string name_;
		//最大半径，计算包围球
		float max_radius_;
		//物体世界坐标位置
		KZMath::KZVector4D world_pos_;
		//物体朝向
		KZMath::KZVector4D dir_;
		//物体局部坐标轴
		KZMath::KZVector4D ux_, uy_, uz_;
		//顶点数
		int num_vertices_;
		//原始顶点列表
		vector<Vertex> vlist_local_;
		//变换顶点列表
		vector<Vertex> vlist_tran_;

		//索引数量
		int num_index_;
		//索引数组
		vector<uint32_t> index_;
	private:
		//
	};
}

#endif // !KZ_OBJECT
