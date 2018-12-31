#include "bsp.h"
#include "pipeline.h"
using namespace KZEngine;

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

}

void KZBSP::IniSplitPlaneVec() {
	KZPipeLine* instance = KZPipeLine::GetInstance();
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
				KZMath::KZPlane3D temp_plane;
				temp_plane.n_.x_ = instance->render_list_->tri_list_[i].face_normal.x_;
				temp_plane.n_.y_ = instance->render_list_->tri_list_[i].face_normal.y_;
				temp_plane.n_.z_ = instance->render_list_->tri_list_[i].face_normal.z_;
				temp_plane.p0_.x_ = instance->render_list_->tri_list_[i].vertex_list[0].pos.x_;
				temp_plane.p0_.y_ = instance->render_list_->tri_list_[i].vertex_list[0].pos.y_;
				temp_plane.p0_.z_ = instance->render_list_->tri_list_[i].vertex_list[0].pos.z_;

				BSPSplitPlane temp_split_plane;
				temp_split_plane.id = bsp_plane_id;
				++bsp_plane_id;
				temp_split_plane.mark_ = false;
				temp_split_plane.plane_ = temp_plane;
				temp_split_plane.relative_tri.push_back(&instance->render_list_->tri_list_[i]);
			}
		}
	}

	for (uint32_t i = 0; i < instance->transparent_tri_num_; ++i) {

	}
}