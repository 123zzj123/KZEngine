#include"pipeline.h"
using namespace KZEngine;

void KZPipeLine::LoadFromFile() {
	//to do
}

//创建cube
void KZPipeLine::CreateCube() {
	object_num_ = 1;
	KZObject cube_obj;
	cube_obj.id_ = 1;
	cube_obj.name_ = "cube";
	cube_obj.world_pos_ = KZMath::KZVector4D(0, 0, -5);
	cube_obj.ux_ = KZMath::KZVector4D(1, 0, 0, 0);
	cube_obj.uy_ = KZMath::KZVector4D(0, 1, 0, 0);
	cube_obj.uz_ = KZMath::KZVector4D(0, 0, 1, 0);
	cube_obj.num_vertices_ = 8;
	cube_obj.vlist_local_.reserve(cube_obj.num_vertices_);
	object_active_.reserve(1);
	object_active_.push_back(true);

	KZEngine::Vertex v1, v2, v3, v4, v5, v6, v7, v8;
	v1.pos.Set(1, 1, 1);
	v2.pos.Set(1, -1, 1);
	v3.pos.Set(-1, -1, 1);
	v4.pos.Set(-1, 1, 1);
	v5.pos.Set(1, 1, -1);
	v6.pos.Set(1, -1, -1);
	v7.pos.Set(-1, -1, -1);
	v8.pos.Set(-1, 1, -1);

	cube_obj.vlist_local_.push_back(v1);
	cube_obj.vlist_local_.push_back(v2);
	cube_obj.vlist_local_.push_back(v3);
	cube_obj.vlist_local_.push_back(v4);
	cube_obj.vlist_local_.push_back(v5);
	cube_obj.vlist_local_.push_back(v6);
	cube_obj.vlist_local_.push_back(v7);
	cube_obj.vlist_local_.push_back(v8);

	cube_obj.num_index_ = 36;
	cube_obj.index_.reserve(36);

	//front face
	cube_obj.index_.push_back(0);
	cube_obj.index_.push_back(2);
	cube_obj.index_.push_back(1);

	cube_obj.index_.push_back(0);
	cube_obj.index_.push_back(3);
	cube_obj.index_.push_back(2);

	//back face
	cube_obj.index_.push_back(7);
	cube_obj.index_.push_back(5);
	cube_obj.index_.push_back(6);

	cube_obj.index_.push_back(7);
	cube_obj.index_.push_back(4);
	cube_obj.index_.push_back(5);

	//left face
	cube_obj.index_.push_back(3);
	cube_obj.index_.push_back(6);
	cube_obj.index_.push_back(2);

	cube_obj.index_.push_back(3);
	cube_obj.index_.push_back(7);
	cube_obj.index_.push_back(6);

	//right face
	cube_obj.index_.push_back(4);
	cube_obj.index_.push_back(1);
	cube_obj.index_.push_back(5);

	cube_obj.index_.push_back(4);
	cube_obj.index_.push_back(0);
	cube_obj.index_.push_back(1);

	//top face
	cube_obj.index_.push_back(4);
	cube_obj.index_.push_back(3);
	cube_obj.index_.push_back(0);

	cube_obj.index_.push_back(4);
	cube_obj.index_.push_back(7);
	cube_obj.index_.push_back(3);

	//bottom face
	cube_obj.index_.push_back(1);
	cube_obj.index_.push_back(6);
	cube_obj.index_.push_back(5);

	cube_obj.index_.push_back(1);
	cube_obj.index_.push_back(2);
	cube_obj.index_.push_back(6);

	cube_obj.CalculateRadian();

	object_vec_.push_back(cube_obj);
}

