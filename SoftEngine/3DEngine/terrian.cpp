#include "terrian.h"
using namespace KZEngine;

KZTerrian::KZTerrian(float width, float height, float vscale, const char* height_map_file_name, const char* texture_map_file_name,
	const KZEngine::Color& ini_color, const KZMath::KZVector4D<float>& world_pos, const KZMath::KZQuat& quat): height_map_(height_map_file_name)
{

	h_img_height_ = height_map_.GetHeight();
	h_img_width_ = height_map_.GetWidth();
	width_ = width;
	height_ = height;
	vscale_ = vscale;
	if (h_img_height_ != 0 && h_img_width_ != 0)
	{
		name_ = "Terrain";

		world_pos_ = world_pos;
		ux_ = KZMath::KZVector4D<float>(1, 0, 0, 0);
		uy_ = KZMath::KZVector4D<float>(0, 1, 0, 0);
		uz_ = KZMath::KZVector4D<float>(0, 0, 1, 0);

		mesh.num_vertices_ = h_img_height_ * h_img_width_;
		mesh.num_face_ = (h_img_height_ - 1) * (h_img_width_ - 1) * 2;
		mesh.num_index_ = mesh.num_face_ * 3;

		mesh.vlist_local_.resize(mesh.num_vertices_);
		mesh.index_.reserve(mesh.num_index_);

		//三角形列步长
		float col_step = 0.0f;
		//三角形行步长
		float row_step = 0.0f;
		//uv列的步长
		float uv_col_step = 0.0f;
		//uv行的步长
		float uv_row_step = 0.0f;

		if (h_img_width_ > 1)
		{
			col_step = width / (float)(h_img_width_ - 1);
		}

		if (h_img_height_ > 1)
		{
			row_step = height / (float)(h_img_height_ - 1);
		}

		uv_col_step = 1.0f / (float)(h_img_width_ - 1);
		uv_row_step = 1.0f / (float)(h_img_height_ - 1);

		uint32_t vertex_idx = 0;
		float half_width = width / 2;
		float half_height = height / 2;
		uint32_t value = 0;
		//生成顶点列表,索引列表
		for (uint32_t i = 0; i < h_img_height_; ++i)
		{
			for (uint32_t j = 0; j < h_img_width_; ++j)
			{
				mesh.vlist_local_[vertex_idx].pos.x_ = (j * col_step - half_width);
				height_map_.GetSingleChannelColor(i, j, value);
				mesh.vlist_local_[vertex_idx].pos.y_ = vscale * (value / 255.0f);
				mesh.vlist_local_[vertex_idx].pos.z_ = (i * row_step - half_height);
				mesh.vlist_local_[vertex_idx].pos.w_ = 1;

				mesh.vlist_local_[vertex_idx].color = ini_color;

				if (texture_map_file_name != "")
				{
					mesh.vlist_local_[vertex_idx].uv.x_ = i * uv_row_step;
					mesh.vlist_local_[vertex_idx].uv.y_ = j * uv_col_step;
				}

				if (i != h_img_height_ - 1 || j != h_img_width_ - 1)
				{
					uint32_t base_index = vertex_idx;
					mesh.index_.push_back(base_index);
					mesh.index_.push_back(base_index + h_img_width_);
					mesh.index_.push_back(base_index + 1);

					mesh.index_.push_back(base_index + 1);
					mesh.index_.push_back(base_index + h_img_width_);
					mesh.index_.push_back(base_index + h_img_width_ + 1);
				}

				++vertex_idx;
			}
		}


		CalculateRadian();
		CalculateNormal();

		if (quat != KZMath::KZQuat::ZERO) {
			RotationQuat(quat);
		}
	}
}

KZQuadTerrian::KZQuadTerrian(float width, float height, float vscale, const char* height_map_file_name, const char* texture_map_file_name,
	const KZEngine::Color& ini_color, const KZMath::KZVector4D<float>& world_pos, const KZMath::KZQuat& quat):KZTerrian(height_map_file_name)
{
	h_img_height_ = height_map_.GetHeight();
	h_img_width_ = height_map_.GetWidth();
	max_level_ = h_img_width_ >> 1;
	GetMaxLevel(h_img_width_);
	width_ = width;
	height_ = height;
	vscale_ = vscale;
	col_step_ = 0.0f;
	row_step_ = 0.0f;
	
	if (h_img_width_ > 1)
	{
		col_step_ = width_ / (float)(h_img_width_ - 1);
	}

	if (h_img_height_ > 1)
	{
		row_step_ = height_ / (float)(h_img_height_ - 1);
	}

	uint32_t diff_num = h_img_height_ * h_img_width_;
	node_state_table_ = new bool[diff_num]();
	root_node_ = new KZQuadTerrianNode();
	root_node_->level_ = 0;
	root_node_->side_len_ = h_img_width_ - 1;
	root_node_->vertex_idx_ = (diff_num) >> 1;
	BuildQuadTree(root_node_, 1, diff_num);
	IniQuadTreeRough(root_node_, 1, diff_num);
	UpdateMesh();
}

