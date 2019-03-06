#include "bsp.h"
#include "pipeline.h"
using namespace KZEngine;

uint32_t KZBSP::bsp_plane_id = 0;
vector<BSPSplitPlane> KZBSP::split_plane_vec_;
vector<TrianglePtr> KZBSP::divide_tri_vec;

//建造场景BSP树
void KZBSP::BuildSceneBSPTree(BSPNodePtr& bsp_root, uint32_t level) {
	if (level == 0) {
		KZPipeLine* instance = KZPipeLine::GetInstance();
		bsp_root = new BSPNode();
		for (uint32_t i = 0; i < instance->pass_vec_[0]->tri_num_; ++i) {
			bsp_root->tri_list_.push_back(instance->pass_vec_[0]->render_list_->tri_list_[i]);
		}
		IniSplitPlaneVec();
		ChooseBestSplitPlane(bsp_root);
		bsp_root->tri_list_.clear();
		BuildSceneBSPTree(bsp_root->left_child_, level + 1);
		BuildSceneBSPTree(bsp_root->right_child_, level + 1);
	}
	else {
		// 到达叶子节点
		if (bsp_root->tri_list_.size() == 1)
		{
			bsp_root->left_child_ = nullptr;
			bsp_root->right_child_ = nullptr;
			bsp_root->split_plane_ = nullptr;
		}
		else
		{
			ChooseBestSplitPlane(bsp_root);
			bsp_root->tri_list_.clear();
			BuildSceneBSPTree(bsp_root->left_child_, level + 1);
			BuildSceneBSPTree(bsp_root->right_child_, level + 1);
		}
	}
}

//BSP树剔除
void KZBSP::BSPTreeCulling(BSPNodePtr bsp_root) {

}

//清除场景BSP树
void KZBSP::CleanSceneBSPTree(BSPNodePtr& bsp_root) {
	if (bsp_root != nullptr) {
		CleanSceneBSPTree(bsp_root->left_child_);
		CleanSceneBSPTree(bsp_root->right_child_);
		delete bsp_root;
	}
	return;
}