//物体消除，包围球测试
void KZPipeLine::CullObject() {
	for (uint32_t i = 0; i < object_num_; ++i) {
		//根据远近平面裁剪
		if ((object_vec_[i].world_pos_.z_ - object_vec_[i].max_radius_ < main_camera_.far_clip_z_) || (object_vec_[i].world_pos_.z_ + object_vec_[i].max_radius_ > main_camera_.near_clip_z_)) {
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
				KZMath::KZVector4D vec2 = object_vec_[i].vlist_tran_[object_vec_[i].index_[j + 2]].pos - object_vec_[i].vlist_tran_[object_vec_[i].index_[j + 1]].pos;
				vec2.w_ = 0;
				KZMath::KZVector4D face_normal;
				vec1.Vector3Cross(face_normal, vec2);

				KZMath::KZVector4D observe_vec = main_camera_.pos_ - object_vec_[i].vlist_tran_[object_vec_[i].index_[j]].pos;
				if (face_normal.Vector3Dot(observe_vec) < 0) {
					continue;
				}
				else
				{
					KZEngine::Triangle tri;
					tri.vertex_list[0].pos = object_vec_[i].vlist_tran_[object_vec_[i].index_[j]].pos;
					tri.vertex_list[1].pos = object_vec_[i].vlist_tran_[object_vec_[i].index_[j + 1]].pos;
					tri.vertex_list[2].pos = object_vec_[i].vlist_tran_[object_vec_[i].index_[j + 2]].pos;
					render_list_->tri_list_.push_back(tri);
					++tri_num_;
				}
			}
		}
	}
	return;
}

//转化到世界坐标
void KZPipeLine::TransformModelToWorld() {
	for (uint32_t i = 0; i < object_num_; ++i) {
		object_vec_[i].TransformModelToWorldMath();
	}
}

//转化到透视坐标
void KZPipeLine::TransformWorldToPer(Projection projection) {
	KZMath::KZMatrix44 view, proj;
	main_camera_.GetViewMatrix(view);
	if (projection == PERSPECTIVE) {
		main_camera_.GetPerspectiveMatrix(proj);
	}
	else if(projection == ORTHOGONAL)
	{
		main_camera_.GetOrthogonalMatrix(proj);
	}

	for (uint32_t i = 0; i < tri_num_; ++i) {
		for (uint32_t j = 0; j < 3; ++j) {
			render_list_->tri_list_[i].vertex_list[j].pos = proj * view * render_list_->tri_list_[i].vertex_list[j].pos;
			float w_inverse = 1 / render_list_->tri_list_[i].vertex_list[j].pos.w_;
			render_list_->tri_list_[i].vertex_list[j].pos.x_ *= w_inverse;
			render_list_->tri_list_[i].vertex_list[j].pos.y_ *= w_inverse;
			render_list_->tri_list_[i].vertex_list[j].pos.z_ *= w_inverse;
		}
	}
}

//转化到视口坐标
void KZPipeLine::TransformPerToViewPort() {
	float alpha = 0.5f * view_width - 0.5f;
	float beta = 0.5f * view_height - 0.5f;
	for (uint32_t i = 0; i < tri_num_; ++i) {
		for (uint32_t j = 0; j < 3; ++j) {
			render_list_->tri_list_[i].vertex_list[j].pos.x_ = alpha + alpha * render_list_->tri_list_[i].vertex_list[j].pos.x_;
			render_list_->tri_list_[i].vertex_list[j].pos.y_ = beta - beta * render_list_->tri_list_[i].vertex_list[j].pos.y_;
		}
	}
}

//每帧更新
void KZPipeLine::FrameUpdate() {
	tri_num_ = 0;
	TransformModelToWorld();
	CullObject();
	RemoveBackface();
	TransformWorldToPer();
	TransformPerToViewPort();
	RasterizationDepthTest();
}

//初始化
void KZPipeLine::Initial() {
	if (render_list_ == NULL) {
		render_list_ = KZEngine::KZRenderList::GetInstance();
	}
	tri_num_ = 0;
	object_num_ = 0;
	view_height = 320;
	view_width = 480;
	z_buffer_ = new float*[view_width];
	for (uint32_t i = 0; i < view_width; ++i) {
		z_buffer_[i] = new float[view_height]();
		for (uint32_t j = 0; j < view_height; ++j) {
			z_buffer_[i][j] = -2.0f;
		}
	}
}

