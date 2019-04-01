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

		mesh.face_normal_.resize(mesh.num_face_);
		mesh.vlist_tran_.resize(mesh.num_vertices_);
		CalculateRadian();
		CalculateNormal();

		if (quat != KZMath::KZQuat::ZERO) {
			RotationQuat(quat);
		}
	}
}

KZQuadTerrian::KZQuadTerrian(float width, float height, float vscale, const char* height_map_file_name, const char* texture_map_file_name,
	const KZEngine::Color& ini_color, const KZMath::KZVector4D<float>& world_pos, const KZMath::KZQuat& quat):KZTerrian(height_map_file_name),quat_(quat)
{
	h_img_height_ = height_map_.GetHeight();
	h_img_width_ = height_map_.GetWidth();

	if (!CheckHeightMap())
	{
		return;
	}
	GetMaxLevel(h_img_width_);
	width_ = width;
	height_ = height;
	vscale_ = vscale;
	col_step_ = 0.0f;
	row_step_ = 0.0f;
	world_pos_ = world_pos;
	static_mesh_ = false;
	//uv列的步长
	float uv_col_step = 0.0f;
	//uv行的步长
	float uv_row_step = 0.0f;

	mesh.vlist_local_.resize(h_img_height_ * h_img_width_);
	mesh.index_.resize((h_img_height_ - 1) * (h_img_width_ - 1) * 6);
	
	if (h_img_width_ > 1)
	{
		col_step_ = width_ / (float)(h_img_width_ - 1);
		uv_col_step = 1.0f / (float)(h_img_width_ - 1);
	}

	if (h_img_height_ > 1)
	{
		row_step_ = height_ / (float)(h_img_height_ - 1);
		uv_row_step = 1.0f / (float)(h_img_height_ - 1);
	}

	uint32_t vertex_idx = 0;
	float half_width = width_ / 2;
	float half_height = height_ / 2;
	uint32_t value = 0;
	//生成顶点列表
	for (uint32_t i = 0; i < h_img_height_; ++i)
	{
		for (uint32_t j = 0; j < h_img_width_; ++j)
		{
			mesh.vlist_local_[vertex_idx].pos.x_ = (j * col_step_ - half_width);
			height_map_.GetSingleChannelColor(i, j, value);
			mesh.vlist_local_[vertex_idx].pos.y_ = vscale * (value / 255.0f);
			mesh.vlist_local_[vertex_idx].pos.z_ = (i * row_step_ - half_height);
			mesh.vlist_local_[vertex_idx].pos.w_ = 1;

			mesh.vlist_local_[vertex_idx].color = ini_color;

			if (texture_map_file_name != "")
			{
				mesh.vlist_local_[vertex_idx].uv.x_ = i * uv_row_step;
				mesh.vlist_local_[vertex_idx].uv.y_ = j * uv_col_step;
			}

			++vertex_idx;
		}
	}
	
	mesh.num_vertices_ = h_img_height_ * h_img_width_;
	uint32_t diff_num = h_img_height_ * h_img_width_;
	node_state_table_ = new bool[diff_num]();
	root_node_ = new KZQuadTerrianNode();
	root_node_->level_ = 0;
	root_node_->side_len_ = h_img_width_ - 1;
	root_node_->vertex_idx_ = (diff_num) >> 1;
	BuildQuadTree(root_node_, 1, diff_num);
	IniQuadTreeRough(root_node_, 0, diff_num);

	mesh.vlist_tran_.resize(mesh.num_vertices_);
	UpdateMesh();
	CalculateRadian();
}

