#include"object.h"
using namespace KZEngine;

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

void KZObject::TransformModelToWorldMath() {
	Vertex temp_vertex;
	vlist_tran_.reserve(num_vertices_);
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		temp_vertex.pos = vlist_local_[i].pos;
		temp_vertex.pos += world_pos_;
		vlist_tran_.push_back(temp_vertex);
	}
	return;
}

void KZObject::ScaleMatrix(const KZMath::KZVector4D& vector) {
	KZMath::KZMatrix44 transform;
	transform.Scaling(vector);
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		vlist_local_[i].pos = transform * vlist_local_[i].pos;
	}
	return;
}

void KZObject::ScaleMath(const KZMath::KZVector4D& vector) {
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		vlist_local_[i].pos *= vector;
	}
	return;
}

void KZObject::RotationMatrix(const KZMath::KZMatrix44& matrix) {
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		vlist_local_[i].pos = matrix * vlist_local_[i].pos;
	}
	ux_ = matrix * ux_;
	uy_ = matrix * uy_;
	uz_ = matrix * uz_;
	return;
}

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
	temp_quat.x_ = ux_.x_;
	temp_quat.y_ = ux_.y_;
	temp_quat.z_ = ux_.z_;
	return;
}

void KZObject::CalculateRadian() {
	float max_r = 0.0f;
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		if (vlist_local_[i].pos.Vector3Length() > max_r) {
			max_r = vlist_local_[i].pos.Vector3Length();
		}
	}
	max_radius_ = sqrt(max_r);
}