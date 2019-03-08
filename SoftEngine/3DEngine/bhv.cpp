#include "pipeline.h"
#include "bhv.h"
using namespace KZEngine;

//建造场景BHV树
void KZBHV::BuildSceneBHVTree(BHVNodePtr& bhv_root, uint32_t level, uint32_t num_divisions) {
	if (level == 0) {
		KZPipeLine* instance = KZPipeLine::GetInstance();
		ClearSceneBHVTree(bhv_root);
		instance->getSceneAABB(true);
		bhv_root = new BHVNode();
		bhv_root->origin_pos_ = instance->aabb_min_;
		bhv_root->radius_.x_ = instance->aabb_max_.x_ - instance->aabb_min_.x_;
		bhv_root->radius_.y_ = instance->aabb_max_.y_ - instance->aabb_min_.y_;
		bhv_root->radius_.z_ = instance->aabb_max_.z_ - instance->aabb_min_.z_;
		bhv_root->world_pos_ = bhv_root->origin_pos_ + bhv_root->radius_ * 0.5f;
		for (int32_t i = 0; i < instance->pass_num_; i++)
		{
			for (uint32_t j = 0; j < instance->pass_vec_[i]->object_num_; ++j) {
				bhv_root->obj_list_.push_back(instance->pass_vec_[i]->object_vec_[j]);
			}
			bhv_root->num_object_ += instance->pass_vec_[i]->object_num_;
		}
		uint32_t num_child = num_divisions * num_divisions * num_divisions;
		bhv_root->bhv_child = new BHVNodePtr[num_child]();
		for (uint32_t i = 0; i < num_child; ++i) {
			bhv_root->bhv_child[i] = nullptr;
		}
		BuildSceneBHVTree(bhv_root, level + 1, num_divisions);
	}
	else
	{
		//BHV树层数大于等于最大层数限制
		if (level >= kMaxBhvLevel) {
			return;
		}

		//当前的子节点物体数小于等于BHV树分裂子节点
		if (bhv_root->num_object_ <= kMinBhvObject) {
			return;
		}

		float cell_size_x = bhv_root->radius_.x_ / num_divisions;
		float cell_size_y = bhv_root->radius_.y_ / num_divisions;
		float cell_size_z = bhv_root->radius_.z_ / num_divisions;

		uint32_t num_divisions_power = num_divisions * num_divisions;
		uint32_t cell_x = 0, cell_y = 0, cell_z = 0, cell_idx = 0;
		for (int32_t i = 0; i < bhv_root->num_object_; ++i) {
			cell_x = (uint32_t)((bhv_root->obj_list_[i]->world_pos_.x_ - bhv_root->origin_pos_.x_) / cell_size_x);
			cell_y = (uint32_t)((bhv_root->obj_list_[i]->world_pos_.y_ - bhv_root->origin_pos_.y_) / cell_size_y);
			cell_z = (uint32_t)((bhv_root->obj_list_[i]->world_pos_.z_ - bhv_root->origin_pos_.z_) / cell_size_z);

			cell_idx = cell_x * num_divisions_power + cell_y * num_divisions + cell_z;
			if (bhv_root->bhv_child[cell_idx] == nullptr)
			{
				bhv_root->bhv_child[cell_idx] = new BHVNode();
			}
			bhv_root->bhv_child[cell_idx]->obj_list_.push_back(bhv_root->obj_list_[i]);
			++bhv_root->bhv_child[cell_idx]->num_object_;
		}

		cell_idx = 0;
		for (uint32_t i = 0; i < num_divisions; ++i) {
			for (uint32_t j = 0; j < num_divisions; ++j) {
				for (uint32_t k = 0; k < num_divisions; ++k) {
					if (bhv_root->bhv_child[cell_idx] != nullptr) {
						bhv_root->bhv_child[cell_idx]->origin_pos_.x_ = bhv_root->origin_pos_.x_ + i * cell_size_x;
						bhv_root->bhv_child[cell_idx]->origin_pos_.y_ = bhv_root->origin_pos_.y_ + j * cell_size_y;
						bhv_root->bhv_child[cell_idx]->origin_pos_.z_ = bhv_root->origin_pos_.z_ + k * cell_size_z;
						bhv_root->bhv_child[cell_idx]->radius_.x_ = cell_size_x;
						bhv_root->bhv_child[cell_idx]->radius_.y_ = cell_size_y;
						bhv_root->bhv_child[cell_idx]->radius_.z_ = cell_size_z;
						bhv_root->bhv_child[cell_idx]->world_pos_ = bhv_root->bhv_child[cell_idx]->origin_pos_ + bhv_root->bhv_child[cell_idx]->radius_ * 0.5f;
						
						BuildSceneBHVTree(bhv_root->bhv_child[cell_idx], level + 1, num_divisions);
					}
					++cell_idx;
				}
			}
		}
	}
}