//光栅化
void KZPipeLine::RasterizationDepthTest() {
	for (uint32_t i = 0; i < tri_num_; ++i) {
 		KZMath::KZVector4D temp_v;
		KZMath::KZVector4D v0 = render_list_->tri_list_[i].vertex_list[0].pos;
		KZMath::KZVector4D v1 = render_list_->tri_list_[i].vertex_list[1].pos;
		KZMath::KZVector4D v2 = render_list_->tri_list_[i].vertex_list[2].pos;
		float dx_left = 0.0f, dx_right = 0.0f, dz_left = 0.0f, dz_right = 0.0f;
		uint32_t y_start, y_end, x_start, x_end;

		//按照y值升序v0,v1,v2
		if (v0.y_ > v1.y_) {
			temp_v = v0;
			v0 = v1;
			v1 = temp_v;
		}

		if (v1.y_ > v2.y_) {
			temp_v = v1;
			v1 = v2;
			v2 = temp_v;
			if (v0.y_ > v1.y_) {
				temp_v = v0;
				v0 = v1;
				v1 = temp_v;
			}
		}

		//检查是否平底三角形
		if (ceil(v1.y_) == ceil(v2.y_)) {
			DrawBottomTri(v0, v1, v2);
		}
		//检查是否平顶三角形
		else if (ceil(v0.y_) == ceil(v1.y_)) {
			DrawTopTri(v0, v1, v2);
		}
		//一般三角形先做分割
		else
		{
			float new_x = v0.x_ + (v1.y_ - v0.y_) * (v2.x_ - v0.x_) / (v2.y_ - v0.y_);
			float new_y = v1.y_;
			float new_z = v0.z_ + (v1.y_ - v0.y_) * (v2.z_ - v0.z_) / (v2.y_ - v0.y_);
			KZMath::KZVector4D new_v(new_x, new_y, new_z);
			DrawBottomTri(v0, v1, new_v);
			DrawTopTri(new_v, v1, v2);
		}

	}
}

//光栅化平底三角形
void KZPipeLine::DrawBottomTri(const KZMath::KZVector4D& v0, const KZMath::KZVector4D& v1, const KZMath::KZVector4D& v2) {
	float dx_left = 0.0f, dx_right = 0.0f, dz_left = 0.0f, dz_right = 0.0f;
	uint32_t y_start, y_end;
	float x_start = 0.0f, x_end = 0.0f;
	float z_left = 0.0f, z_right = 0.0f;
	float min_x = 0.0f, max_x = 0.0f;

	//计算左右两边斜率
	if (v2.x_ < v1.x_) {
		dx_left = (v0.x_ - v2.x_) / (v0.y_ - v2.y_);
		dx_right = (v0.x_ - v1.x_) / (v0.y_ - v1.y_);
		dz_left = (v0.z_ - v2.z_) / (v0.y_ - v2.y_);
		dz_right = (v0.z_ - v1.z_) / (v0.y_ - v1.y_);

		//计算x最大值最小值,修正误差
		if (v1.x_ < v0.x_) {
			min_x = v2.x_;
			max_x = v0.x_;
		}
		else
		{
			max_x = v1.x_;
			if (v0.x_ < v2.x_) {
				min_x = v0.x_;
			}
			else
			{
				min_x = v2.x_;
			}
		}
	}
	else
	{
		dx_left = (v0.x_ - v1.x_) / (v0.y_ - v1.y_);
		dx_right = (v0.x_ - v2.x_) / (v0.y_ - v2.y_);
		dz_left = (v0.z_ - v1.z_) / (v0.y_ - v1.y_);
		dz_right = (v0.z_ - v2.z_) / (v0.y_ - v2.y_);

		//计算x最大值最小值,修正误差
		if (v2.x_ < v0.x_) {
			min_x = v1.x_;
			max_x = v0.x_;
		}
		else
		{
			max_x = v2.x_;
			if (v0.x_ < v1.x_) {
				min_x = v0.x_;
			}
			else
			{
				min_x = v1.x_;
			}
		}
	}

	//垂直裁剪
	if (v0.y_ < 0) {
		y_start = 0;
	}
	else
	{
		y_start = ceil(v0.y_);
	}

	if (v2.y_ > view_height) {
		y_end = view_height;
	}
	else
	{
		y_end = ceil(v2.y_);
	}
	z_left = (y_start - v0.y_) * dz_left + v0.z_;
	z_right = (y_start - v0.y_) * dz_right + v0.z_;
	x_start = (y_start - v0.y_) * dx_left + v0.x_;
	x_end = (y_start - v0.y_) * dx_right + v0.x_;

	for (uint32_t j = y_start; j <= y_end; ++j) {
		float delta_zx = 0.0f;
		if (x_end - x_start != 0) {
			delta_zx = (z_right - z_left) / (x_end - x_start);
		}
		float z_cur = z_left;
		//水平裁剪
		min_x = min_x < 0 ? 0 : min_x;
		max_x = max_x > view_width - 1 ? view_width - 1 : max_x;
		x_start = x_start < min_x ? min_x : x_start;
		x_end = x_end > max_x ? max_x : x_end;
		uint32_t xs = (uint32_t)(x_start + 0.5f);
		uint32_t xe = (uint32_t)(x_end + 0.5f);
		for (uint32_t k = xs; k <= xe; ++k) {
			//深度缓冲检测
			if (z_cur > z_buffer_[k][j]) {
				z_buffer_[k][j] = z_cur;
				//draw point
			}
			z_cur += delta_zx;
		}
		x_start += dx_left;
		x_end += dx_right;
		z_left += dz_left;
		z_right += dz_right;
	}
}