void KZQuadTerrian::UpdateMesh() {
	mesh.vlist_local_.clear();
	mesh.index_.clear();
	uint32_t queue_id = 0;
	process_queue_[queue_id].push(root_node_);
	while (!process_queue_[queue_id].empty())
	{
		uint32_t size = process_queue_[queue_id].size();
		for (uint32_t i = 0; i < size; ++i)
		{
			KZQuadTerrianNode* cur_node = process_queue_[queue_id].front();
			process_queue_[queue_id].pop();
			if (EvalTerrianNode(cur_node) && EvalNeighbor(cur_node))
			{
				//通过评价系统
				node_state_table_[cur_node->vertex_idx_] = true;
				for (uint32_t i = 0; i < 4; ++i)
				{
					node_state_table_[cur_node->child_node_[i]->vertex_idx_] = true;
				}


				if (queue_id == 0)
				{
					for (uint32_t i = 0; i < 4; ++i)
					{
						process_queue_[1].push(cur_node->child_node_[i]);
					}
				}
				else
				{
					for (uint32_t i = 0; i < 4; ++i)
					{
						process_queue_[0].push(cur_node->child_node_[i]);
					}
				}
			}
			else
			{
				//不通过评价系统
				node_state_table_[cur_node->vertex_idx_] = true;
				for (uint32_t i = 0; i < 4; ++i)
				{
					node_state_table_[cur_node->child_node_[i]->vertex_idx_] = false;
				}
			}

		}
		
		if (queue_id == 1)
		{
			queue_id = 0;
		}
		else
		{
			queue_id = 1;
		}
	}

}

bool KZQuadTerrian::EvalTerrianNode(KZQuadTerrianNode* node) {
	if (node == nullptr)
	{
		return false;
	}

	float half_width = width_ / 2;
	float half_height = height_ / 2;

	uint32_t row = node->vertex_idx_ / h_img_width_;
	uint32_t col = node->vertex_idx_ % h_img_height_;
	KZMath::KZVector4D<float> center_pos(col * col_step_ - half_width, 0.0f, row * row_step_ - half_height);
	center_pos += world_pos_;
	KZCamera* main_camera = KZCameraManager::GetInstance()->GetMainCameraInstance();
	float camera_right_over_near = main_camera->GetViewRight() / main_camera->GetCameraNearClip();
	float camera_top_over_near = main_camera->GetViewTop() / main_camera->GetCameraNearClip();
	KZMath::KZMatrix44 view;
	main_camera->GetViewMatrix(view);
	center_pos = view * center_pos;
	uint32_t half_len = (node->side_len_ >> 1);
	//根据远近平面裁剪
	if ((center_pos.z_ + half_len * row_step_ < main_camera->GetCameraFarClip()) || (center_pos.z_ - half_len * row_step_ > main_camera->GetCameraNearClip())) {
		return false;
	}
	//根据左右平面裁剪
	float w_test = camera_right_over_near * center_pos.z_;
	if ((center_pos.x_ - half_len * col_step_ > w_test) || (center_pos.x_ + half_len * col_step_ < -w_test)) {
		return false;
	}
	float dis_to_cam = (center_pos.x_ > 0 ? center_pos.x_ : -center_pos.x_) + (center_pos.z_ > 0 ? center_pos.z_ : -center_pos.z_);
	//通过评价函数
	if (dis_to_cam / (node->side_len_ * node->rough_ * kFactor) > 1) {
		return false;
	}

	return true;
}

bool KZQuadTerrian::EvalNeighbor(KZQuadTerrianNode* node) {
	if (node->level_ == 0)
	{
		return true;
	}
	else
	{
		bool result = true;
		uint32_t row = node->vertex_idx_ / h_img_width_;
		uint32_t col = node->vertex_idx_ % h_img_height_;
		uint32_t diff_idx = node->side_len_ * h_img_width_;
		if (col > node->side_len_)
		{
			result &= node_state_table_[node->vertex_idx_ - node->side_len_];
		}
		if (col + node->side_len_ < h_img_width_) {
			result &= node_state_table_[node->vertex_idx_ + node->side_len_];
		}
		if (row > node->side_len_)
		{
			result &= node_state_table_[node->vertex_idx_ - diff_idx];
		}
		if (row + node->side_len_ < h_img_width_) {
			result &= node_state_table_[node->vertex_idx_ + diff_idx];
		}
		return result;
	}
}