void KZQuadTerrian::UpdateMesh() {
	mesh.num_face_ = 0;
	mesh.num_index_ = 0;
	uint32_t queue_id = 0;
	process_queue_[queue_id].push(root_node_);
	while (!process_queue_[queue_id].empty())
	{
		uint32_t size = process_queue_[queue_id].size();
		for (uint32_t i = 0; i < size; ++i)
		{
			KZQuadTerrianNode* cur_node = process_queue_[queue_id].front();
			process_queue_[queue_id].pop();
			bool inside = true;
			if (EvalTerrianNode(cur_node, inside) && EvalNeighbor(cur_node))
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
				//不通过评价系统，更新标记位
				node_state_table_[cur_node->vertex_idx_] = true;

				if (cur_node->level_ < max_level_) {
					for (uint32_t i = 0; i < 4; ++i)
					{
						node_state_table_[cur_node->child_node_[i]->vertex_idx_] = false;
					}
				}
				
				//不通过评价系统,且在视野内
				if (inside)
				{
					AddNodeToMesh(cur_node);
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


	if (quat_ != KZMath::KZQuat::ZERO) {
		RotationQuat(quat_);
	}
	mesh.face_normal_.resize(mesh.num_face_);
	mat_id_.resize(mesh.num_face_, -1);
	CalculateNormal();
}

bool KZQuadTerrian::EvalTerrianNode(KZQuadTerrianNode* node, bool& inside) {
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
		inside = false;
		return false;
	}
	//根据左右平面裁剪
	float w_test = camera_right_over_near * (center_pos.z_ - half_len * row_step_);
	if ((center_pos.x_ - half_len * col_step_ > w_test) || (center_pos.x_ + half_len * col_step_ < -w_test)) {
		inside = false;
		return false;
	}

	//最小节点不可分割
	if (node->level_ == max_level_)
	{
		return false;
	}

	float dis_to_cam = (center_pos.x_ > 0 ? center_pos.x_ : -center_pos.x_) + (center_pos.z_ > 0 ? center_pos.z_ : -center_pos.z_);
	//不通过评价函数
	if ((dis_to_cam * kFactor) / (node->side_len_ * node->rough_) > 1) {
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
	diff_num >>= 1;
	root->diff_num_ = diff_num;
	//最大节点层次
	if (level > max_level_)
	{
		root->child_node_[0] = nullptr;
		root->child_node_[1] = nullptr;
		root->child_node_[2] = nullptr;
		root->child_node_[3] = nullptr;
		return;
	}

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
	root->child_node_[1]->vertex_idx_ = root->child_node_[0]->vertex_idx_ + child_len;
	BuildQuadTree(root->child_node_[1], level + 1, diff_num);

	root->child_node_[3] = new KZQuadTerrianNode();
	root->child_node_[3]->level_ = level;
	root->child_node_[3]->side_len_ = child_len;
	root->child_node_[3]->vertex_idx_ = (root->vertex_idx_ + right_down_vertex_idx) >> 1;
	BuildQuadTree(root->child_node_[3], level + 1, diff_num);

	root->child_node_[2] = new KZQuadTerrianNode();
	root->child_node_[2]->level_ = level;
	root->child_node_[2]->side_len_ = child_len;
	root->child_node_[2]->vertex_idx_ = root->child_node_[3]->vertex_idx_ - child_len;
	BuildQuadTree(root->child_node_[2], level + 1,diff_num);

}

void KZQuadTerrian::IniQuadTreeRough(KZQuadTerrianNode* root, uint32_t level, uint32_t diff_num) {
	diff_num >>= 1;
	if (level == max_level_)
	{
		uint32_t left_up_vertex_idx_ = root->vertex_idx_ - diff_num;
		uint32_t right_down_vertex_idx = root->vertex_idx_ + diff_num;
		uint32_t diff = root->side_len_ >> 1;
		uint32_t up_vertex_idx = left_up_vertex_idx_ + diff;
		uint32_t down_vertex_idx = right_down_vertex_idx - diff;
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
		uint32_t up_vertex_idx = left_up_vertex_idx_ + diff;
		uint32_t down_vertex_idx = right_down_vertex_idx - diff;
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
	while (img_len > 3)
	{
		img_len >>= 1;
		++max_level_;
	}
}

void KZQuadTerrian::AddNodeToMesh(KZQuadTerrianNode* node) {
	uint32_t row = node->vertex_idx_ / h_img_width_;
	uint32_t col = node->vertex_idx_ % h_img_height_;
	uint32_t diff_idx = node->side_len_ * h_img_width_;
	uint32_t left_up_vertex_idx_ = node->vertex_idx_ - node->diff_num_;
	uint32_t right_down_vertex_idx = node->vertex_idx_ + node->diff_num_;
	uint32_t half_side_len = node->side_len_ >> 1;

	if ((row > node->side_len_ && node_state_table_[node->vertex_idx_ - diff_idx]) || (row < node->side_len_))
	{
		mesh.index_[mesh.num_index_] = left_up_vertex_idx_;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = node->vertex_idx_;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = left_up_vertex_idx_ + half_side_len;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = left_up_vertex_idx_ + half_side_len;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = node->vertex_idx_;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = left_up_vertex_idx_ + node->side_len_;
		++mesh.num_index_;

		// 增加两个面
		mesh.num_face_ += 2;
	}
	else
	{
		mesh.index_[mesh.num_index_] = left_up_vertex_idx_;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = node->vertex_idx_;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = left_up_vertex_idx_ + node->side_len_;
		++mesh.num_index_;

		// 增加一个面
		mesh.num_face_ += 1;
	}

	if ((col > node->side_len_ && node_state_table_[node->vertex_idx_ - node->side_len_]) || (col < node->side_len_))
	{
		mesh.index_[mesh.num_index_] = left_up_vertex_idx_;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = node->vertex_idx_ - half_side_len;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = node->vertex_idx_;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = node->vertex_idx_ - half_side_len;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = right_down_vertex_idx - node->side_len_;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = node->vertex_idx_;
		++mesh.num_index_;

		// 增加两个面
		mesh.num_face_ += 2;
	}
	else
	{
		mesh.index_[mesh.num_index_] = left_up_vertex_idx_;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = right_down_vertex_idx - node->side_len_;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = node->vertex_idx_;
		++mesh.num_index_;

		// 增加一个面
		mesh.num_face_ += 1;
	}

	if ((col + node->side_len_ < h_img_width_ && node_state_table_[node->vertex_idx_ + node->side_len_]) || col + node->side_len_ > h_img_width_)
	{
		mesh.index_[mesh.num_index_] = left_up_vertex_idx_ + node->side_len_;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = node->vertex_idx_;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = node->vertex_idx_ + half_side_len;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = node->vertex_idx_;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = right_down_vertex_idx;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = node->vertex_idx_ + half_side_len;
		++mesh.num_index_;

		// 增加两个面
		mesh.num_face_ += 2;
	}
	else
	{
		mesh.index_[mesh.num_index_] = left_up_vertex_idx_ + node->side_len_;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = node->vertex_idx_;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = right_down_vertex_idx;
		++mesh.num_index_;

		// 增加一个面
		mesh.num_face_ += 1;
	}

	if ((row + node->side_len_ < h_img_width_ && node_state_table_[node->vertex_idx_ + diff_idx]) || (row + node->side_len_ > h_img_width_))
	{
		mesh.index_[mesh.num_index_] = right_down_vertex_idx - node->side_len_;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = right_down_vertex_idx - half_side_len;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = node->vertex_idx_;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = node->vertex_idx_;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = right_down_vertex_idx - half_side_len;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = right_down_vertex_idx;
		++mesh.num_index_;

		// 增加两个面
		mesh.num_face_ += 2;
	}
	else
	{
		mesh.index_[mesh.num_index_] = right_down_vertex_idx - node->side_len_;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = right_down_vertex_idx;
		++mesh.num_index_;
		mesh.index_[mesh.num_index_] = node->vertex_idx_;
		++mesh.num_index_;

		// 增加一个面
		mesh.num_face_ += 1;
	}

}

//AddNodeToMesh
//此方法是动态按需要合入顶点和索引，缺点冗余现象，最大需要顶点容量超出
//uint32_t row = node->vertex_idx_ / h_img_width_;
//uint32_t col = node->vertex_idx_ % h_img_height_;
//uint32_t diff_idx = node->side_len_ * h_img_width_;
//uint32_t left_up_vertex_idx_ = node->vertex_idx_ - node->diff_num_;
//uint32_t right_down_vertex_idx = node->vertex_idx_ + node->diff_num_;
//
////首先保证节点中心和四个角点加入
//uint32_t half_side_len = node->side_len_ >> 1;
//uint32_t row_up = row - half_side_len;
//uint32_t row_down = row + half_side_len;
//uint32_t col_left = col - half_side_len;
//uint32_t col_right = col + half_side_len;
//uint32_t value = 0;
//float half_width = width_ / 2;
//float half_height = height_ / 2;
//
//float scale_factor = vscale_ / 255.0f;
//
//float x_left = col_left * col_step_ - half_width;
//float x_right = col_right * col_step_ - half_width;
//float x_mid = col * col_step_ - half_width;
//float z_up = row_up * row_step_ - half_height;
//float z_down = row_down * row_step_ - half_height;
//float z_mid = row * row_step_ - half_height;
//
//uint32_t idx_begin = mesh.num_vertices_;
//
////左上角的顶点
//mesh.vlist_local_[mesh.num_vertices_].pos.x_ = x_left;
//height_map_.GetSingleChannelColor(row_up, col_left, value);
//mesh.vlist_local_[mesh.num_vertices_].pos.y_ = scale_factor * value;
//mesh.vlist_local_[mesh.num_vertices_].pos.z_ = z_up;
//++mesh.num_vertices_;
//
////右上角的顶点
//mesh.vlist_local_[mesh.num_vertices_].pos.x_ = x_right;
//height_map_.GetSingleChannelColor(row_up, col_right, value);
//mesh.vlist_local_[mesh.num_vertices_].pos.y_ = scale_factor * value;
//mesh.vlist_local_[mesh.num_vertices_].pos.z_ = z_up;
//++mesh.num_vertices_;
//
////中间的顶点
//mesh.vlist_local_[mesh.num_vertices_].pos.x_ = x_mid;
//height_map_.GetSingleChannelColor(row, col, value);
//mesh.vlist_local_[mesh.num_vertices_].pos.y_ = scale_factor * value;
//mesh.vlist_local_[mesh.num_vertices_].pos.z_ = z_mid;
//++mesh.num_vertices_;
//
////左下角的顶点
//mesh.vlist_local_[mesh.num_vertices_].pos.x_ = x_left;
//height_map_.GetSingleChannelColor(row_down, col_left, value);
//mesh.vlist_local_[mesh.num_vertices_].pos.y_ = scale_factor * value;
//mesh.vlist_local_[mesh.num_vertices_].pos.z_ = z_down;
//++mesh.num_vertices_;
//
////右下角的顶点
//mesh.vlist_local_[mesh.num_vertices_].pos.x_ = x_right;
//height_map_.GetSingleChannelColor(row_down, col_right, value);
//mesh.vlist_local_[mesh.num_vertices_].pos.y_ = scale_factor * value;
//mesh.vlist_local_[mesh.num_vertices_].pos.z_ = z_down;
//++mesh.num_vertices_;
//
//if ((row > node->side_len_ && node_state_table_[node->vertex_idx_ - diff_idx]) || (row < node->side_len_))
//{
//	mesh.index_[mesh.num_index_] = idx_begin;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = idx_begin + 2;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = mesh.num_vertices_;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = mesh.num_vertices_;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = idx_begin + 2;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = idx_begin + 1;
//	++mesh.num_index_;
//
//	// 增加两个面
//	mesh.num_face_ += 2;
//
//	//上方边的中点加入
//	mesh.vlist_local_[mesh.num_vertices_].pos.x_ = x_mid;
//	height_map_.GetSingleChannelColor(row_up, col, value);
//	mesh.vlist_local_[mesh.num_vertices_].pos.y_ = scale_factor * value;
//	mesh.vlist_local_[mesh.num_vertices_].pos.z_ = z_up;
//	++mesh.num_vertices_;
//}
//else
//{
//	mesh.index_[mesh.num_index_] = idx_begin;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = idx_begin + 2;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = idx_begin + 1;
//	++mesh.num_index_;
//
//	// 增加一个面
//	mesh.num_face_ += 1;
//}
//
//if ((col > node->side_len_ && node_state_table_[node->vertex_idx_ - node->side_len_]) || (col < node->side_len_))
//{
//	mesh.index_[mesh.num_index_] = idx_begin;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = mesh.num_vertices_;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = idx_begin + 2;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = mesh.num_vertices_;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = idx_begin + 3;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = idx_begin + 2;
//	++mesh.num_index_;
//
//	// 增加两个面
//	mesh.num_face_ += 2;
//
//	//左边的中点加入
//	mesh.vlist_local_[mesh.num_vertices_].pos.x_ = x_left;
//	height_map_.GetSingleChannelColor(row, col_left, value);
//	mesh.vlist_local_[mesh.num_vertices_].pos.y_ = scale_factor * value;
//	mesh.vlist_local_[mesh.num_vertices_].pos.z_ = z_mid;
//	++mesh.num_vertices_;
//}
//else
//{
//	mesh.index_[mesh.num_index_] = idx_begin;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = idx_begin + 3;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = idx_begin + 2;
//	++mesh.num_index_;
//
//	// 增加一个面
//	mesh.num_face_ += 1;
//}
//
//if ((col + node->side_len_ < h_img_width_ && node_state_table_[node->vertex_idx_ + node->side_len_]) || col + node->side_len_ > h_img_width_)
//{
//	mesh.index_[mesh.num_index_] = idx_begin + 1;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = idx_begin + 2;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = mesh.num_vertices_;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = idx_begin + 2;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = idx_begin + 4;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = mesh.num_vertices_;
//	++mesh.num_index_;
//
//	// 增加两个面
//	mesh.num_face_ += 2;
//
//	//右边的中点加入
//	mesh.vlist_local_[mesh.num_vertices_].pos.x_ = x_right;
//	height_map_.GetSingleChannelColor(row, col_right, value);
//	mesh.vlist_local_[mesh.num_vertices_].pos.y_ = scale_factor * value;
//	mesh.vlist_local_[mesh.num_vertices_].pos.z_ = z_mid;
//	++mesh.num_vertices_;
//}
//else
//{
//	mesh.index_[mesh.num_index_] = idx_begin + 1;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = idx_begin + 2;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = idx_begin + 4;
//	++mesh.num_index_;
//
//	// 增加一个面
//	mesh.num_face_ += 1;
//}
//
//if ((row + node->side_len_ < h_img_width_ && node_state_table_[node->vertex_idx_ + diff_idx]) || (row + node->side_len_ > h_img_width_))
//{
//	mesh.index_[mesh.num_index_] = idx_begin + 3;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = mesh.num_vertices_;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = idx_begin + 2;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = idx_begin + 2;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = mesh.num_vertices_;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = idx_begin + 4;
//	++mesh.num_index_;
//
//	// 增加两个面
//	mesh.num_face_ += 2;
//
//	//下边的中点加入
//	mesh.vlist_local_[mesh.num_vertices_].pos.x_ = x_mid;
//	height_map_.GetSingleChannelColor(row_down, col, value);
//	mesh.vlist_local_[mesh.num_vertices_].pos.y_ = scale_factor * value;
//	mesh.vlist_local_[mesh.num_vertices_].pos.z_ = z_down;
//	++mesh.num_vertices_;
//}
//else
//{
//	mesh.index_[mesh.num_index_] = idx_begin + 3;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = idx_begin + 4;
//	++mesh.num_index_;
//	mesh.index_[mesh.num_index_] = idx_begin + 2;
//	++mesh.num_index_;
//
//	// 增加一个面
//	mesh.num_face_ += 1;
//}

bool KZQuadTerrian::CheckHeightMap()
{
	if (h_img_width_ != h_img_height_ && h_img_width_ != 0)
	{
		return false;
	}

	//如果该数是2的乘方，则该数的二进制表示仅包含一个1，那么该数与他的减一的数相与，一定为0； 否则该数一定不是2的乘方
	uint32_t res = (h_img_width_ - 1) & (h_img_width_ - 2);
	if (res == 0)
	{
		return true;
	}

	return false;
}