//光栅化平顶三角形
void KZPipeLine::DrawTopTri(const KZMath::KZVector4D& v0, const KZMath::KZVector4D& v1, const KZMath::KZVector4D& v2) {
	float dx_left = 0.0f, dx_right = 0.0f, dz_left = 0.0f, dz_right = 0.0f;
	uint32_t y_start, y_end;
	float x_start = 0.0f, x_end = 0.0f;
	float z_left = 0.0f, z_right = 0.0f;
	float min_x = 0.0f, max_x = 0.0f;
	//计算左右两边斜率
	if (v0.x_ < v1.x_) {
		dx_left = - (v2.x_ - v0.x_) / (v2.y_ - v0.y_);
		dx_right = - (v2.x_ - v1.x_) / (v2.y_ - v1.y_);
		dz_left = - (v2.z_ - v0.z_) / (v2.y_ - v0.y_);
		dz_right = - (v2.z_ - v1.z_) / (v2.y_ - v1.y_);

		//计算x最大值最小值,修正误差
		if(v1.x_ < v2.x_) {
			min_x = v0.x_;
			max_x = v2.x_;
		}
		else
		{
			max_x = v1.x_;
			if (v0.x_ < v2.x_) {
				min_x = v0.x_;
			}
			else
			{
				min_x = v2.x_;
			}
		}
	}
	else
	{
		dx_left = - (v2.x_ - v1.x_) / (v2.y_ - v1.y_);
		dx_right = - (v2.x_ - v0.x_) / (v2.y_ - v0.y_);
		dz_left = - (v2.z_ - v1.z_) / (v2.y_ - v1.y_);
		dz_right = - (v2.z_ - v0.z_) / (v2.y_ - v0.y_);

		//计算x最大值最小值,修正误差
		if (v0.x_ < v2.x_) {
			min_x = v1.x_;
			max_x = v2.x_;
		}
		else
		{
			max_x = v0.x_;
			if (v1.x_ < v2.x_) {
				min_x = v1.x_;
			}
			else
			{
				min_x = v2.x_;
			}
		}
	}

	//垂直裁剪
	if (v0.y_ < 0) {
		y_end = 0;
	}
	else
	{
		y_end = ceil(v0.y_);
	}

	if (v2.y_ > view_height - 1) {
		y_start = view_height - 1;
	}
	else
	{
		y_start = ceil(v2.y_);
	}

	z_left = (y_start - v2.y_) * dz_left + v2.z_;
	z_right = (y_start - v2.y_) * dz_right + v2.z_;
	x_start = (y_start - v2.y_) * dx_left + v2.x_;
	x_end = (y_start - v2.y_) * dx_right + v2.x_;

	for (uint32_t j = y_start; j >= y_end; --j) {
		float delta_zx = 0.0f;
		if (x_end - x_start != 0) {
			delta_zx = (z_right - z_left) / (x_end - x_start);
		}
		float z_cur = z_left;
		//水平裁剪
		min_x = min_x < 0 ? 0 : min_x;
		max_x = max_x > view_width - 1 ? view_width - 1 : max_x;
		x_start = x_start < min_x ? min_x : x_start;
		x_end = x_end > max_x ? max_x : x_end;
		uint32_t xs = (uint32_t)(x_start + 0.5f);
		uint32_t xe = (uint32_t)(x_end + 0.5f);
		for (uint32_t k = xs; k <= xe; ++k) {
			//深度缓冲检测
			if (z_cur >= z_buffer_[k][j]) {
				z_buffer_[k][j] = z_cur;
				//draw point
			}
			z_cur += delta_zx;
		}
		x_start += dx_left;
		x_end += dx_right;
		z_left += dz_left;
		z_right += dz_right;
	}
}
