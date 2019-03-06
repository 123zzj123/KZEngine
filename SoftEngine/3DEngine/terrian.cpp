#include "terrian.h"
using namespace KZEngine;

KZTerrian::KZTerrian(float width, float height, float vscale, const char* height_map_file_name, const char* texture_map_file_name,
	const KZEngine::Color& ini_color, const KZMath::KZVector4D<float>& world_pos, const KZMath::KZQuat& quat): height_map_(height_map_file_name)
{

	h_img_height_ = height_map_.GetHeight();
	h_img_width_ = height_map_.GetWidth();

	if (h_img_height_ != 0 && h_img_width_ != 0)
	{
		name_ = "Terrain";

		world_pos_ = world_pos;
		ux_ = KZMath::KZVector4D<float>(1, 0, 0, 0);
		uy_ = KZMath::KZVector4D<float>(0, 1, 0, 0);
		uz_ = KZMath::KZVector4D<float>(0, 0, 1, 0);

		num_vertices_ = h_img_height_ * h_img_width_;
		num_face_ = (h_img_height_ - 1) * (h_img_width_ - 1) * 2;
		num_index_ = num_face_ * 3;

		vlist_local_.resize(num_vertices_);
		index_.reserve(num_index_);

		//�������в���
		float col_step = 0.0f;
		//�������в���
		float row_step = 0.0f;
		//uv�еĲ���
		float uv_col_step = 0.0f;
		//uv�еĲ���
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
		//���ɶ����б�,�����б�
		for (uint32_t i = 0; i < h_img_height_; ++i)
		{
			for (uint32_t j = 0; j < h_img_width_; ++j)
			{
				vlist_local_[vertex_idx].pos.x_ = (i * row_step - half_width);
				height_map_.GetSingleChannelColor(i, j, value);
				vlist_local_[vertex_idx].pos.y_ = vscale * (value / 255.0f);
				vlist_local_[vertex_idx].pos.z_ = (j * col_step - half_height);
				vlist_local_[vertex_idx].pos.w_ = 1;

				vlist_local_[vertex_idx].color = ini_color;

				if (texture_map_file_name != "")
				{
					vlist_local_[vertex_idx].uv.x_ = i * uv_row_step;
					vlist_local_[vertex_idx].uv.y_ = j * uv_col_step;
				}

				if (i != h_img_height_ - 1 || j != h_img_width_ - 1)
				{
					uint32_t base_index = vertex_idx;
					index_.push_back(base_index);
					index_.push_back(base_index + h_img_width_);
					index_.push_back(base_index + 1);

					index_.push_back(base_index + 1);
					index_.push_back(base_index + h_img_width_);
					index_.push_back(base_index + h_img_width_ + 1);
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