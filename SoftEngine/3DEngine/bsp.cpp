#include "bsp.h"
#include "pipeline.h"
using namespace KZEngine;

uint32_t KZBSP::bsp_plane_id = 0;
vector<BSPSplitPlane> KZBSP::split_plane_vec_;

//建造场景BSP树
void KZBSP::BuildSceneBSPTree(BSPNodePtr& bsp_root, uint32_t level) {
	if (level == 0) {
		KZPipeLine* instance = KZPipeLine::GetInstance();
		bsp_root = new BSPNode();
		for (uint32_t i = 0; i < instance->pass_vec_[0]->tri_num_; ++i) {
			bsp_root->tri_list_.push_back(instance->pass_vec_[0]->render_list_->tri_list_[i]);
		}
		IniSplitPlaneVec();
		int32_t best_idx = ChooseBestSplitPlane(bsp_root);
		split_plane_vec_[best_idx].mark_ = true;
		bsp_root->split_plane_ = &split_plane_vec_[best_idx];
		bsp_root->left_child_ = new BSPNode();
		bsp_root->right_child_ = new BSPNode();
		uint32_t size = bsp_root->tri_list_.size();
		for (uint32_t i = 0; i < size; ++i) {
			//先判断不是分割面三角形
			if (bsp_root->tri_list_[i]->bsp_split_id != split_plane_vec_[best_idx].id_) {
				if (bsp_root->tri_list_[i]->bsp_tri_state == BSPTriState::FRONT)
				{
					bsp_root->left_child_->tri_list_.push_back(bsp_root->tri_list_[i]);
				}
				else if (bsp_root->tri_list_[i]->bsp_tri_state == BSPTriState::BEHIND)
				{
					bsp_root->right_child_->tri_list_.push_back(bsp_root->tri_list_[i]);
				}
				else if (bsp_root->tri_list_[i]->bsp_tri_state == BSPTriState::CROSS) {

				}
			}
		}
	}
	else {

	}
}

//BHV树剔除
void KZBSP::BSPTreeCulling(BSPNodePtr bsp_root) {

}

//清除场景BSP树
void KZBSP::CleanSceneBSPTree(BSPNodePtr& bsp_root) {

}

int32_t KZBSP::ChooseBestSplitPlane(BSPNodePtr& bsp_root) {
	
	uint32_t len = split_plane_vec_.size();
	uint32_t best_value = UINT_MAX;
	int32_t best_bsp_idx = -1;
	uint32_t cur_value = 0;
	uint32_t ratio = 5;
	for (uint32_t i = 0; i < len; ++i) 
	{
		if (!split_plane_vec_[i].mark_) {
			uint32_t front_num = 0, behind_num = 0, cross_num = 0;
			uint32_t size = bsp_root->tri_list_.size();
			for (uint32_t j = 0; j <size; ++j)
			{
				if (bsp_root->tri_list_[j]->bsp_split_id == split_plane_vec_[i].id_) {
					continue;
				}
				uint32_t front = 0, behind = 0, in = 0;
				for (uint32_t k = 0; k < 3; ++k)
				{
					KZMath::KZVector4D<float> vec = split_plane_vec_[i].relative_tri->vertex_list[0].pos - bsp_root->tri_list_[j]->vertex_list[k].pos;
					float res = vec.Vector3Dot(split_plane_vec_[i].relative_tri->face_normal);
					if (res > 0) {
						++front;
					}
					else if (res == 0) {
						++in;
					}
					else {
						++behind;
					}
				}
				if (in > 0) {
					bsp_root->tri_list_[j]->bsp_tri_state = BSPTriState::CROSS;
					++cross_num;
				}
				else if(front == 3)
				{
					bsp_root->tri_list_[j]->bsp_tri_state = BSPTriState::FRONT;
					++front_num;
				}
				else if(behind == 3)
				{
					bsp_root->tri_list_[j]->bsp_tri_state = BSPTriState::BEHIND;
					++behind_num;
				}
				else
				{
					bsp_root->tri_list_[j]->bsp_tri_state = BSPTriState::CROSS;
					++cross_num;
				}
			}
			cur_value = front_num > behind_num ? (front_num - behind_num) + ratio * cross_num : (cross_num - front_num) + ratio * cross_num;
			if (cur_value < best_value) {
				best_value = cur_value;
				best_bsp_idx = i;
			}
		}
	}
	return best_bsp_idx;
}

void KZBSP::IniSplitPlaneVec() {
	split_plane_vec_.clear();
	KZPipeLine* instance = KZPipeLine::GetInstance();
	//只针对所有不透明三角形，bsp为了产生portal，透明无法形成portal
	for (uint32_t i = 0; i < instance->pass_vec_[0]->tri_num_; ++i) {
		for (uint32_t i = 0; i < split_plane_vec_.size(); ++i) {
			float x_ratio = split_plane_vec_[i].plane_.n_.x_ / instance->pass_vec_[0]->render_list_->tri_list_[i]->face_normal.x_;
			float y_ratio = split_plane_vec_[i].plane_.n_.y_ / instance->pass_vec_[0]->render_list_->tri_list_[i]->face_normal.y_;
			float z_ratio = split_plane_vec_[i].plane_.n_.z_ / instance->pass_vec_[0]->render_list_->tri_list_[i]->face_normal.z_;
			//法线相同，可能同一平面
			if ((x_ratio - y_ratio <= EPSILON_E5 || x_ratio - y_ratio >= -EPSILON_E5) && (x_ratio - z_ratio <= EPSILON_E5 || x_ratio - z_ratio >= -EPSILON_E5)) {
				//判断三角形的点是否在平面上如果是则共面
				if (split_plane_vec_[i].plane_.n_.x_ * (instance->pass_vec_[0]->render_list_->tri_list_[i]->vertex_list[0].pos.x_ - split_plane_vec_[i].plane_.p0_.x_) +
					split_plane_vec_[i].plane_.n_.y_ * (instance->pass_vec_[0]->render_list_->tri_list_[i]->vertex_list[0].pos.y_ - split_plane_vec_[i].plane_.p0_.y_) +
					split_plane_vec_[i].plane_.n_.z_ * (instance->pass_vec_[0]->render_list_->tri_list_[i]->vertex_list[0].pos.z_ - split_plane_vec_[i].plane_.p0_.z_) == 0)
				{
					instance->pass_vec_[0]->render_list_->tri_list_[i]->bsp_split_id = split_plane_vec_[i].id_;
					continue;
				}
			}
			else
			{
				KZMath::KZPlane3D temp_plane(instance->pass_vec_[0]->render_list_->tri_list_[i]->vertex_list[0].pos.x_,
					instance->pass_vec_[0]->render_list_->tri_list_[i]->vertex_list[0].pos.y_,
					instance->pass_vec_[0]->render_list_->tri_list_[i]->vertex_list[0].pos.z_,
					instance->pass_vec_[0]->render_list_->tri_list_[i]->face_normal.x_,
					instance->pass_vec_[0]->render_list_->tri_list_[i]->face_normal.y_,
					instance->pass_vec_[0]->render_list_->tri_list_[i]->face_normal.z_);
				instance->pass_vec_[0]->render_list_->tri_list_[i]->bsp_split_id = bsp_plane_id;
				BSPSplitPlane temp_split_plane(temp_plane, instance->pass_vec_[0]->render_list_->tri_list_[i], bsp_plane_id);
				++bsp_plane_id;
				split_plane_vec_.push_back(temp_split_plane);
			}
		}
	}
}