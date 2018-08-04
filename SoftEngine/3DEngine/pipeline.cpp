#include"pipeline.h"
using namespace KZEngine;

//构造函数
KZPipeLine::KZPipeLine() {
	render_list_ = NULL;
	main_camera_.GetCameraPos().Set(1.5f, 1.5f, 3);
	Initial();
};

//从文件中加载
void KZPipeLine::LoadFromFile() {
	//to do
}

//创建cube
void KZPipeLine::CreateCube() {
	++object_num_;
	KZObject cube_obj;
	cube_obj.id_ = 1;
	cube_obj.name_ = "cube";
	cube_obj.world_pos_ = KZMath::KZVector4D(0, 0, -5);
	cube_obj.ux_ = KZMath::KZVector4D(1, 0, 0, 0);
	cube_obj.uy_ = KZMath::KZVector4D(0, 1, 0, 0);
	cube_obj.uz_ = KZMath::KZVector4D(0, 0, 1, 0);
	cube_obj.num_vertices_ = 8;
	cube_obj.vlist_local_.reserve(cube_obj.num_vertices_);
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

	v1.color.Set(255, 0, 0);
	v2.color.Set(0, 255, 0);
	v3.color.Set(0, 0, 255);
	v4.color.Set(255, 255, 0);
	v5.color.Set(255, 0, 255);
	v6.color.Set(0, 255, 255);
	v7.color.Set(255, 255, 255);
	v8.color.Set(128, 128, 128);

	v1.uv.x_ = 1.0f;
	v1.uv.y_ = 0.0f;
	v2.uv.x_ = 1.0f;
	v2.uv.y_ = 1.0f;
	v3.uv.x_ = 0.0f;
	v3.uv.y_ = 1.0f;
	v4.uv.x_ = 0.0f;
	v4.uv.y_ = 0.0f;

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
	cube_obj.CalculateNormal();

	cube_obj.mat_id_.resize(12, -1);
	cube_obj.mat_id_[0] = 0;
	cube_obj.mat_id_[1] = 0;
	string texture_path = "container.jpg";
	static KZEngine::KZMaterial cube_mat(Color(255, 255, 255), texture_path, 1.0, 0.0, 0.0, 0.0);
	cube_mat.id_ = 1;
	cube_mat.has_texture_ = true;
	AddMaterial(cube_mat);
	object_vec_.push_back(cube_obj);
}

void KZPipeLine::CreatePyramid() {
	++object_num_;
	KZObject pyramid;
	pyramid.id_ = 2;
	pyramid.name_ = "pyramid";
	pyramid.world_pos_ = KZMath::KZVector4D(2, 0, -7);
	pyramid.ux_ = KZMath::KZVector4D(1, 0, 0, 0);
	pyramid.uy_ = KZMath::KZVector4D(0, 1, 0, 0);
	pyramid.uz_ = KZMath::KZVector4D(0, 0, 1, 0);
	pyramid.num_vertices_ = 4;
	pyramid.vlist_local_.reserve(pyramid.num_vertices_);
	object_active_.push_back(true);

	KZEngine::Vertex v1, v2, v3, v4;
	v1.pos.Set(-1, 0, 0);
	v2.pos.Set(1, 0, 0);
	v3.pos.Set(0, 0, 2);
	v4.pos.Set(0, 2, 1);

	v1.color.Set(255, 0, 0);
	v2.color.Set(0, 255, 0);
	v3.color.Set(0, 0, 255);
	v4.color.Set(255, 255, 0);

	pyramid.vlist_local_.push_back(v1);
	pyramid.vlist_local_.push_back(v2);
	pyramid.vlist_local_.push_back(v3);
	pyramid.vlist_local_.push_back(v4);

	pyramid.num_index_ = 12;
	pyramid.index_.reserve(12);

	//face 1
	pyramid.index_.push_back(3);
	pyramid.index_.push_back(0);
	pyramid.index_.push_back(2);

	//face 2
	pyramid.index_.push_back(3);
	pyramid.index_.push_back(2);
	pyramid.index_.push_back(1);

	//face 3
	pyramid.index_.push_back(3);
	pyramid.index_.push_back(1);
	pyramid.index_.push_back(0);

	//face 4
	pyramid.index_.push_back(1);
	pyramid.index_.push_back(2);
	pyramid.index_.push_back(0);

	pyramid.CalculateRadian();
	pyramid.CalculateNormal();

	pyramid.mat_id_.resize(4, -1);
	object_vec_.push_back(pyramid);
}

