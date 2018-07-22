#include"pipeline.h"
using namespace KZEngine;

void KZPipeLine::LoadFromFile() {
	//to do
}

//物体消除，包围球测试
void KZPipeLine::CullObject() {
	for (uint32_t i = 0; i < object_num_; ++i) {
		//根据远近平面裁剪
		if ((object_vec_[i].world_pos_.z_ + object_vec_[i].max_radius_ > main_camera_.far_clip_z_) || (object_vec_[i].world_pos_.z_ - object_vec_[i].max_radius_ < main_camera_.near_clip_z_)) {
			object_active_[i] = false;
			continue;
		}
		//根据左右平面裁剪
		float w_test = main_camera_.right_ * object_vec_[i].world_pos_.z_ / main_camera_.near_clip_z_;
		if ((object_vec_[i].world_pos_.x_ + object_vec_[i].max_radius_ > w_test) || (object_vec_[i].world_pos_.x_ - object_vec_[i].max_radius_ < -w_test)) {
			object_active_[i] = false;
			continue;
		}

		//根据上下平面裁剪
		float h_test = main_camera_.top_ * object_vec_[i].world_pos_.z_ / main_camera_.near_clip_z_;
		if ((object_vec_[i].world_pos_.y_ + object_vec_[i].max_radius_ > h_test) || (object_vec_[i].world_pos_.y_ - object_vec_[i].max_radius_ < -h_test)) {
			object_active_[i] = false;
			continue;
		}
	}
	return;
}

//背面消除
void KZPipeLine::RemoveBackface() {
	for (uint32_t i = 0; i < object_num_; ++i) {
		if (object_active_[i]) {
			for (uint32_t j = 0; j < object_vec_[i].num_index_; j += 3) {
				KZMath::KZVector4D vec1 = object_vec_[i].vlist_tran_[object_vec_[i].index_[j + 1]].pos - object_vec_[i].vlist_tran_[object_vec_[i].index_[j]].pos;
				vec1.w_ = 0;
				KZMath::KZVector4D vec2 = object_vec_[i].vlist_tran_[object_vec_[i].index_[j + 2]].pos - object_vec_[i].vlist_tran_[object_vec_[i].index_[j]].pos;
				vec2.w_ = 0;
				KZMath::KZVector4D face_normal;
				vec1.Vector3Cross(face_normal, vec2);

				if (face_normal.Vector3Dot(main_camera_.n_) < 0) {
					continue;
				}
				else
				{
					KZEngine::Triangle tri;
					tri.vertex_list[0].pos = object_vec_[i].vlist_tran_[object_vec_[i].index_[j]].pos;
					tri.vertex_list[1].pos = object_vec_[i].vlist_tran_[object_vec_[i].index_[j + 1]].pos;
					tri.vertex_list[2].pos = object_vec_[i].vlist_tran_[object_vec_[i].index_[j + 2]].pos;
					render_list_->tri_list_.push_back(tri);
				}
			}
		}
	}
	return;
}