void KZBSP::ChooseBestSplitPlane(BSPNodePtr& bsp_root) {
	
	uint32_t len = (uint32_t)split_plane_vec_.size();
	uint32_t best_value = UINT_MAX;
	int32_t best_bsp_idx = -1;
	uint32_t cur_value = 0;
	uint32_t ratio = 5;
	uint32_t size = (uint32_t)bsp_root->tri_list_.size();
	for (uint32_t i = 0; i < len; ++i) 
	{
		if (!split_plane_vec_[i].mark_) {
			uint32_t front_num = 0, behind_num = 0, cross_num = 0;
			
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
				if (front != 0 && behind == 0)
				{
					++front_num;
				}
				else if (behind != 0 && front == 0)
				{
					++behind_num;
				}
				else
				{
					++cross_num;
				}
			}
			cur_value = front_num > behind_num ? (front_num - behind_num) + ratio * cross_num : (behind_num - front_num) + ratio * cross_num;
			if (cur_value < best_value) {
				best_value = cur_value;
				best_bsp_idx = i;
			}
		}
	}
	split_plane_vec_[best_value].mark_ = true;
	bsp_root->split_plane_ = &split_plane_vec_[best_value];
	bsp_root->left_child_ = new BSPNode();
	bsp_root->right_child_ = new BSPNode();
	BSPVertexState stateArray[3];
	for (uint32_t i = 0; i < size; ++i) {
		if (bsp_root->tri_list_[i]->bsp_split_id == split_plane_vec_[best_value].id_) {
			continue;
		}
		uint32_t front = 0, behind = 0, in = 0;

		for (uint32_t k = 0; k < 3; ++k)
		{
			KZMath::KZVector4D<float> vec = split_plane_vec_[best_value].relative_tri->vertex_list[0].pos - bsp_root->tri_list_[i]->vertex_list[k].pos;
			float res = vec.Vector3Dot(split_plane_vec_[best_value].relative_tri->face_normal);
			if (res > 0) {
				++front;
				stateArray[k] = BSPVertexState::FRONT;
			}
			else if (res == 0) {
				++in;
				stateArray[k] = BSPVertexState::INSIDE;
			}
			else {
				++behind;
				stateArray[k] = BSPVertexState::BEHIND;
			}
		}
		if (front != 0 && behind == 0)
		{
			bsp_root->left_child_->tri_list_.push_back(bsp_root->tri_list_[i]);
		}
		else if (behind != 0 && front == 0)
		{
			bsp_root->right_child_->tri_list_.push_back(bsp_root->tri_list_[i]);
		}
		else
		{
			//三角形被分割面分割，两种情况：1.两个顶点在同一边，一个在另外一边; 2.一个顶点在分割面上，另外两个顶点分别在两边
			for (uint32_t k = 0; k < 2; ++k) {
				uint32_t anotherIdx = (k + 2) % 3;
				if (stateArray[k] == stateArray[k + 1]) {	
					KZMath::KZPoint3D temp0, temp1;
					float t1, t2;
					KZMath::KZLine3D line1(bsp_root->tri_list_[i]->vertex_list[k].pos.x_, bsp_root->tri_list_[i]->vertex_list[k].pos.y_, bsp_root->tri_list_[i]->vertex_list[k].pos.z_,
						bsp_root->tri_list_[i]->vertex_list[anotherIdx].pos.x_, bsp_root->tri_list_[i]->vertex_list[anotherIdx].pos.y_, bsp_root->tri_list_[i]->vertex_list[anotherIdx].pos.z_);
					KZMath::KZLine3D line2(bsp_root->tri_list_[i]->vertex_list[k + 1].pos.x_, bsp_root->tri_list_[i]->vertex_list[k + 1].pos.y_, bsp_root->tri_list_[i]->vertex_list[k + 1].pos.z_,
						bsp_root->tri_list_[i]->vertex_list[anotherIdx].pos.x_, bsp_root->tri_list_[i]->vertex_list[anotherIdx].pos.y_, bsp_root->tri_list_[i]->vertex_list[anotherIdx].pos.z_);
					KZMath::IntersetLine3DInPlane3D(&line1, &split_plane_vec_[best_value].plane_, &temp0, t1);
					
					KZMath::IntersetLine3DInPlane3D(&line2, &split_plane_vec_[best_value].plane_, &temp1, t2);
					KZEngine::Vertex temp_v0(temp0.x_, temp0.y_, temp0.z_), temp_v1(temp1.x_, temp1.y_, temp1.z_);
					KZEngine::TrianglePtr new_tri1 = new KZEngine::Triangle();
					KZEngine::TrianglePtr new_tri2 = new KZEngine::Triangle();
					KZEngine::TrianglePtr new_tri3 = new KZEngine::Triangle();
					divide_tri_vec.push_back(new_tri1);
					divide_tri_vec.push_back(new_tri2);
					divide_tri_vec.push_back(new_tri3);
					new_tri1->vertex_list[0] = bsp_root->tri_list_[i]->vertex_list[k];
					new_tri1->vertex_list[1] = bsp_root->tri_list_[i]->vertex_list[k + 1];
					new_tri1->vertex_list[2] = temp_v1;
					new_tri1->bsp_split = true;
					new_tri1->bsp_split_id = bsp_root->tri_list_[i]->bsp_split_id;

					new_tri2->vertex_list[0] = bsp_root->tri_list_[i]->vertex_list[k];
					new_tri2->vertex_list[1] = temp_v1;
					new_tri2->vertex_list[2] = temp_v0;
					new_tri2->bsp_split = true;
					new_tri2->bsp_split_id = bsp_root->tri_list_[i]->bsp_split_id;

					new_tri3->vertex_list[0] = temp_v0;
					new_tri3->vertex_list[1] = temp_v1;
					new_tri3->vertex_list[2] = bsp_root->tri_list_[i]->vertex_list[anotherIdx];
					new_tri3->bsp_split = true;
					new_tri3->bsp_split_id = bsp_root->tri_list_[i]->bsp_split_id;

					if (stateArray[k] == BSPVertexState::FRONT)
					{
						bsp_root->left_child_->tri_list_.push_back(new_tri1);
						bsp_root->left_child_->tri_list_.push_back(new_tri2);
						bsp_root->right_child_->tri_list_.push_back(new_tri3);
					}
					else
					{
						bsp_root->right_child_->tri_list_.push_back(new_tri1);
						bsp_root->right_child_->tri_list_.push_back(new_tri2);
						bsp_root->left_child_->tri_list_.push_back(new_tri3);
					}
				}
				else if(stateArray[k] == BSPVertexState::INSIDE) {
					KZMath::KZPoint3D temp0;
					float t1;
					KZMath::KZLine3D line1(bsp_root->tri_list_[i]->vertex_list[k + 1].pos.x_, bsp_root->tri_list_[i]->vertex_list[k + 1].pos.y_, bsp_root->tri_list_[i]->vertex_list[k + 1].pos.z_,
						bsp_root->tri_list_[i]->vertex_list[anotherIdx].pos.x_, bsp_root->tri_list_[i]->vertex_list[anotherIdx].pos.y_, bsp_root->tri_list_[i]->vertex_list[anotherIdx].pos.z_);
					KZMath::IntersetLine3DInPlane3D(&line1, &split_plane_vec_[best_value].plane_, &temp0, t1);
					KZEngine::Vertex temp_v0(temp0.x_, temp0.y_, temp0.z_);
					KZEngine::TrianglePtr new_tri1 = new KZEngine::Triangle();
					KZEngine::TrianglePtr new_tri2 = new KZEngine::Triangle();
					divide_tri_vec.push_back(new_tri1);
					divide_tri_vec.push_back(new_tri2);
					new_tri1->vertex_list[0] = bsp_root->tri_list_[i]->vertex_list[k];
					new_tri1->vertex_list[1] = bsp_root->tri_list_[i]->vertex_list[k + 1];
					new_tri1->vertex_list[2] = temp_v0;
					new_tri1->bsp_split = true;
					new_tri1->bsp_split_id = bsp_root->tri_list_[i]->bsp_split_id;

					new_tri2->vertex_list[0] = bsp_root->tri_list_[i]->vertex_list[k];
					new_tri2->vertex_list[1] = temp_v0;
					new_tri2->vertex_list[2] = bsp_root->tri_list_[i]->vertex_list[anotherIdx];
					new_tri2->bsp_split = true;
					new_tri2->bsp_split_id = bsp_root->tri_list_[i]->bsp_split_id;

					if (stateArray[k + 1] == BSPVertexState::FRONT) {
						bsp_root->left_child_->tri_list_.push_back(new_tri1);
						bsp_root->right_child_->tri_list_.push_back(new_tri2);
					}
					else
					{
						bsp_root->right_child_->tri_list_.push_back(new_tri1);
						bsp_root->left_child_->tri_list_.push_back(new_tri2);
					}
				}
				else if (stateArray[k + 1] == BSPVertexState::INSIDE) {
					KZMath::KZPoint3D temp0;
					float t1;
					KZMath::KZLine3D line1(bsp_root->tri_list_[i]->vertex_list[k].pos.x_, bsp_root->tri_list_[i]->vertex_list[k].pos.y_, bsp_root->tri_list_[i]->vertex_list[k].pos.z_,
						bsp_root->tri_list_[i]->vertex_list[anotherIdx].pos.x_, bsp_root->tri_list_[i]->vertex_list[anotherIdx].pos.y_, bsp_root->tri_list_[i]->vertex_list[anotherIdx].pos.z_);
					KZMath::IntersetLine3DInPlane3D(&line1, &split_plane_vec_[best_value].plane_, &temp0, t1);
					KZEngine::Vertex temp_v0(temp0.x_, temp0.y_, temp0.z_);
					KZEngine::TrianglePtr new_tri1 = new KZEngine::Triangle();
					KZEngine::TrianglePtr new_tri2 = new KZEngine::Triangle();
					divide_tri_vec.push_back(new_tri1);
					divide_tri_vec.push_back(new_tri2);
					new_tri1->vertex_list[0] = bsp_root->tri_list_[i]->vertex_list[k];
					new_tri1->vertex_list[1] = bsp_root->tri_list_[i]->vertex_list[k + 1];
					new_tri1->vertex_list[2] = temp_v0;
					new_tri1->bsp_split = true;
					new_tri1->bsp_split_id = bsp_root->tri_list_[i]->bsp_split_id;

					new_tri2->vertex_list[0] = bsp_root->tri_list_[i]->vertex_list[k + 1];
					new_tri2->vertex_list[1] = bsp_root->tri_list_[i]->vertex_list[anotherIdx];
					new_tri2->vertex_list[2] = temp_v0;
					new_tri2->bsp_split = true;
					new_tri2->bsp_split_id = bsp_root->tri_list_[i]->bsp_split_id;

					if (stateArray[k] == BSPVertexState::FRONT) {
						bsp_root->left_child_->tri_list_.push_back(new_tri1);
						bsp_root->right_child_->tri_list_.push_back(new_tri2);
					}
					else
					{
						bsp_root->right_child_->tri_list_.push_back(new_tri1);
						bsp_root->left_child_->tri_list_.push_back(new_tri2);
					}
				}
			}
		}
	}
	return;
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