void KZQuadTerrian::BuildQuadTree(KZQuadTerrianNode* root, uint32_t level, uint32_t diff_num) {
	//最大节点层次
	if (level == max_level_)
	{
		root->child_node_[0] = nullptr;
		root->child_node_[1] = nullptr;
		root->child_node_[2] = nullptr;
		root->child_node_[3] = nullptr;
		return;
	}

	diff_num >> 1;
	uint32_t left_up_vertex_idx = root->vertex_idx_ - diff_num;
	uint32_t right_down_vertex_idx = root->vertex_idx_ + diff_num;
	uint32_t child_len = root->side_len_ >> 1;
	root->child_node_[0] = new KZQuadTerrianNode();
	root->child_node_[0]->level_ = level;
	root->child_node_[0]->side_len_ = child_len;
	root->child_node_[0]->vertex_idx_ = (root->vertex_idx_ + left_up_vertex_idx) >> 1;
	BuildQuadTree(root->child_node_[0], level + 1, diff_num);

	root->child_node_[1] = new KZQuadTerrianNode();
	root->child_node_[1]->level_ = level;
	root->child_node_[1]->side_len_ = child_len;
	root->child_node_[1]->level_ = root->child_node_[0]->vertex_idx_ + child_len;
	BuildQuadTree(root->child_node_[1], level + 1, diff_num);

	root->child_node_[4] = new KZQuadTerrianNode();
	root->child_node_[4]->level_ = level;
	root->child_node_[4]->side_len_ = child_len;
	root->child_node_[4]->vertex_idx_ = (root->vertex_idx_ + right_down_vertex_idx) >> 1;
	BuildQuadTree(root->child_node_[2], level + 1, diff_num);

	root->child_node_[3] = new KZQuadTerrianNode();
	root->child_node_[3]->level_ = level;
	root->child_node_[3]->side_len_ = child_len;
	root->child_node_[3]->level_ = root->child_node_[4]->vertex_idx_ - child_len;
	BuildQuadTree(root->child_node_[3], level + 1,diff_num);

}

void KZQuadTerrian::IniQuadTreeRough(KZQuadTerrianNode* root, uint32_t level, uint32_t diff_num) {
	diff_num >> 1;
	if (level == max_level_)
	{
		uint32_t left_up_vertex_idx_ = root->vertex_idx_ - diff_num;
		uint32_t right_down_vertex_idx = root->vertex_idx_ + diff_num;
		uint32_t diff = root->side_len_ >> 1;
		uint32_t up_vertex_idx = left_up_vertex_idx_ + root->side_len_ >> 1;
		uint32_t down_vertex_idx = right_down_vertex_idx - root->side_len_ >> 1;
		uint32_t left_vertex_idx = root->vertex_idx_ - diff;
		uint32_t right_vertex_idx = root->vertex_idx_ + diff;
		
		root->rough_ = 0;
		uint32_t val[4];
		height_map_.GetSingleChannelColor(up_vertex_idx, val[0]);
		height_map_.GetSingleChannelColor(down_vertex_idx, val[1]);
		height_map_.GetSingleChannelColor(left_vertex_idx, val[2]);
		height_map_.GetSingleChannelColor(right_vertex_idx, val[3]);
		for (uint32_t i = 0; i < 4; ++i)
		{
			if (val[i] > root->rough_)
			{
				root->rough_ = val[i];
			}
		}
		return;
	}
	else
	{
		IniQuadTreeRough(root->child_node_[0], level + 1, diff_num);
		IniQuadTreeRough(root->child_node_[1], level + 1, diff_num);
		IniQuadTreeRough(root->child_node_[2], level + 1, diff_num);
		IniQuadTreeRough(root->child_node_[3], level + 1, diff_num);

		uint32_t left_up_vertex_idx_ = root->vertex_idx_ - diff_num;
		uint32_t right_down_vertex_idx = root->vertex_idx_ + diff_num;
		uint32_t diff = root->side_len_ >> 1;
		uint32_t up_vertex_idx = left_up_vertex_idx_ + root->side_len_ >> 1;
		uint32_t down_vertex_idx = right_down_vertex_idx - root->side_len_ >> 1;
		uint32_t left_vertex_idx = root->vertex_idx_ - diff;
		uint32_t right_vertex_idx = root->vertex_idx_ + diff;

		root->rough_ = 0;
		uint32_t val[8];
		height_map_.GetSingleChannelColor(up_vertex_idx, val[0]);
		height_map_.GetSingleChannelColor(down_vertex_idx, val[1]);
		height_map_.GetSingleChannelColor(left_vertex_idx, val[2]);
		height_map_.GetSingleChannelColor(right_vertex_idx, val[3]);
		val[4] = root->child_node_[0]->rough_;
		val[5] = root->child_node_[1]->rough_;
		val[6] = root->child_node_[2]->rough_;
		val[7] = root->child_node_[3]->rough_;
		for (uint32_t i = 0; i < 8; ++i)
		{
			if (val[i] > root->rough_)
			{
				root->rough_ = val[i];
			}
		}
		return;
	}
}

void KZQuadTerrian::GetMaxLevel(uint32_t img_len) {
	max_level_ = 0;
	while (img_len > 1)
	{
		img_len >> 1;
		++max_level_;
	}
}