//物体消除，包围球测试
void KZPipeLine::CullObject() {
	for (uint32_t i = 0; i < object_num_; ++i) {
		//根据远近平面裁剪
		if ((object_vec_[i].world_pos_.z_ - object_vec_[i].max_radius_ < main_camera_.GetCameraFarClip()) || (object_vec_[i].world_pos_.z_ + object_vec_[i].max_radius_ > main_camera_.GetCameraNearClip())) {
			object_active_[i] = false;
			continue;
		}
		//根据左右平面裁剪
		float w_test = main_camera_.GetViewRight() * object_vec_[i].world_pos_.z_ / main_camera_.GetCameraNearClip();
		if ((object_vec_[i].world_pos_.x_ + object_vec_[i].max_radius_ > w_test) || (object_vec_[i].world_pos_.x_ - object_vec_[i].max_radius_ < -w_test)) {
			object_active_[i] = false;
			continue;
		}

		//根据上下平面裁剪
		float h_test = main_camera_.GetViewTop() * object_vec_[i].world_pos_.z_ / main_camera_.GetCameraNearClip();
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
				KZMath::KZVector4D observe_vec = main_camera_.GetCameraPos() - object_vec_[i].vlist_tran_[object_vec_[i].index_[j]].pos;
				if (object_vec_[i].face_normal_[j / 3].Vector3Dot(observe_vec) < 0) {
					continue;
				}
				else
				{
					KZEngine::Triangle tri;
					if (object_vec_[i].is_light_) {
						uint32_t light_num = light_vec.size();
						for (uint32_t k = 0; k < light_num; ++k) {
							object_vec_[i].vlist_tran_[object_vec_[i].index_[j]].color = mat_vec_[object_vec_[i].mat_id_[j / 3]].CalculateFinalColor(light_vec[i], object_vec_[i].vlist_tran_[object_vec_[i].index_[j]].pos, object_vec_[i].vlist_tran_[object_vec_[i].index_[j]].normal);
							object_vec_[i].vlist_tran_[object_vec_[i].index_[j + 1]].color = mat_vec_[object_vec_[i].mat_id_[j / 3]].CalculateFinalColor(light_vec[i], object_vec_[i].vlist_tran_[object_vec_[i].index_[j + 1]].pos, object_vec_[i].vlist_tran_[object_vec_[i].index_[j + 1]].normal);
							object_vec_[i].vlist_tran_[object_vec_[i].index_[j + 2]].color = mat_vec_[object_vec_[i].mat_id_[j / 3]].CalculateFinalColor(light_vec[i], object_vec_[i].vlist_tran_[object_vec_[i].index_[j + 2]].pos, object_vec_[i].vlist_tran_[object_vec_[i].index_[j + 2]].normal);
						}
					}
					tri.vertex_list[0] = object_vec_[i].vlist_tran_[object_vec_[i].index_[j]];
					tri.vertex_list[1] = object_vec_[i].vlist_tran_[object_vec_[i].index_[j + 1]];
					tri.vertex_list[2] = object_vec_[i].vlist_tran_[object_vec_[i].index_[j + 2]];
					tri.material = object_vec_[i].mat_id_[j/3];
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
	DWORD start = ::GetTickCount();
	////初始化
	tri_num_ = 0;
	for (uint32_t i = 0; i < object_num_; ++i) {
		object_active_[i] = true;
	}
	for (uint32_t i = 0; i < view_width; ++i) {
		for (uint32_t j = 0; j < view_height; ++j) {
			z_buffer_[i][j] = 2.0f;
		}
	}
	for (uint32_t i = 0; i < view_width * view_height * 3; ++i) {
		frame_buffer_[i] = 0;
	}
	vector<Triangle>().swap(render_list_->tri_list_);

	//固定管线
	TransformModelToWorld();
	CullObject();
	RemoveBackface();
	TransformWorldToPer();
	TransformPerToViewPort();
	RasterizationDepthTest();

	DWORD delta_time = ::GetTickCount() - start;
	if (delta_time != 0) {
		float cur_fps = 1000.0f / delta_time;
		if (fps_ != 0) {
			fps_ += cur_fps;
			fps_ *= 0.5;
		}
		else
		{
			fps_ = cur_fps;
		}
	}
}

//初始化
void KZPipeLine::Initial() {
	if (!render_list_) {
		render_list_ = KZEngine::KZRenderList::GetInstance();
	}
	tri_num_ = 0;
	object_num_ = 0;
	view_height = 600;
	view_width = 800;
	z_buffer_ = new float*[view_width];
	object_vec_.reserve(8);
	object_active_.reserve(8);
	for (uint32_t i = 0; i < view_width; ++i) {
		z_buffer_[i] = new float[view_height]();
		for (uint32_t j = 0; j < view_height; ++j) {
			z_buffer_[i][j] = 2.0f;
		}
	}
	frame_buffer_ = new unsigned char[view_width * view_height * 3];
	for (uint32_t i = 0; i < view_width * view_height * 3; ++i) {
		frame_buffer_[i] = 0;
	}
	record_time_ = ::GetTickCount();
}

//光栅化
void KZPipeLine::RasterizationDepthTest() {
	for (uint32_t i = 0; i < tri_num_; ++i) {
		Vertex temp_v;
		Vertex v0 = render_list_->tri_list_[i].vertex_list[0];
		Vertex v1 = render_list_->tri_list_[i].vertex_list[1];
		Vertex v2 = render_list_->tri_list_[i].vertex_list[2];
		float dx_left = 0.0f, dx_right = 0.0f, dz_left = 0.0f, dz_right = 0.0f;

		//按照y值升序v0,v1,v2
		if (v0.pos.y_ > v1.pos.y_) {
			temp_v = v0;
			v0 = v1;
			v1 = temp_v;
		}

		if (v1.pos.y_ > v2.pos.y_) {
			temp_v = v1;
			v1 = v2;
			v2 = temp_v;
			if (v0.pos.y_ > v1.pos.y_) {
				temp_v = v0;
				v0 = v1;
				v1 = temp_v;
			}
		}

		bool has_texture = false;
		if (render_list_->tri_list_[i].material != -1 && mat_vec_[render_list_->tri_list_[i].material].has_texture_) {
			has_texture = true;
		}

		//检查是否平底三角形
		if (ceil(v1.pos.y_) == ceil(v2.pos.y_)) {
			DrawBottomTri(v0, v1, v2, has_texture, render_list_->tri_list_[i].material);
		}
		//检查是否平顶三角形
		else if (ceil(v0.pos.y_) == ceil(v1.pos.y_)) {
			DrawTopTri(v0, v1, v2, has_texture, render_list_->tri_list_[i].material);
		}
		//一般三角形先做分割
		else
		{
			float new_x = v0.pos.x_ + (v1.pos.y_ - v0.pos.y_) * (v2.pos.x_ - v0.pos.x_) / (v2.pos.y_ - v0.pos.y_);
			float new_y = v1.pos.y_;
			float new_z = v0.pos.z_ + (v1.pos.y_ - v0.pos.y_) * (v2.pos.z_ - v0.pos.z_) / (v2.pos.y_ - v0.pos.y_);
			float new_r = v0.color.r_ + (v1.pos.y_ - v0.pos.y_) * (v2.color.r_ - v0.color.r_) / (v2.pos.y_ - v0.pos.y_);
			float new_g = v0.color.g_ + (v1.pos.y_ - v0.pos.y_) * (v2.color.g_ - v0.color.g_) / (v2.pos.y_ - v0.pos.y_);
			float new_b = v0.color.b_ + (v1.pos.y_ - v0.pos.y_) * (v2.color.b_ - v0.color.b_) / (v2.pos.y_ - v0.pos.y_);
			KZMath::KZVector4D new_v(new_x, new_y, new_z);
			Vertex new_vertex;
			new_vertex.pos.Set(new_x, new_y, new_z);
			new_vertex.color.Set(new_r, new_g, new_b);
			DrawBottomTri(v0, v1, new_vertex, has_texture, render_list_->tri_list_[i].material);
			DrawTopTri(new_vertex, v1, v2, has_texture, render_list_->tri_list_[i].material);
		}

	}
}

//光栅化平底三角形
void KZPipeLine::DrawBottomTri(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id) {
	float dx_left = 0.0f, dx_right = 0.0f, dz_left = 0.0f, dz_right = 0.0f;
	float dr_left = 0.0f, dr_right = 0.0f, dg_left = 0.0f, dg_right = 0.0f, db_left = 0.0f, db_right = 0.0f;
	float ds_left = 0.0f, ds_right = 0.0f, dt_left = 0.0f, dt_right = 0.0f;
	uint32_t y_start, y_end;
	float x_start = 0.0f, x_end = 0.0f;
	float z_left = 0.0f, z_right = 0.0f;
	float r_left = 0.0f, r_right = 0.0f;
	float g_left = 0.0f, g_right = 0.0f;
	float b_left = 0.0f, b_right = 0.0f;
	float s_left = 0.0f, s_right = 0.0f, t_left = 0.0f, t_right = 0.0f;
	float min_x = 0.0f, max_x = 0.0f;
	
	float v0_s_overz = 0.0f, v0_t_overz = 0.0f;

	//检查是否贴图，避免不必要计算
	if (has_texture) {
		v0_s_overz = v0.uv.x_ * v0.pos.z_;
		v0_t_overz = v0.uv.y_ * v0.pos.z_;
	}

	//计算左右两边斜率
	if (v2.pos.x_ < v1.pos.x_) {
		float one_over_y_diff_left = 1 / (v0.pos.y_ - v2.pos.y_);
		float one_over_y_diff_right = 1 / (v0.pos.y_ - v1.pos.y_);
		//求x,z的y的左右梯度
		dx_left = (v0.pos.x_ - v2.pos.x_) * one_over_y_diff_left;
		dx_right = (v0.pos.x_ - v1.pos.x_) * one_over_y_diff_right;
		dz_left = (v0.pos.z_ - v2.pos.z_) * one_over_y_diff_left;
		dz_right = (v0.pos.z_ - v1.pos.z_) * one_over_y_diff_right;
		
		//求颜色r,g,b的y的左右梯度
		dr_left = (v0.color.r_ - v2.color.r_) * one_over_y_diff_left;
		dr_right = (v0.color.r_ - v1.color.r_) * one_over_y_diff_right;
		dg_left = (v0.color.g_ - v2.color.g_) * one_over_y_diff_left;
		dg_right = (v0.color.g_ - v1.color.g_) * one_over_y_diff_right;
		db_left = (v0.color.b_ - v2.color.b_) * one_over_y_diff_left;
		db_right = (v0.color.b_ - v1.color.b_) * one_over_y_diff_right;

		//检查是否贴图，避免不必要计算
		if (has_texture) {
			//求纹理s/z,t/z的y的左右梯度
			ds_left = (v0_s_overz - v2.uv.x_ * v2.pos.z_) * one_over_y_diff_left;
			ds_right = (v0_s_overz - v1.uv.x_ * v1.pos.z_) * one_over_y_diff_right;
			dt_left = (v0_t_overz - v2.uv.y_ * v2.pos.z_) * one_over_y_diff_left;
			dt_right = (v0_t_overz - v1.uv.y_ * v1.pos.z_) * one_over_y_diff_right;
		}

		//计算x最大值最小值,修正误差
		if (v1.pos.x_ < v0.pos.x_) {
			min_x = v2.pos.x_;
			max_x = v0.pos.x_;
		}
		else
		{
			max_x = v1.pos.x_;
			if (v0.pos.x_ < v2.pos.x_) {
				min_x = v0.pos.x_;
			}
			else
			{
				min_x = v2.pos.x_;
			}
		}
	}
	else
	{
		float one_over_y_diff_left = 1 / (v0.pos.y_ - v1.pos.y_);
		float one_over_y_diff_right = 1 / (v0.pos.y_ - v2.pos.y_);
		//求x,z的y的左右梯度
		dx_left = (v0.pos.x_ - v1.pos.x_) * one_over_y_diff_left;
		dx_right = (v0.pos.x_ - v2.pos.x_) * one_over_y_diff_right;
		dz_left = (v0.pos.z_ - v1.pos.z_) * one_over_y_diff_left;
		dz_right = (v0.pos.z_ - v2.pos.z_) * one_over_y_diff_right;
		
		//求颜色r,g,b的y的左右梯度
		dr_left = (v0.color.r_ - v1.color.r_) * one_over_y_diff_left;
		dr_right = (v0.color.r_ - v2.color.r_) * one_over_y_diff_right;
		dg_left = (v0.color.g_ - v1.color.g_) * one_over_y_diff_left;
		dg_right = (v0.color.g_ - v2.color.g_) * one_over_y_diff_right;
		db_left = (v0.color.b_ - v1.color.b_) * one_over_y_diff_left;
		db_right = (v0.color.b_ - v2.color.b_) * one_over_y_diff_right;

		//检查是否贴图，避免不必要计算
		if (has_texture) {
			//求纹理s/z,t/z的y的左右梯度
			ds_left = (v0_s_overz - v1.uv.x_ * v1.pos.z_) * one_over_y_diff_left;
			ds_right = (v0_s_overz - v2.uv.x_ * v2.pos.z_) * one_over_y_diff_right;
			dt_left = (v0_t_overz - v1.uv.y_ * v1.pos.z_) * one_over_y_diff_left;
			dt_right = (v0_t_overz - v2.uv.y_ * v2.pos.z_) * one_over_y_diff_right;
		}


		//计算x最大值最小值,修正误差
		if (v2.pos.x_ < v0.pos.x_) {
			min_x = v1.pos.x_;
			max_x = v0.pos.x_;
		}
		else
		{
			max_x = v2.pos.x_;
			if (v0.pos.x_ < v1.pos.x_) {
				min_x = v0.pos.x_;
			}
			else
			{
				min_x = v1.pos.x_;
			}
		}
	}

	if (max_x < 0) {
		return;
	}
	//计算最大和最小x限制范围
	min_x = min_x < 0 ? 0 : min_x > view_width - 1 ? view_width - 1 : min_x;
	max_x = max_x > view_width - 1 ? view_width - 1 : max_x < 0 ? 0 : max_x;

	//垂直裁剪
	y_start = v0.pos.y_ < 0 ? 0 : v0.pos.y_ < view_height - 1 ? static_cast<uint32_t>(v0.pos.y_ + 0.5f) : view_height - 1;
	y_end = v2.pos.y_ > view_height - 1 ? view_height - 1 : v2.pos.y_ < 0 ? 0 : static_cast<uint32_t>(v2.pos.y_ + 0.5f);

	float y_error = (y_start - v0.pos.y_);
	//根据y值误差修正z,x,r,g,b,s,t
	z_left = y_error * dz_left + v0.pos.z_;
	z_right = y_error * dz_right + v0.pos.z_;
	x_start = y_error * dx_left + v0.pos.x_;
	x_end = y_error * dx_right + v0.pos.x_;

	r_left = y_error * dr_left + v0.color.r_;
	r_right = y_error * dr_right + v0.color.r_;
	g_left = y_error * dg_left + v0.color.g_;
	g_right = y_error * dg_right + v0.color.g_;
	b_left = y_error * db_left + v0.color.b_;
	b_right = y_error * db_right + v0.color.b_;

	//检查是否贴图，避免不必要计算
	if (has_texture) {
		s_left = y_error * ds_left + v0_s_overz;
		s_right = y_error * ds_right + v0_s_overz;
		t_left = y_error * dt_left + v0_t_overz;
		t_right = y_error * dt_right + v0_t_overz;
	}

	for (uint32_t j = y_start; j <= y_end; ++j) {
		float delta_zx = 0.0f;
		float delta_rx = 0.0f;
		float delta_gx = 0.0f;
		float delta_bx = 0.0f;

		float delta_sx = 0.0f;
		float delta_tx = 0.0f;

		if (x_end - x_start != 0) {
			//计算z,r,g,b,s,t相对于x的误差
			float coeff = 1.0f / (x_end - x_start);
			delta_zx = (z_right - z_left) * coeff ;
			delta_rx = (r_right - r_left) * coeff;
			delta_gx = (g_right - g_left) * coeff;
			delta_bx = (b_right - b_left) * coeff;

			//检查是否贴图，避免不必要计算
			if (has_texture) {
				delta_sx = (s_right - s_left) * coeff;
				delta_tx = (t_right - t_left) * coeff;
			}
			
		}
		//水平裁剪
		float fix_x_start = x_start < min_x ? min_x : x_start;
		//根据x值误差修正z,r,g,b,s,t
		float z_cur = z_left + (fix_x_start - x_start) * delta_zx;
		float r_cur = r_left + (fix_x_start - x_start) * delta_rx;
		float g_cur = g_left + (fix_x_start - x_start) * delta_gx;
		float b_cur = b_left + (fix_x_start - x_start) * delta_bx;

		float s_cur;
		float t_cur;

		//检查是否贴图，避免不必要计算
		if (has_texture) {
			s_cur = s_left + (fix_x_start - x_start) * delta_sx;
			t_cur = t_left + (fix_x_start - x_start) * delta_tx;
		}

		float fix_x_end = x_end > max_x ? max_x : x_end;
		//扫描线终点小于0，直接跳过
		if (fix_x_end < 0) {
			x_start += dx_left;
			x_end += dx_right;
			z_left += dz_left;
			z_right += dz_right;
			r_left += dr_left;
			r_right += dr_right;
			g_left += dg_left;
			g_right += dg_right;
			b_left += db_left;
			b_right += db_right;

			//检查是否贴图，避免不必要计算
			if (has_texture) {
				s_left += ds_left;
				s_right += ds_right;
				t_left += dt_left;
				t_right += dt_right;
			}
			continue;
		}
		uint32_t xs = static_cast<uint32_t>(fix_x_start + 0.5f);
		uint32_t xe = static_cast<uint32_t>(fix_x_end + 0.5f);

		for (uint32_t k = xs; k <= xe; ++k) {
			//CVV裁剪z
			if (z_cur >= -1 && z_cur <= 1) {
				//深度缓冲
				if (z_cur <= z_buffer_[k][j]) {
					z_buffer_[k][j] = z_cur;
					Color final_color(static_cast<unsigned char>(r_cur), static_cast<unsigned char>(g_cur), static_cast<unsigned char>(b_cur));
					if (has_texture) {
						float one_over_z_cur = 1 / z_cur;
						float s_cord = s_cur * one_over_z_cur;
						float t_cord = t_cur * one_over_z_cur;
						s_cord = s_cord < 0 ? 0 : s_cord > 1 ? 1 : s_cord;
						t_cord = t_cord < 0 ? 0 : t_cord > 1 ? 1 : t_cord;
						final_color = final_color * mat_vec_[mat_id].GetTextureColor(s_cord, t_cord);
					}
					//draw point
					uint32_t dist = j * view_width * 3 + k * 3;
					frame_buffer_[dist] = final_color.b_;//Blue
					frame_buffer_[dist + 1] = final_color.g_;//Green
					frame_buffer_[dist + 2] = final_color.r_;//Red 
				}
			}
			z_cur += delta_zx;
			//限制颜色范围
			r_cur += delta_rx;
			r_cur = r_cur < 0 ? 0 : r_cur > 255 ? 255 : r_cur;
			g_cur += delta_gx;
			g_cur = g_cur < 0 ? 0 : g_cur > 255 ? 255 : g_cur;
			b_cur += delta_bx;
			b_cur = b_cur < 0 ? 0 : b_cur > 255 ? 255 : b_cur;

			//检查是否贴图，避免不必要计算
			if (has_texture) {
				//限制纹理坐标范围
				s_cur += delta_sx;
				s_cur = s_cur < 0 ? 0 : s_cur > 1 ? 1 : s_cur;
				t_cur += delta_tx;
				t_cur = t_cur < 0 ? 0 : t_cur > 1 ? 1 : t_cur;
			}
		}
		x_start += dx_left;
		x_end += dx_right;
		z_left += dz_left;
		z_right += dz_right;
		r_left += dr_left;
		r_right += dr_right;
		g_left += dg_left;
		g_right += dg_right;
		b_left += db_left;
		b_right += db_right;

		//检查是否贴图，避免不必要计算
		if (has_texture) {
			s_left += ds_left;
			s_right += ds_right;
			t_left += dt_left;
			t_right += dt_right;
		}
	}
}

//光栅化平顶三角形
void KZPipeLine::DrawTopTri(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id) {
	float dx_left = 0.0f, dx_right = 0.0f, dz_left = 0.0f, dz_right = 0.0f;
	float dr_left = 0.0f, dr_right = 0.0, dg_left = 0.0f, dg_right = 0.0f, db_left = 0.0f, db_right = 0.0f;
	float ds_left = 0.0f, ds_right = 0.0f, dt_left = 0.0f, dt_right = 0.0f;
	uint32_t y_start, y_end;
	float x_start = 0.0f, x_end = 0.0f;
	float z_left = 0.0f, z_right = 0.0f;
	float r_left = 0.0f, r_right = 0.0f;
	float g_left = 0.0f, g_right = 0.0f;
	float b_left = 0.0f, b_right = 0.0f;
	float s_left = 0.0f, s_right = 0.0f, t_left = 0.0f, t_right = 0.0f;

	float min_x = 0.0f, max_x = 0.0f;
	float v2_s_overz = 0.0f, v2_t_overz = 0.0f;
	//检查是否贴图，避免不必要计算
	if (has_texture) {
		v2_s_overz = v2.uv.x_ * v2.pos.z_;
		v2_t_overz = v2.uv.y_ * v2.pos.z_;
	}
	//计算左右两边斜率
	if (v0.pos.x_ < v1.pos.x_) {
		float one_over_y_diff_left = 1 / (v2.pos.y_ - v0.pos.y_);
		float one_over_y_diff_right = 1 / (v2.pos.y_ - v1.pos.y_);
		//求x,z的y的左右梯度
		dx_left = - (v2.pos.x_ - v0.pos.x_) * one_over_y_diff_left;
		dx_right = - (v2.pos.x_ - v1.pos.x_) * one_over_y_diff_right;
		dz_left = - (v2.pos.z_ - v0.pos.z_) * one_over_y_diff_left;
		dz_right = - (v2.pos.z_ - v1.pos.z_) * one_over_y_diff_right;

		//求颜色r,g,b的y的左右梯度
		dr_left = - (v2.color.r_ - v0.color.r_) * one_over_y_diff_left;
		dr_right = - (v2.color.r_ - v1.color.r_) * one_over_y_diff_right;
		dg_left = - (v2.color.g_ - v0.color.g_) * one_over_y_diff_left;
		dg_right = - (v2.color.g_ - v1.color.g_) * one_over_y_diff_right;
		db_left = - (v2.color.b_ - v0.color.b_) * one_over_y_diff_left;
		db_right = - (v2.color.b_ - v1.color.b_) * one_over_y_diff_right;

		//检查是否贴图，避免不必要计算
		if (has_texture) {
			//求纹理s/z,t/z的y的左右梯度
			ds_left = - (v2_s_overz - v0.uv.x_ * v0.pos.z_) * one_over_y_diff_left;
			ds_right = - (v2_s_overz - v1.uv.x_ * v1.pos.z_) * one_over_y_diff_right;
			dt_left = - (v2_t_overz - v0.uv.y_ * v0.pos.z_) * one_over_y_diff_left;
			dt_right = - (v2_t_overz - v1.uv.y_ * v1.pos.z_) * one_over_y_diff_right;
		}
		
		//计算x最大值最小值,修正误差
		if(v1.pos.x_ < v2.pos.x_) {
			min_x = v0.pos.x_;
			max_x = v2.pos.x_;
		}
		else
		{
			max_x = v1.pos.x_;
			if (v0.pos.x_ < v2.pos.x_) {
				min_x = v0.pos.x_;
			}
			else
			{
				min_x = v2.pos.x_;
			}
		}
	}
	else
	{
		float one_over_y_diff_left = 1 / (v2.pos.y_ - v1.pos.y_);
		float one_over_y_diff_right = 1 / (v2.pos.y_ - v0.pos.y_);
		//求x,z的y的左右梯度
		dx_left = - (v2.pos.x_ - v1.pos.x_) * one_over_y_diff_left;
		dx_right = - (v2.pos.x_ - v0.pos.x_) * one_over_y_diff_right;
		dz_left = - (v2.pos.z_ - v1.pos.z_) * one_over_y_diff_left;
		dz_right = - (v2.pos.z_ - v0.pos.z_) * one_over_y_diff_right;
		
		//求颜色r,g,b的y的左右梯度
		dr_left = -(v2.color.r_ - v1.color.r_) * one_over_y_diff_left;
		dr_right = -(v2.color.r_ - v0.color.r_) * one_over_y_diff_right;
		dg_left = -(v2.color.g_ - v1.color.g_) * one_over_y_diff_left;
		dg_right = -(v2.color.g_ - v0.color.g_) * one_over_y_diff_right;
		db_left = -(v2.color.b_ - v1.color.b_) * one_over_y_diff_left;
		db_right = -(v2.color.b_ - v0.color.b_) * one_over_y_diff_right;

		//检查是否贴图，避免不必要计算
		if (has_texture) {
			//求纹理s/z,t/z的y的左右梯度
			ds_left = - (v2_s_overz - v1.uv.x_ * v1.pos.z_) * one_over_y_diff_left;
			ds_right = - (v2_s_overz - v0.uv.x_ * v0.pos.z_) * one_over_y_diff_right;
			dt_left = - (v2_t_overz - v1.uv.y_ * v1.pos.z_) * one_over_y_diff_left;
			dt_right = - (v2_t_overz - v0.uv.y_ * v0.pos.z_) * one_over_y_diff_right;
		}
		
		//计算x最大值最小值,修正误差
		if (v0.pos.x_ < v2.pos.x_) {
			min_x = v1.pos.x_;
			max_x = v2.pos.x_;
		}
		else
		{
			max_x = v0.pos.x_;
			if (v1.pos.x_ < v2.pos.x_) {
				min_x = v1.pos.x_;
			}
			else
			{
				min_x = v2.pos.x_;
			}
		}
	}

	if (max_x < 0) {
		return;
	}

	//计算最大和最小x限制范围
	min_x = min_x < 0 ? 0 : min_x > view_width - 1 ? view_width - 1 : min_x;
	max_x = max_x > view_width - 1 ? view_width - 1 : max_x < 0 ? 0 : max_x;

	//垂直裁剪
	y_end = v0.pos.y_ < 0 ? 0 : v0.pos.y_ < view_height - 1 ? static_cast<uint32_t>(v0.pos.y_ + 0.5f) : view_height - 1;
	y_start = v2.pos.y_ > view_height - 1 ? view_height - 1 : v2.pos.y_ < 0 ? 0 : static_cast<uint32_t>(v2.pos.y_ + 0.5f);

	float y_error = (v2.pos.y_ - y_start);
	//根据y值误差修正z,x,r,g,b,s,t
	z_left = y_error * dz_left + v2.pos.z_;
	z_right = y_error * dz_right + v2.pos.z_;
	x_start = y_error * dx_left + v2.pos.x_;
	x_end = y_error * dx_right + v2.pos.x_;

	r_left = y_error * dr_left + v2.color.r_;
	r_right = y_error * dr_right + v2.color.r_;
	g_left = y_error * dg_left + v2.color.g_;
	g_right = y_error * dg_right + v2.color.g_;
	b_left = y_error * db_left + v2.color.b_;
	b_right = y_error * db_right + v2.color.b_;

	//检查是否贴图，避免不必要计算
	if (has_texture) {
		s_left = y_error * ds_left + v2_s_overz;
		s_right = y_error * ds_right + v2_s_overz;
		t_left = y_error * dt_left + v2_t_overz;
		t_right = y_error * dt_right + v2_t_overz;
	}

	for (uint32_t j = y_start; j > y_end; --j) {
		float delta_zx = 0.0f;
		float delta_rx = 0.0f;
		float delta_gx = 0.0f;
		float delta_bx = 0.0f;

		float delta_sx = 0.0f;
		float delta_tx = 0.0f;
		if (x_end - x_start != 0) {
			float coeff = 1.0f / (x_end - x_start);
			//计算z,r,g,b,s,t相对于x的梯度
			delta_zx = (z_right - z_left) * coeff;
			delta_rx = (r_right - r_left) * coeff;
			delta_gx = (g_right - g_left) * coeff;
			delta_bx = (b_right - b_left) * coeff;

			//检查是否贴图，避免不必要计算
			if (has_texture) {
				delta_sx = (s_right - s_left) * coeff;
				delta_tx = (t_right - t_left) * coeff;
			}
		}
		float z_cur = z_left;
		//水平裁剪
		float fix_x_start = x_start < min_x ? min_x : x_start;
		//根据x值误差修正z,r,g,b,s,t
		float r_cur = r_left + (fix_x_start - x_start) * delta_rx;
		float g_cur = g_left + (fix_x_start - x_start) * delta_gx;
		float b_cur = b_left + (fix_x_start - x_start) * delta_bx;
		
		float s_cur = 0.0f, t_cur = 0.0f;
		//检查是否贴图，避免不必要计算
		if (has_texture) {
			s_cur = s_left + (fix_x_start - x_start) * delta_sx;
			t_cur = t_left + (fix_x_start - x_start) * delta_tx;
		}

		float fix_x_end = x_end > max_x ? max_x : x_end;
		//扫描线终点小于0，直接跳过
		if(fix_x_end < 0) {
			x_start += dx_left;
			x_end += dx_right;
			z_left += dz_left;
			z_right += dz_right;
			r_left += dr_left;
			r_right += dr_right;
			g_left += dg_left;
			g_right += dg_right;
			b_left += db_left;
			b_right += db_right;

			//检查是否贴图，避免不必要计算
			if (has_texture) {
				s_left += ds_left;
				s_right += ds_right;
				t_left += dt_left;
				t_right += dt_right;
			}
			
			continue;
		}
		uint32_t xs = static_cast<uint32_t>(fix_x_start + 0.5f);
		uint32_t xe = static_cast<uint32_t>(fix_x_end + 0.5f);
		for (uint32_t k = xs; k <= xe; ++k) {
			//CVV裁剪z
			if (z_cur >= -1 && z_cur <= 1) {
				//深度缓冲检测
				if (z_cur <= z_buffer_[k][j]) {
					z_buffer_[k][j] = z_cur;
					Color final_color(static_cast<unsigned char>(r_cur), static_cast<unsigned char>(g_cur), static_cast<unsigned char>(b_cur));
					if (has_texture) {
						float one_over_z_cur = 1 / z_cur;
						float s_cord = s_cur * one_over_z_cur;
						float t_cord = t_cur * one_over_z_cur;
						s_cord = s_cord < 0 ? 0 : s_cord > 1 ? 1 : s_cord;
						t_cord = t_cord < 0 ? 0 : t_cord > 1 ? 1 : t_cord;
						final_color = final_color * mat_vec_[mat_id].GetTextureColor(s_cord, t_cord);
					}
					//draw point
					uint32_t dist = j * view_width * 3 + k * 3;
					//draw point
					frame_buffer_[dist] = final_color.b_;//Blue
					frame_buffer_[dist + 1] = final_color.g_;//Green
					frame_buffer_[dist + 2] = final_color.r_;//Red
				}
				z_cur += delta_zx;
				//限制颜色范围
				r_cur += delta_rx;
				r_cur = r_cur < 0 ? 0 : r_cur > 255 ? 255 : r_cur;
				g_cur += delta_gx;
				g_cur = g_cur < 0 ? 0 : g_cur > 255 ? 255 : g_cur;
				b_cur += delta_bx;
				b_cur = b_cur < 0 ? 0 : b_cur > 255 ? 255 : b_cur;

				//检查是否贴图，避免不必要计算
				if (has_texture) {
					//限制纹理坐标范围
					s_cur += delta_sx;
					s_cur = s_cur < 0 ? 0 : s_cur > 1 ? 1 : s_cur;
					t_cur += delta_tx;
					t_cur = t_cur < 0 ? 0 : t_cur > 1 ? 1 : t_cur;
				}
				
			}
		}
		x_start += dx_left;
		x_end += dx_right;
		z_left += dz_left;
		z_right += dz_right;
		r_left += dr_left;
		r_right += dr_right;
		g_left += dg_left;
		g_right += dg_right;
		b_left += db_left;
		b_right += db_right;

		//检查是否贴图，避免不必要计算
		if (has_texture) {
			s_left += ds_left;
			s_right += ds_right;
			t_left += dt_left;
			t_right += dt_right;
		}
	}
}

//设置fps
void KZPipeLine::SetGameFps() {
	char window_title[64];
	sprintf_s(window_title, "Engine FPS:%f", fps_);
	WCHAR w_window_title[64];
	MultiByteToWideChar(CP_ACP, 0, window_title, strlen(window_title) + 1, w_window_title, sizeof(w_window_title) / sizeof(w_window_title[0]));
	SetWindowText(hwnd_, w_window_title);
	fps_ = 0;
}

//增加材质
void KZPipeLine::AddMaterial(const KZEngine::KZMaterial& new_mat) {
	uint32_t mat_num = mat_vec_.size();
	uint32_t index;
	//遍历材质数组防止重复材质
	for (index = 0; index < mat_num; ++index) {
		if (mat_vec_[index].id_ == new_mat.id_) {
			break;
		}
	}

	if (index == mat_num) {
		mat_vec_.push_back(new_mat);
	}
}