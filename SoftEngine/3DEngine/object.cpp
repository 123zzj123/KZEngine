#include"object.h"
using namespace KZEngine;

//物体通过矩阵从模型坐标系转世界坐标系
void KZObject::TransformModelToWorldMatrix() {
	KZMath::KZMatrix44 transform;
	transform.Translation(world_pos_);
	Vertex temp_vertex;
	vlist_tran_.reserve(num_vertices_);
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		temp_vertex.pos = transform * vlist_local_[i].pos;
		vlist_tran_.push_back(temp_vertex);
	}
	return;
}

//物体通过数学公式从模型坐标系转世界坐标系
void KZObject::TransformModelToWorldMath() {
	Vertex temp_vertex;
	vector<KZEngine::Vertex>().swap(vlist_tran_);
	vlist_tran_.reserve(num_vertices_);
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		temp_vertex = vlist_local_[i];
		temp_vertex.pos += world_pos_;
		vlist_tran_.push_back(temp_vertex);
	}
	return;
}

//通过矩阵缩放
void KZObject::ScaleMatrix(const KZMath::KZVector4D<float>& vector) {
	KZMath::KZMatrix44 transform;
	transform.Scaling(vector);
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		vlist_local_[i].pos = transform * vlist_local_[i].pos;
	}
	return;
}

//通过数学公式缩放
void KZObject::ScaleMath(const KZMath::KZVector4D<float>& vector) {
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		vlist_local_[i].pos *= vector;
	}
	return;
}

//通过矩阵旋转
void KZObject::RotationMatrix(const KZMath::KZMatrix44& matrix) {
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		vlist_local_[i].pos = matrix * vlist_local_[i].pos;
		vlist_local_[i].normal = matrix * vlist_local_[i].normal;
	}
	for (uint32_t i = 0; i < num_face_; ++i) {
		face_normal_[i] = matrix * face_normal_[i];
	}
	ux_ = matrix * ux_;
	uy_ = matrix * uy_;
	uz_ = matrix * uz_;
	return;
}

//通过四元数旋转
void KZObject::RotationQuat(const KZMath::KZQuat& quat) {
	KZMath::KZQuat temp_quat;
	KZMath::KZQuat quat_conj;
	KZMath::KZQuat ans_quat;
	Vertex ans_vertex;
	quat.Conjugate(quat_conj);
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		temp_quat.Set(0, vlist_local_[i].pos.x_, vlist_local_[i].pos.y_, vlist_local_[i].pos.z_);
		quat.TripleProduct(temp_quat, quat_conj, ans_quat);
		vlist_local_[i].pos.x_ = ans_quat.x_;
		vlist_local_[i].pos.y_ = ans_quat.y_;
		vlist_local_[i].pos.z_ = ans_quat.z_;
	}
	for (uint32_t i = 0; i < num_face_; ++i) {
		temp_quat.Set(0, face_normal_[i].x_, face_normal_[i].y_, face_normal_[i].z_);
		quat.TripleProduct(temp_quat, quat_conj, ans_quat);
		face_normal_[i].x_ = ans_quat.x_;
		face_normal_[i].y_ = ans_quat.y_;
		face_normal_[i].z_ = ans_quat.z_;
	}
	temp_quat.x_ = ux_.x_;
	temp_quat.y_ = ux_.y_;
	temp_quat.z_ = ux_.z_;
	return;
}

//计算包围球半径
void KZObject::CalculateRadian() {
	float max_r = 0.0f;
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		if (vlist_local_[i].pos.Vector3Length() > max_r) {
			max_r = vlist_local_[i].pos.Vector3Length();
		}
	}
	max_radius_ = sqrt(max_r);
}

//预先计算面法线和顶点法线
void KZObject::CalculateNormal(bool need_vertex) {
	face_normal_.reserve(num_face_);
	uint32_t* poly_vertex = new uint32_t[num_vertices_]();
	for (uint32_t i = 0; i < num_index_; i += 3) {
		KZMath::KZVector4D<float> vec1 = vlist_local_[index_[i + 1]].pos - vlist_local_[index_[i]].pos;
		vec1.w_ = 0;
		KZMath::KZVector4D<float> vec2 = vlist_local_[index_[i + 2]].pos - vlist_local_[index_[i + 1]].pos;
		vec2.w_ = 0;
		KZMath::KZVector4D<float> face_normal;
		vec1.Vector3Cross(face_normal, vec2);
		face_normal_.push_back(face_normal);
		if (need_vertex) {
			vlist_local_[index_[i]].normal += face_normal;
			vlist_local_[index_[i + 1]].normal += face_normal;
			vlist_local_[index_[i + 2]].normal += face_normal;
			++poly_vertex[index_[i]];
			++poly_vertex[index_[i + 1]];
			++poly_vertex[index_[i + 2]];
		}
	}
	if (need_vertex) {
		for (uint32_t i = 0; i < num_vertices_; ++i) {
			vlist_local_[i].normal /= (float)poly_vertex[i];
			vlist_local_[i].normal.Vector3Normalize();
		}
	}
	delete[]poly_vertex;
	return;
}