//BHV树剔除
void KZBHV::BHVTreeCulling(BHVNodePtr bhv_root, int32_t pass_id) {
	if (bhv_root == nullptr)
	{
		return;
	}
	KZPipeLine* instance = KZPipeLine::GetInstance();
	float camera_right_over_near = instance->main_camera_.GetViewRight() / instance->main_camera_.GetCameraNearClip();
	float camera_top_over_near = instance->main_camera_.GetViewTop() / instance->main_camera_.GetCameraNearClip();
	KZMath::KZVector4D<float> temp_camera_pos;
	KZMath::KZMatrix44 view;
	instance->main_camera_.GetViewMatrix(view);
	temp_camera_pos = view * bhv_root->world_pos_;

	//根据左右平面裁剪
	float w_test = camera_right_over_near * temp_camera_pos.z_;
	//根据上下平面裁剪
	float h_test = camera_top_over_near * temp_camera_pos.z_;

	if (temp_camera_pos.z_ + bhv_root->radius_.z_ < instance->main_camera_.GetCameraFarClip() || temp_camera_pos.z_ - bhv_root->radius_.z_ > instance->main_camera_.GetCameraNearClip())
	{
		return;
	}

	else if (temp_camera_pos.x_ - bhv_root->radius_.x_ > w_test || temp_camera_pos.x_ + bhv_root->radius_.x_ < -w_test) {
		return;
	}

	else if (temp_camera_pos.y_ - bhv_root->radius_.y_ > h_test || temp_camera_pos.y_ + bhv_root->radius_.y_ < -h_test)
	{
		return;
	}

	//到达子节点未被剔除，进行背面消除后加入渲染列表
	if (bhv_root->num_children_ == 0) {
		for (int i = 0; i < bhv_root->num_object_; ++i) {
			uint32_t face_index = 0;
			for (uint32_t j = 0; j < bhv_root->obj_list_[i]->mesh.num_index_; face_index++, j += 3) {
				//只处理当前pass
				if (bhv_root->obj_list_[i]->pass_id_ == pass_id) {
					KZMath::KZVector4D<float> observe_vec = instance->main_camera_.GetCameraPos() - bhv_root->obj_list_[i]->mesh.vlist_tran_[bhv_root->obj_list_[i]->mesh.index_[j]].pos;
					if (bhv_root->obj_list_[i]->mesh.face_normal_[face_index].Vector3Dot(observe_vec) < 0) {
						continue;
					}
					else
					{
						++instance->pass_vec_[pass_id]->tri_num_;
						if (instance->pass_vec_[pass_id]->tri_num_ > instance->pass_vec_[pass_id]->render_list_->tri_list_.size()) {
							uint32_t old_len = (uint32_t)instance->pass_vec_[pass_id]->render_list_->tri_list_.size();
							instance->pass_vec_[pass_id]->render_list_->tri_list_.resize(instance->pass_vec_[pass_id]->tri_num_ * 2, nullptr);
							uint32_t new_len = (uint32_t)instance->pass_vec_[pass_id]->render_list_->tri_list_.size();
							for (uint32_t i = old_len; i < new_len; ++i)
							{
								instance->pass_vec_[pass_id]->render_list_->tri_list_[i] = new KZEngine::Triangle();
							}
						}
						KZEngine::TrianglePtr tri = instance->pass_vec_[pass_id]->render_list_->tri_list_[instance->pass_vec_[pass_id]->tri_num_ - 1];
						uint32_t idx0 = j, idx1 = j + 1, idx2 = j + 2;
						if (bhv_root->obj_list_[i]->is_light_) {
							uint32_t light_num = static_cast<uint32_t>(instance->light_vec_.size());
							for (uint32_t k = 0; k < light_num; ++k) {
								if (instance->light_active_vec_[k]) {
									bhv_root->obj_list_[i]->mesh.vlist_tran_[bhv_root->obj_list_[i]->mesh.index_[idx0]].color += instance->mat_vec_[bhv_root->obj_list_[i]->mat_id_[face_index]].CalculateFinalColor(instance->light_vec_[k],
										bhv_root->obj_list_[i]->mesh.vlist_tran_[bhv_root->obj_list_[i]->mesh.index_[idx0]].pos,
										bhv_root->obj_list_[i]->mesh.vlist_tran_[bhv_root->obj_list_[i]->mesh.index_[idx0]].normal);
									bhv_root->obj_list_[i]->mesh.vlist_tran_[bhv_root->obj_list_[i]->mesh.index_[idx1]].color += instance->mat_vec_[bhv_root->obj_list_[i]->mat_id_[face_index]].CalculateFinalColor(instance->light_vec_[k],
										bhv_root->obj_list_[i]->mesh.vlist_tran_[bhv_root->obj_list_[i]->mesh.index_[idx1]].pos,
										bhv_root->obj_list_[i]->mesh.vlist_tran_[bhv_root->obj_list_[i]->mesh.index_[idx1]].normal);
									bhv_root->obj_list_[i]->mesh.vlist_tran_[bhv_root->obj_list_[i]->mesh.index_[idx2]].color += instance->mat_vec_[bhv_root->obj_list_[i]->mat_id_[face_index]].CalculateFinalColor(instance->light_vec_[k],
										bhv_root->obj_list_[i]->mesh.vlist_tran_[bhv_root->obj_list_[i]->mesh.index_[idx2]].pos,
										bhv_root->obj_list_[i]->mesh.vlist_tran_[bhv_root->obj_list_[i]->mesh.index_[idx2]].normal);
								}
							}
						}
						tri->vertex_list[0] = bhv_root->obj_list_[i]->mesh.vlist_tran_[bhv_root->obj_list_[i]->mesh.index_[idx0]];
						tri->vertex_list[1] = bhv_root->obj_list_[i]->mesh.vlist_tran_[bhv_root->obj_list_[i]->mesh.index_[idx1]];
						tri->vertex_list[2] = bhv_root->obj_list_[i]->mesh.vlist_tran_[bhv_root->obj_list_[i]->mesh.index_[idx2]];
						tri->material = bhv_root->obj_list_[i]->mat_id_[face_index];
						tri->alpha = bhv_root->obj_list_[i]->alpha_;
						tri->active = true;
					}
				}
			}
		}
	}

	//该节点未被剔除继续往下剔除
	for (int i = 0; i < bhv_root->num_children_; ++i) {
		BHVTreeCulling(bhv_root->bhv_child[i], pass_id);
	}
}

//清除BHV场景树
void KZBHV::ClearSceneBHVTree(BHVNodePtr& bhv_root_) {
	if (bhv_root_ == nullptr)
	{
		return;
	}

	//到达叶子节点
	if (bhv_root_->num_children_ == 0)
	{
		delete[] bhv_root_->bhv_child;
		delete bhv_root_;
		return;
	}

	for (int i = 0; i < bhv_root_->num_children_; ++i)
	{
		ClearSceneBHVTree(bhv_root_->bhv_child[i]);
	}

	delete[] bhv_root_->bhv_child;
	delete bhv_root_;
	return;
}