#include "bsp.h"
#include "pipeline.h"
using namespace KZEngine;

uint32_t KZBSP::bsp_plane_id = 0;
vector<BSPSplitPlane> KZBSP::split_plane_vec_;

//建造场景BSP树
void KZBSP::BuildSceneBSPTree(BSPNodePtr& bsp_root, uint32_t level) {
	if (level == 0) {
		IniSplitPlaneVec();
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

void KZBSP::ChooseBestSplitPlane() {
	uint32_t len = split_plane_vec_.size();
	for (uint32_t i = 0; i < len; ++i) {

	}
}

void KZBSP::IniSplitPlaneVec() {
	split_plane_vec_.clear();
	KZPipeLine* instance = KZPipeLine::GetInstance();
	//只针对所有不透明三角形，bsp为了产生portal，透明无法形成portal
	for (uint32_t i = 0; i < instance->tri_num_; ++i) {
		for (uint32_t i = 0; i < split_plane_vec_.size(); ++i) {
			float x_ratio = split_plane_vec_[i].plane_.n_.x_ / instance->render_list_->tri_list_[i].face_normal.x_;
			float y_ratio = split_plane_vec_[i].plane_.n_.y_ / instance->render_list_->tri_list_[i].face_normal.y_;
			float z_ratio = split_plane_vec_[i].plane_.n_.z_ / instance->render_list_->tri_list_[i].face_normal.z_;
			//法线相同，可能同一平面
			if ((x_ratio - y_ratio <= EPSILON_E5 || x_ratio - y_ratio >= -EPSILON_E5) && (x_ratio - z_ratio <= EPSILON_E5 || x_ratio - z_ratio >= -EPSILON_E5)) {
				//判断三角形的点是否在平面上如果是则共面
				if (split_plane_vec_[i].plane_.n_.x_ * (instance->render_list_->tri_list_[i].vertex_list[0].pos.x_ - split_plane_vec_[i].plane_.p0_.x_) +
					split_plane_vec_[i].plane_.n_.y_ * (instance->render_list_->tri_list_[i].vertex_list[0].pos.y_ - split_plane_vec_[i].plane_.p0_.y_) +
					split_plane_vec_[i].plane_.n_.z_ * (instance->render_list_->tri_list_[i].vertex_list[0].pos.z_ - split_plane_vec_[i].plane_.p0_.z_) == 0)
				{
					continue;
				}
			}
			else
			{
				KZMath::KZPlane3D temp_plane(instance->render_list_->tri_list_[i].vertex_list[0].pos.x_,
					instance->render_list_->tri_list_[i].vertex_list[0].pos.y_,
					instance->render_list_->tri_list_[i].vertex_list[0].pos.z_,
					instance->render_list_->tri_list_[i].face_normal.x_,
					instance->render_list_->tri_list_[i].face_normal.y_,
					instance->render_list_->tri_list_[i].face_normal.z_);

				BSPSplitPlane temp_split_plane(temp_plane, &instance->render_list_->tri_list_[i], bsp_plane_id);
				split_plane_vec_.push_back(temp_split_plane);
			}
		}
	}
}