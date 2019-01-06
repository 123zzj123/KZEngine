#include"pipeline.h"
using namespace KZEngine;

//构造函数
KZPipeLine::KZPipeLine(uint32_t renderlist_num) {
	main_camera_.GetCameraPos().Set(1.5f, 1.5f, 3);
	LightBase* point_light = new PointLight(Color(255, 255, 0), KZMath::KZVector4D<float>(0, 10, -20), 1.0, 0.0f, 0.0f);
	LightBase* ambient_light = new AmbientLight(Color(255, 255, 0));
	LightBase* direction_light = new DirectionLight(Color(255, 255, 0), KZMath::KZVector4D<float>(-1, 0, 0));
	light_vec_.push_back(point_light);
	light_vec_.push_back(ambient_light);
	light_vec_.push_back(direction_light);
	light_active_vec_.push_back(true);
	light_active_vec_.push_back(false);
	light_active_vec_.push_back(false);
	
	pass_num_ = renderlist_num;

	for (uint32_t i = 0; i < renderlist_num; ++i) {
		pass_vec_.push_back(new Pass());
	}

	view_height_ = 600;
	view_width_ = 800;
	shadow_map_width_ = 1024;
	shadow_map_height_ = 1024;
	z_buffer_ = new float[view_width_ * view_height_]();
	depth_map_buffer_ = new float[shadow_map_width_ * shadow_map_height_]();
	//失败的多线程
	//mutex_buffer_ = new mutex[view_width_ * view_height_]();
	fill_n(z_buffer_, view_width_ * view_height_, 2.0f);
	frame_buffer_ = new unsigned char[view_width_ * view_height_ * 3]();
	record_time_ = 0;
};

KZPipeLine::~KZPipeLine() {
	uint32_t len = light_vec_.size();
	for (uint32_t i = 0; i < len; ++i) {
		delete light_vec_[i];
		light_vec_[i] = nullptr;
	}
	for (uint32_t i = 0; i < pass_num_; ++i)
	{
		delete pass_vec_[i];
		pass_vec_[i] = nullptr;
	}
	delete[] depth_map_buffer_;
	delete[] z_buffer_;
	delete[] frame_buffer_;
	//失败的多线程
	//delete[] mutex_buffer_;
}

//从文件中加载
void KZPipeLine::LoadFromFile() {
	//to do
}

//创建cube
void KZPipeLine::CreateCube(float width, float length, float height, 
	bool light_cube, int32_t pass_id, bool is_light, float alpha, const KZMath::KZVector4D<float>& world_pos, const KZMath::KZQuat& quat, const KZMath::KZVector4D<float>& scale, string shadow_map) {
	KZObject* cube_obj = new KZObject();
	//设置物体id
	cube_obj->id_ = obj_id_;
	++obj_id_;

	cube_obj->name_ = "cube";
	cube_obj->world_pos_ = world_pos;
	/*cube_obj.ux_ = KZMath::KZVector4D<float>(1, 0, 0, 0);
	cube_obj.uy_ = KZMath::KZVector4D<float>(0, 1, 0, 0);
	cube_obj.uz_ = KZMath::KZVector4D<float>(0, 0, 1, 0);*/
	cube_obj->num_vertices_ = 8;
	cube_obj->vlist_local_.reserve(cube_obj->num_vertices_);
	

	//设置顶点的模型坐标
	KZEngine::Vertex v1(width, length, height), v2(width, -length, height), v3(-width, -length, height), v4(-width, length, height), 
		v5(width, length, -height), v6(width, -length, -height), v7(-width, -length, -height), v8(-width, length, -height);
	/*v1.pos.Set(width, length, height);
	v2.pos.Set(width, -length, height);
	v3.pos.Set(-width, -length, height);
	v4.pos.Set(-width, length, height);
	v5.pos.Set(width, length, -height);
	v6.pos.Set(width, -length, -height);
	v7.pos.Set(-width, -length, -height);
	v8.pos.Set(-width, length, -height);*/

	if (!light_cube)
	{
		//设置顶点的颜色
		v1.color.Set(255, 0, 0);
		v2.color.Set(0, 255, 0);
		v3.color.Set(0, 0, 255);
		v4.color.Set(255, 255, 0);
		v5.color.Set(255, 0, 255);
		v6.color.Set(0, 255, 255);
		v7.color.Set(255, 255, 255);
		v8.color.Set(128, 128, 128);

		//设置顶点的uv坐标
		v1.uv.x_ = 1.0f;
		v1.uv.y_ = 0.0f;
		v2.uv.x_ = 1.0f;
		v2.uv.y_ = 1.0f;
		v3.uv.x_ = 0.0f;
		v3.uv.y_ = 1.0f;
		v4.uv.x_ = 0.0f;
		v4.uv.y_ = 0.0f;
	}
	else {
		cube_obj->has_shadow = false;
	}

	//将顶点加入列表
	cube_obj->vlist_local_.push_back(move(v1));
	cube_obj->vlist_local_.push_back(move(v2));
	cube_obj->vlist_local_.push_back(move(v3));
	cube_obj->vlist_local_.push_back(move(v4));
	cube_obj->vlist_local_.push_back(move(v5));
	cube_obj->vlist_local_.push_back(move(v6));
	cube_obj->vlist_local_.push_back(move(v7));
	cube_obj->vlist_local_.push_back(move(v8));

	cube_obj->num_index_ = 36;
	cube_obj->num_face_ = 12;
	cube_obj->index_.reserve(36);

	//设置顶点索引
	//front face
	cube_obj->index_.push_back(0);
	cube_obj->index_.push_back(2);
	cube_obj->index_.push_back(1);

	cube_obj->index_.push_back(0);
	cube_obj->index_.push_back(3);
	cube_obj->index_.push_back(2);

	//back face
	cube_obj->index_.push_back(7);
	cube_obj->index_.push_back(5);
	cube_obj->index_.push_back(6);

	cube_obj->index_.push_back(7);
	cube_obj->index_.push_back(4);
	cube_obj->index_.push_back(5);

	//left face
	cube_obj->index_.push_back(3);
	cube_obj->index_.push_back(6);
	cube_obj->index_.push_back(2);

	cube_obj->index_.push_back(3);
	cube_obj->index_.push_back(7);
	cube_obj->index_.push_back(6);

	//right face
	cube_obj->index_.push_back(4);
	cube_obj->index_.push_back(1);
	cube_obj->index_.push_back(5);

	cube_obj->index_.push_back(4);
	cube_obj->index_.push_back(0);
	cube_obj->index_.push_back(1);

	//top face
	cube_obj->index_.push_back(4);
	cube_obj->index_.push_back(3);
	cube_obj->index_.push_back(0);

	cube_obj->index_.push_back(4);
	cube_obj->index_.push_back(7);
	cube_obj->index_.push_back(3);

	//bottom face
	cube_obj->index_.push_back(1);
	cube_obj->index_.push_back(6);
	cube_obj->index_.push_back(5);

	cube_obj->index_.push_back(1);
	cube_obj->index_.push_back(2);
	cube_obj->index_.push_back(6);
	
	cube_obj->CalculateRadian();
	cube_obj->CalculateNormal();
	if (!light_cube)
	{
		string texture_path = "container.jpg";
		KZEngine::KZMaterial cube_mat("cube", Color(255, 255, 255), texture_path, 0.1f, 0.7f, 0.0f, 128, 3);
		cube_mat.has_texture_ = true;
		cube_mat.id_ = AddMaterial(cube_mat);

		//设置物体材质，-1表示无材质
		cube_obj->mat_id_.resize(12, -1);
		cube_obj->mat_id_[0] = cube_mat.id_;
		cube_obj->mat_id_[1] = cube_mat.id_;
		cube_obj->is_light_ = is_light;
	}
	else
	{
		//设置物体材质，-1表示无材质
		cube_obj->mat_id_.resize(12, -1);
		cube_obj->is_light_ = is_light;
	}

	

	if (quat != KZMath::KZQuat::ZERO) {
		cube_obj->RotationQuat(quat);
	}

	if (scale != KZMath::KZVector4D<float>(1, 1, 1))
	{
		cube_obj->ScaleMath(scale);
	}

	bool is_alpha = (alpha < 1.0f);
	//阴影位图物体
	if (calculate_shadow_ == CalCulateShadow::SHADOWTEXTURE && shadow_map != "")
	{
		addShadowObj("cube_shadow", shadow_map, cube_obj->world_pos_, cube_obj->max_radius_);
	}

	if (!is_alpha)
	{
		if (calculate_shadow_ == CalCulateShadow::VERTEXMAPPING && cube_obj->has_shadow) {
			uint32_t diff = cube_obj->num_index_;
			//uint32_t ori_num_face = cube_obj.num_face_;
			cube_obj->num_index_ <<= 1;
			cube_obj->num_face_ <<= 1;
			cube_obj->index_.reserve(cube_obj->num_index_);
			cube_obj->face_normal_.resize(cube_obj->num_face_);
			for (uint32_t i = 0; i < diff; ++i)
			{
				cube_obj->index_.push_back(cube_obj->index_[i] + cube_obj->num_vertices_);
			}
			assert(cube_obj->index_.size() == diff * 2);
			//设置物体材质，-1表示无材质
			cube_obj->mat_id_.resize(cube_obj->num_face_, -1);
		}

		//如果未指定Pass，默认不透明是pass0，透明是pass1
		if (pass_id == -1 && pass_id < (int32_t)pass_num_) {
			cube_obj->pass_id_ = 0;
			++pass_vec_[0]->object_num_;
			pass_vec_[0]->object_vec_.push_back(cube_obj);
		}
		else {
			cube_obj->pass_id_ = pass_id;
			++pass_vec_[pass_id]->object_num_;
			pass_vec_[pass_id]->object_vec_.push_back(cube_obj);
		}
		
	}
	else
	{
		cube_obj->alpha_ = alpha;
		//如果未指定Pass，默认不透明是pass0，透明是pass1
		if (pass_id == -1 && pass_id < (int32_t)pass_num_) {
			cube_obj->pass_id_ = 1;
			++pass_vec_[1]->object_num_;
			pass_vec_[1]->object_vec_.push_back(cube_obj);
			sort(pass_vec_[1]->object_vec_.begin(), pass_vec_[1]->object_vec_.end(), [](KZObject* a, KZObject* b) {return a->world_pos_.z_ > b->world_pos_.z_; });
		}
		else
		{
			cube_obj->pass_id_ = pass_id;
			++pass_vec_[pass_id]->object_num_;
			pass_vec_[pass_id]->object_vec_.push_back(cube_obj);
		}
		
	}
}

//创建三棱锥
void KZPipeLine::CreatePyramid(int32_t pass_id,  bool is_light, float alpha, const KZMath::KZVector4D<float>& world_pos, const KZMath::KZQuat& quat, const KZMath::KZVector4D<float>& scale, string shadow_map) {
	KZObject* pyramid_obj = new KZObject();
	//设置物体id
	pyramid_obj->id_ = obj_id_;
	++obj_id_;

	pyramid_obj->name_ = "pyramid";
	pyramid_obj->world_pos_ = world_pos;
	pyramid_obj->ux_ = KZMath::KZVector4D<float>(1, 0, 0, 0);
	pyramid_obj->uy_ = KZMath::KZVector4D<float>(0, 1, 0, 0);
	pyramid_obj->uz_ = KZMath::KZVector4D<float>(0, 0, 1, 0);
	pyramid_obj->num_vertices_ = 4;
	pyramid_obj->vlist_local_.reserve(pyramid_obj->num_vertices_);

	//设置顶点的模型坐标
	KZEngine::Vertex v1(-1, 0, 0), v2(1, 0, 0), v3(0, 0, 2), v4(0, 2, 1);
	/*v1.pos.Set(-1, 0, 0);
	v2.pos.Set(1, 0, 0);
	v3.pos.Set(0, 0, 2);
	v4.pos.Set(0, 2, 1);*/

	v1.color.Set(0, 0, 0);
	v2.color.Set(0, 0, 0);
	v3.color.Set(0, 0, 0);
	v4.color.Set(0, 0, 0);

	//将顶点加入列表
	pyramid_obj->vlist_local_.push_back(v1);
	pyramid_obj->vlist_local_.push_back(v2);
	pyramid_obj->vlist_local_.push_back(v3);
	pyramid_obj->vlist_local_.push_back(v4);

	pyramid_obj->num_index_ = 12;
	pyramid_obj->num_face_ = 4;
	pyramid_obj->index_.reserve(12);

	//设置顶点索引
	//face 1
	pyramid_obj->index_.push_back(3);
	pyramid_obj->index_.push_back(0);
	pyramid_obj->index_.push_back(2);

	//face 2
	pyramid_obj->index_.push_back(3);
	pyramid_obj->index_.push_back(2);
	pyramid_obj->index_.push_back(1);

	//face 3
	pyramid_obj->index_.push_back(3);
	pyramid_obj->index_.push_back(1);
	pyramid_obj->index_.push_back(0);

	//face 4
	pyramid_obj->index_.push_back(1);
	pyramid_obj->index_.push_back(2);
	pyramid_obj->index_.push_back(0);

	

	pyramid_obj->CalculateRadian();
	pyramid_obj->CalculateNormal();
	pyramid_obj->is_light_ = is_light;
	string texture_path = "";
	KZEngine::KZMaterial pyramid_mat("pyramid", Color(255, 255, 255), texture_path, 0.1f, 0.7f, 0.0f, 128, 3);
	pyramid_mat.has_texture_ = false;
	pyramid_mat.id_ = AddMaterial(pyramid_mat);
	pyramid_obj->mat_id_.resize(4, pyramid_mat.id_);

	if (quat != KZMath::KZQuat::ZERO) {
		pyramid_obj->RotationQuat(quat);
	}

	if (scale != KZMath::KZVector4D<float>(1, 1, 1))
	{
		pyramid_obj->ScaleMath(scale);
	}

	//阴影位图物体
	if (calculate_shadow_ == CalCulateShadow::SHADOWTEXTURE && shadow_map != "")
	{
		addShadowObj("cube_shadow", shadow_map, pyramid_obj->world_pos_, pyramid_obj->max_radius_);
	}

	bool is_alpha = (alpha < 1.0f);
	if (!is_alpha)
	{
		if (calculate_shadow_ == CalCulateShadow::VERTEXMAPPING) {
			uint32_t diff = pyramid_obj->num_index_;
			//uint32_t ori_num_face = cube_obj.num_face_;
			pyramid_obj->num_index_ <<= 1;
			pyramid_obj->num_face_ <<= 1;
			pyramid_obj->index_.reserve(pyramid_obj->num_index_);
			pyramid_obj->face_normal_.resize(pyramid_obj->num_face_);
			for (uint32_t i = 0; i < diff; ++i)
			{
				pyramid_obj->index_.push_back(pyramid_obj->index_[i] + pyramid_obj->num_vertices_);
			}
			assert(pyramid_obj->index_.size() == diff * 2);
			//设置物体材质，-1表示无材质
			pyramid_obj->mat_id_.resize(pyramid_obj->num_face_, -1);
		}

		//如果未指定Pass，默认不透明是pass0，透明是pass1
		if (pass_id == -1 && pass_id < (int32_t)pass_num_) {
			pyramid_obj->pass_id_ = 0;
			++pass_vec_[0]->object_num_;
			pass_vec_[0]->object_vec_.push_back(pyramid_obj);
		}
		else
		{
			pyramid_obj->pass_id_ = pass_id;
			++pass_vec_[pass_id]->object_num_;
			pass_vec_[pass_id]->object_vec_.push_back(pyramid_obj);
		}
		
	}
	else
	{
		pyramid_obj->alpha_ = alpha;

		//如果未指定Pass，默认不透明是pass0，透明是pass1
		if (pass_id == -1 && pass_id < (int32_t)pass_num_) {
			pyramid_obj->pass_id_ = 1;
			++pass_vec_[1]->object_num_;
			pass_vec_[1]->object_vec_.push_back(pyramid_obj);
			sort(pass_vec_[1]->object_vec_.begin(), pass_vec_[1]->object_vec_.end(), [](KZObject* a, KZObject* b) {return a->world_pos_.z_ > b->world_pos_.z_; });
		}
		else
		{
			pyramid_obj->pass_id_ = pass_id;
			++pass_vec_[pass_id]->object_num_;
			pass_vec_[pass_id]->object_vec_.push_back(pyramid_obj);
		}
	}
}

//创建圆柱
void KZPipeLine::CreateCylinder(float top_radius, float bottom_radius, float height, uint32_t stack, uint32_t slice,
	const KZEngine::Color& ini_color,
	int32_t pass_id,
	bool is_light,
	float alpha,
	const KZMath::KZVector4D<float>& world_pos,
	const KZMath::KZQuat& quat,
	const KZMath::KZVector4D<float>& scale,
	string shadow_map)
{
	KZObject* cylinder_obj = new KZObject();
	//设置物体id
	cylinder_obj->id_ = obj_id_;
	++obj_id_;

	cylinder_obj->name_ = "cylinder";
	cylinder_obj->world_pos_ = world_pos;
	cylinder_obj->ux_ = KZMath::KZVector4D<float>(1, 0, 0, 0);
	cylinder_obj->uy_ = KZMath::KZVector4D<float>(0, 1, 0, 0);
	cylinder_obj->uz_ = KZMath::KZVector4D<float>(0, 0, 1, 0);

	//uint32_t verts_per_row = slice;
	uint32_t n_rows = stack + 1;

	//+2因为顶部和底部各有个中心的顶点
	cylinder_obj->num_vertices_ = slice * n_rows + 2;
	cylinder_obj->vlist_local_.resize(cylinder_obj->num_vertices_);

	//stack * slice * 2 * 3 + slice * 2 * 3
	cylinder_obj->num_face_ = (stack+1) * 2 * slice;
	cylinder_obj->num_index_ = cylinder_obj->num_face_ * 3;
	cylinder_obj->index_.reserve(cylinder_obj->num_index_);
	

	float half_height = height * 0.5f;
	float d_radius = (bottom_radius - top_radius) / (float)stack;
	float d_height = height / (float)stack;
	float temp_y = half_height;
	float temp_radius = top_radius;
	
	float d_theta = 6.28318f / slice;

	uint32_t vertex_idx = 0;
	for (uint32_t i = 0; i < n_rows; ++i)
	{
		//float temp_y = half_height - i * d_height;
		//float temp_radius = top_radius + i * d_radius;
		float theta = 0;
		for (uint32_t j = 0; j < slice; ++j)
		{
			cylinder_obj->vlist_local_[vertex_idx].pos.Set(temp_radius * cos(theta), temp_y, temp_radius * sin(theta));
			cylinder_obj->vlist_local_[vertex_idx].color = ini_color;
			theta += d_theta;
			if (i != stack)
			{
				cylinder_obj->index_.push_back(vertex_idx);
				cylinder_obj->index_.push_back(vertex_idx + 1);
				cylinder_obj->index_.push_back(vertex_idx + 1 + slice);

				cylinder_obj->index_.push_back(vertex_idx);
				cylinder_obj->index_.push_back(vertex_idx + 1 + slice);
				cylinder_obj->index_.push_back(vertex_idx + slice);
			}

			++vertex_idx;
		}
		temp_y -= d_height;
		temp_radius += d_radius;
	}

	cylinder_obj->vlist_local_[vertex_idx].pos.Set(0, half_height, 0);
	cylinder_obj->vlist_local_[vertex_idx].color = Color(255, 0, 0);
	++vertex_idx;
	cylinder_obj->vlist_local_[vertex_idx].pos.Set(0, -half_height, 0);
	cylinder_obj->vlist_local_[vertex_idx].color = Color(255, 0, 0);

	uint32_t next_index = 0;
	uint32_t diff = stack * slice;
	for (uint32_t i = 0; i < slice; ++i)
	{
		next_index = i + 1;
		if (next_index == slice)
		{
			next_index = 0;
		}
		cylinder_obj->index_.push_back(next_index);
		cylinder_obj->index_.push_back(i);
		cylinder_obj->index_.push_back(cylinder_obj->num_vertices_ - 2);

		cylinder_obj->index_.push_back(diff + next_index);
		cylinder_obj->index_.push_back(diff + i);
		cylinder_obj->index_.push_back(cylinder_obj->num_vertices_ - 1);
	}

	cylinder_obj->mat_id_.resize(cylinder_obj->num_face_, -1);
	cylinder_obj->is_light_ = is_light;
	cylinder_obj->CalculateRadian();
	cylinder_obj->CalculateNormal();

	if (quat != KZMath::KZQuat::ZERO) {
		cylinder_obj->RotationQuat(quat);
	}

	if (scale != KZMath::KZVector4D<float>(1, 1, 1))
	{
		cylinder_obj->ScaleMath(scale);
	}

	//阴影位图物体
	if (calculate_shadow_ == CalCulateShadow::SHADOWTEXTURE && shadow_map != "")
	{
		addShadowObj("cylinder_shadow", shadow_map, cylinder_obj->world_pos_, cylinder_obj->max_radius_);
	}

	bool is_alpha = (alpha < 1.0f);
	if (!is_alpha)
	{
		if (calculate_shadow_ == CalCulateShadow::VERTEXMAPPING) {
			uint32_t diff = cylinder_obj->num_index_;
			//uint32_t ori_num_face = cube_obj.num_face_;
			cylinder_obj->num_index_ <<= 1;
			cylinder_obj->num_face_ <<= 1;
			cylinder_obj->index_.reserve(cylinder_obj->num_index_);
			cylinder_obj->face_normal_.resize(cylinder_obj->num_face_);
			for (uint32_t i = 0; i < diff; ++i)
			{
				cylinder_obj->index_.push_back(cylinder_obj->index_[i] + cylinder_obj->num_vertices_);
			}
			assert(cylinder_obj->index_.size() == diff * 2);
			//设置物体材质，-1表示无材质
			cylinder_obj->mat_id_.resize(cylinder_obj->num_face_, -1);
		}

		//如果未指定Pass，默认不透明是pass0，透明是pass1
		if (pass_id == -1 && pass_id < (int32_t)pass_num_) {
			cylinder_obj->pass_id_ = 0;
			++pass_vec_[0]->object_num_;
			pass_vec_[0]->object_vec_.push_back(cylinder_obj);
		} 
		else 
		{
			cylinder_obj->pass_id_ = pass_id;
			++pass_vec_[pass_id]->object_num_;
			pass_vec_[pass_id]->object_vec_.push_back(cylinder_obj);
		}
	}
	else
	{
		cylinder_obj->alpha_ = alpha;

		//如果未指定Pass，默认不透明是pass0，透明是pass1
		if (pass_id == -1 && pass_id < (int32_t)pass_num_) {
			cylinder_obj->pass_id_ = 1;
			++pass_vec_[1]->object_num_;
			pass_vec_[1]->object_vec_.push_back(cylinder_obj);
			sort(pass_vec_[1]->object_vec_.begin(), pass_vec_[1]->object_vec_.end(), [](KZObject* a, KZObject* b) {return a->world_pos_.z_ > b->world_pos_.z_; });
		}
		else
		{
			cylinder_obj->pass_id_ = pass_id;
			++pass_vec_[pass_id]->object_num_;
			pass_vec_[pass_id]->object_vec_.push_back(cylinder_obj);
		}
		
	}

	
}

	//创建球体
void KZPipeLine::CreateSphere(float radius, uint32_t stack, uint32_t slice,
	const KZEngine::Color& ini_color,
	int32_t pass_id,
	bool is_light,
	float alpha,
	const KZMath::KZVector4D<float>& world_pos,
	const KZMath::KZQuat& quat,
	const KZMath::KZVector4D<float>& scale,
	string shadow_map)
{
	KZObject* sphere_obj = new KZObject();
	//设置物体id
	sphere_obj->id_ = obj_id_;
	++obj_id_;

	sphere_obj->name_ = "sphere";
	sphere_obj->world_pos_ = world_pos;
	sphere_obj->ux_ = KZMath::KZVector4D<float>(1, 0, 0, 0);
	sphere_obj->uy_ = KZMath::KZVector4D<float>(0, 1, 0, 0);
	sphere_obj->uz_ = KZMath::KZVector4D<float>(0, 0, 1, 0);

	uint32_t vertex_idx = 0;
	
	//+2因为顶部和底部各有个中心的顶点
	sphere_obj->num_vertices_ = slice * (stack - 1) + 2;
	sphere_obj->vlist_local_.resize(sphere_obj->num_vertices_);

	//(stack - 2) * slice * 2 + slice * 2
	sphere_obj->num_face_ = (stack - 1) * 2 * slice;
	sphere_obj->num_index_ = sphere_obj->num_face_ * 3;
	sphere_obj->index_.reserve(sphere_obj->num_index_);

	float temp_radius = 0;
	float d_phy = 3.14159f / stack;
	float d_theta = 6.28318f / slice;
	float phy = d_phy;
	for (uint32_t i = 1; i < stack; ++i)
	{
		float theta = 0;
		temp_radius = radius * sin(phy);
		for (uint32_t j = 0; j < slice; ++j)
		{
			sphere_obj->vlist_local_[vertex_idx].pos.Set(temp_radius * cos(theta), radius * cos(phy), temp_radius * sin(theta));
			sphere_obj->vlist_local_[vertex_idx].color = ini_color;
			if (i != stack - 1)
			{
				sphere_obj->index_.push_back(vertex_idx);
				sphere_obj->index_.push_back(vertex_idx + 1);
				sphere_obj->index_.push_back(vertex_idx + 1 + slice);

				sphere_obj->index_.push_back(vertex_idx);
				sphere_obj->index_.push_back(vertex_idx + 1 + slice);
				sphere_obj->index_.push_back(vertex_idx + slice);
			}
			theta += d_theta;
			++vertex_idx;
		}
		phy += d_phy;
	}

	sphere_obj->vlist_local_[vertex_idx].pos.Set(0, radius, 0);
	sphere_obj->vlist_local_[vertex_idx].color = Color(255, 0, 0);
	++vertex_idx;
	sphere_obj->vlist_local_[vertex_idx].pos.Set(0, -radius, 0);
	sphere_obj->vlist_local_[vertex_idx].color = Color(255, 0, 0);

	uint32_t next_index = 0;
	uint32_t diff = (stack - 2) * slice;
	for (uint32_t i = 0; i < slice; ++i)
	{
		next_index = i + 1;
		if (next_index == slice)
		{
			next_index = 0;
		}
		sphere_obj->index_.push_back(next_index);
		sphere_obj->index_.push_back(i);
		sphere_obj->index_.push_back(sphere_obj->num_vertices_ - 2);

		sphere_obj->index_.push_back(diff + next_index);
		sphere_obj->index_.push_back(diff + i);
		sphere_obj->index_.push_back(sphere_obj->num_vertices_ - 1);
	}

	sphere_obj->mat_id_.resize(sphere_obj->num_face_, -1);
	sphere_obj->is_light_ = is_light;
	sphere_obj->CalculateRadian();
	sphere_obj->CalculateNormal();

	if (quat != KZMath::KZQuat::ZERO) {
		sphere_obj->RotationQuat(quat);
	}

	if (scale != KZMath::KZVector4D<float>(1, 1, 1))
	{
		sphere_obj->ScaleMath(scale);
	}

	//阴影位图物体
	if (calculate_shadow_ == CalCulateShadow::SHADOWTEXTURE && shadow_map != "")
	{
		addShadowObj("sphere_shadow", shadow_map, sphere_obj->world_pos_, sphere_obj->max_radius_);
	}

	bool is_alpha = (alpha < 1.0f);
	if (!is_alpha)
	{
		if (calculate_shadow_ == CalCulateShadow::VERTEXMAPPING && world_pos.y_ > 0.0f) {
			uint32_t diff = sphere_obj->num_index_;
			//uint32_t ori_num_face = cube_obj.num_face_;
			sphere_obj->num_index_ <<= 1;
			sphere_obj->num_face_ <<= 1;
			sphere_obj->index_.reserve(sphere_obj->num_index_);
			sphere_obj->face_normal_.resize(sphere_obj->num_face_);
			for (uint32_t i = 0; i < diff; ++i)
			{
				sphere_obj->index_.push_back(sphere_obj->index_[i] + sphere_obj->num_vertices_);
			}
			assert(sphere_obj->index_.size() == diff * 2);
			//设置物体材质，-1表示无材质
			sphere_obj->mat_id_.resize(sphere_obj->num_face_, -1);
		}

		if (pass_id == -1 && pass_id < (int32_t)pass_num_) {
			sphere_obj->pass_id_ = 0;
			++pass_vec_[0]->object_num_;
			pass_vec_[0]->object_vec_.push_back(sphere_obj);
		}
		else
		{
			sphere_obj->pass_id_ = pass_id;
			++pass_vec_[pass_id]->object_num_;
			pass_vec_[pass_id]->object_vec_.push_back(sphere_obj);
		}
		
	}
	else
	{
		sphere_obj->alpha_ = alpha;
		if (pass_id == -1 && pass_id < (int32_t)pass_num_) {
			sphere_obj->pass_id_ = 1;
			++pass_vec_[1]->object_num_;
			pass_vec_[1]->object_vec_.push_back(sphere_obj);
			sort(pass_vec_[1]->object_vec_.begin(), pass_vec_[1]->object_vec_.end(), [](KZObject* a, KZObject* b) {return a->world_pos_.z_ > b->world_pos_.z_; });
		}
		else
		{
			sphere_obj->pass_id_ = pass_id;
			++pass_vec_[pass_id]->object_num_;
			pass_vec_[pass_id]->object_vec_.push_back(sphere_obj);
		}
		
	}
}

//创建地形
void KZPipeLine::Create_Terrain(float width, float height, float vscale, const char* height_map_file_name, const char* texture_map_file_name,
	const KZEngine::Color& ini_color, int32_t pass_id, bool is_light, float alpha, const KZMath::KZVector4D<float>& world_pos, const KZMath::KZQuat& quat)
{
	assert(height != 0.0f && width != 0 && vscale > 0.0f && strlen(height_map_file_name) != 0);
	if (height == 0.0f || width == 0.0f || vscale < 0.0f || strlen(height_map_file_name) == 0)
	{
		return;
	}
	
	//加载高度图
	KZEngine::KZImage height_img(height_map_file_name);
	uint32_t h_img_height = height_img.GetHeight();
	uint32_t h_img_width = height_img.GetWidth();
	int32_t terrain_mat_index = -1;

	if (h_img_height != 0 && h_img_width != 0)
	{
		KZObject* terrain_obj = new KZObject();

		terrain_obj->is_light_ = is_light;

		//设置物体id
		terrain_obj->id_ = obj_id_;
		++obj_id_;
		terrain_obj->name_ = "Terrain";

		terrain_obj->world_pos_ = world_pos;
		terrain_obj->ux_ = KZMath::KZVector4D<float>(1, 0, 0, 0);
		terrain_obj->uy_ = KZMath::KZVector4D<float>(0, 1, 0, 0);
		terrain_obj->uz_ = KZMath::KZVector4D<float>(0, 0, 1, 0);

		terrain_obj->num_vertices_ = h_img_height * h_img_width;
		terrain_obj->num_face_ = (h_img_height - 1) * (h_img_width - 1) * 2;
		terrain_obj->num_index_ = terrain_obj->num_face_ * 3;
		
		terrain_obj->vlist_local_.resize(terrain_obj->num_vertices_);
		terrain_obj->index_.reserve(terrain_obj->num_index_);

		//三角形列步长
		float col_step = 0.0f;
		//三角形行步长
		float row_step = 0.0f;
		//uv列的步长
		float uv_col_step = 0.0f;
		//uv行的步长
		float uv_row_step = 0.0f;

		if (h_img_width > 1 )
		{
			col_step = width / (float)(h_img_width - 1);
		}

		if (h_img_height > 1)
		{
			row_step = height / (float)(h_img_height - 1);
		}

		//加载纹理图
		if (strlen(texture_map_file_name) != 0)
		{
			KZEngine::KZMaterial terrain_mat("terrain" ,Color(255, 255, 0), texture_map_file_name, 0.1f, 0.7f, 0.0f, 128);
			terrain_mat.id_ = 1;
			terrain_mat.has_texture_ = true;
			terrain_mat_index = AddMaterial(terrain_mat);

			uint32_t t_map_height = terrain_mat.GetBitMapHeight();
			uint32_t t_map_width = terrain_mat.GetBitMapWidth();
			if (t_map_width > 1)
			{
				uv_col_step = 1.0f / (float)(h_img_width - 1);
			}
			if (h_img_height > 1)
			{
				uv_row_step = 1.0f / (float)(h_img_height - 1);
			}

			
		}

		uint32_t vertex_idx = 0;
		float half_width = width / 2;
		float half_height = height / 2;
		uint32_t value = 0;
		//生成顶点列表,索引列表
		for (uint32_t i = 0; i < h_img_height; ++i)
		{
			for (uint32_t j = 0; j < h_img_width; ++j)
			{
				terrain_obj->vlist_local_[vertex_idx].pos.x_ = (i * row_step - half_width);
				height_img.GetSingleChannelColor(i, j, value);
				terrain_obj->vlist_local_[vertex_idx].pos.y_ = vscale * (value / 255.0f);
				terrain_obj->vlist_local_[vertex_idx].pos.z_ = (j * col_step - half_height);
				terrain_obj->vlist_local_[vertex_idx].pos.w_ = 1;

				terrain_obj->vlist_local_[vertex_idx].color = ini_color;

				if (texture_map_file_name != "")
				{
					terrain_obj->vlist_local_[vertex_idx].uv.x_ = i * uv_row_step;
					terrain_obj->vlist_local_[vertex_idx].uv.y_ = j * uv_col_step;
				}

				if (i != h_img_height - 1 || j != h_img_width - 1)
				{
					uint32_t base_index = vertex_idx;
					terrain_obj->index_.push_back(base_index);
					terrain_obj->index_.push_back(base_index + h_img_width);
					terrain_obj->index_.push_back(base_index + 1);

					terrain_obj->index_.push_back(base_index + 1);
					terrain_obj->index_.push_back(base_index + h_img_width);
					terrain_obj->index_.push_back(base_index + h_img_width + 1);
				}

				++vertex_idx;
			}
		}

		terrain_obj->mat_id_.resize(terrain_obj->num_face_, terrain_mat_index);
		terrain_obj->CalculateRadian();
		terrain_obj->CalculateNormal();

		if (quat != KZMath::KZQuat::ZERO) {
			terrain_obj->RotationQuat(quat);
		}

		bool is_alpha = (alpha < 1.0f);
		if (!is_alpha)
		{
			if (pass_id == -1 && pass_id < (int32_t)pass_num_) {
				terrain_obj->pass_id_ = 0;
				++pass_vec_[0]->object_num_;
				pass_vec_[0]->object_vec_.push_back(terrain_obj);
			}
			else
			{
				terrain_obj->pass_id_ = pass_id;
				++pass_vec_[pass_id]->object_num_;
				pass_vec_[pass_id]->object_vec_.push_back(terrain_obj);
			}
			
		}
		else
		{
			terrain_obj->alpha_ = alpha;
			if (pass_id == -1 && pass_id < (int32_t)pass_num_) {
				terrain_obj->pass_id_ = 1;
				++pass_vec_[1]->object_num_;
				pass_vec_[1]->object_vec_.push_back(move(terrain_obj));
				sort(pass_vec_[1]->object_vec_.begin(), pass_vec_[1]->object_vec_.end(), [](KZObject* a, KZObject* b) {return a->world_pos_.z_ > b->world_pos_.z_; });
			}
			else 
			{
				terrain_obj->pass_id_ = pass_id;
				++pass_vec_[pass_id]->object_num_;
				pass_vec_[pass_id]->object_vec_.push_back(terrain_obj);
			}
			
		}
	}
	
}

//增加阴影位图物体
void KZPipeLine::addShadowObj(const string& shadow_name, const string& shadow_map_texture, const KZMath::KZVector4D<float>& ori_world_pos, const float radius)
{
	KZObject* shadow_obj = new KZObject();
	shadow_obj->alpha_ = 0.5f;
	//设置物体id
	shadow_obj->id_ = obj_id_;
	++obj_id_;
	shadow_obj->name_ = shadow_name;

	KZMath::KZVector4D<float> light_pos;
	light_vec_[0]->GetLightPos(light_pos);
	//经过点光源与物体中心直线与地面相交参数方程 ps = pl + t * (p0 - pl), 令ps.y = 0;
	float t = -light_pos.y_ / (ori_world_pos.y_ - light_pos.y_);
	shadow_obj->world_pos_.x_ = light_pos.x_ + t * (ori_world_pos.x_ - light_pos.x_);
	shadow_obj->world_pos_.y_ = 0.0f;
	shadow_obj->world_pos_.z_ = light_pos.z_ + t * (ori_world_pos.z_ - light_pos.z_);

	shadow_obj->ux_ = KZMath::KZVector4D<float>(1, 0, 0, 0);
	shadow_obj->uy_ = KZMath::KZVector4D<float>(0, 1, 0, 0);
	shadow_obj->uz_ = KZMath::KZVector4D<float>(0, 0, 1, 0);
	shadow_obj->num_vertices_ = 4;
	shadow_obj->vlist_local_.reserve(shadow_obj->num_vertices_);

	KZEngine::Vertex v1(1, 0, 1), v2(1, 0, -1), v3(-1, 0, -1), v4(-1, 0, 1);
	/*v1.pos.Set(1, 0, 1);
	v2.pos.Set(1, 0, -1);
	v3.pos.Set(-1, 0, -1);
	v4.pos.Set(-1, 0, 1);*/

	//设置顶点的颜色
	/*v1.color.Set(255, 255, 255);
	v2.color.Set(255, 255, 255);
	v3.color.Set(255, 255, 255);
	v4.color.Set(255, 255, 255);*/

	//设置顶点的uv坐标
	v1.uv.x_ = 1.0f;
	v1.uv.y_ = 1.0f;
	v2.uv.x_ = 1.0f;
	v2.uv.y_ = 0.0f;
	v3.uv.x_ = 0.0f;
	v3.uv.y_ = 0.0f;
	v4.uv.x_ = 0.0f;
	v4.uv.y_ = 1.0f;

	//将顶点加入列表
	shadow_obj->vlist_local_.push_back(v1);
	shadow_obj->vlist_local_.push_back(v2);
	shadow_obj->vlist_local_.push_back(v3);
	shadow_obj->vlist_local_.push_back(v4);

	shadow_obj->num_index_ = 6;
	shadow_obj->num_face_ = 2;
	shadow_obj->index_.reserve(6);

	//设置顶点索引
	shadow_obj->index_.push_back(0);
	shadow_obj->index_.push_back(1);
	shadow_obj->index_.push_back(2);

	shadow_obj->index_.push_back(0);
	shadow_obj->index_.push_back(2);
	shadow_obj->index_.push_back(3);

	KZEngine::KZMaterial shadow_mat(shadow_name, Color(0, 0, 0), shadow_map_texture, 0.1f, 0.7f, 0.0f, 128, 3);
	shadow_mat.has_texture_ = true;
	shadow_mat.id_ = AddMaterial(shadow_mat);

	//设置物体材质，-1表示无材质
	shadow_obj->mat_id_.resize(2, -1);
	shadow_obj->mat_id_[0] = shadow_mat.id_;
	shadow_obj->mat_id_[1] = shadow_mat.id_;
	shadow_obj->is_light_ = false;

	//根据相似三角形，由光源的高度计算出缩放因子
	float scale_xz = 0.5f * radius * (light_pos.y_ / (light_pos.y_ - ori_world_pos.y_));
	shadow_obj->ScaleMath(KZMath::KZVector4D<float>(scale_xz, 1.0f, scale_xz));

	shadow_obj->CalculateRadian();
	shadow_obj->CalculateNormal();

	shadow_obj->pass_id_ = 1;
	//阴影物体视为透明物体
	++pass_vec_[1]->object_num_;
	pass_vec_[1]->object_vec_.push_back(shadow_obj);
	sort(pass_vec_[1]->object_vec_.begin(), pass_vec_[1]->object_vec_.end(), [](KZObject* a, KZObject* b) {return a->world_pos_.z_ > b->world_pos_.z_; });
}

//转化到世界坐标
void KZPipeLine::TransformModelToWorld() {
	for (uint32_t i = 0; i < pass_vec_[pass_idx_]->object_num_; ++i) {
		pass_vec_[pass_idx_]->object_vec_[i]->TransformModelToWorldMath();
		if (calculate_shadow_ == CalCulateShadow::VERTEXMAPPING && pass_vec_[pass_idx_]->object_vec_[i]->has_shadow && pass_vec_[pass_idx_]->object_vec_[i]->world_pos_.y_ > 0)
		{
			KZMath::KZVector4D<float> light_pos;
			light_vec_[0]->GetLightPos(light_pos);
			for (uint32_t j = 0; j < pass_vec_[pass_idx_]->object_vec_[i]->num_vertices_; ++j)
			{
				//经过点光源与物体中心直线与地面相交参数方程 ps = pl + t * (p0 - pl), 令ps.y = 0;
				float t = -light_pos.y_ / (pass_vec_[pass_idx_]->object_vec_[i]->vlist_tran_[j].pos.y_ - light_pos.y_);
				float temp_x = light_pos.x_ + t * (pass_vec_[pass_idx_]->object_vec_[i]->vlist_tran_[j].pos.x_ - light_pos.x_);
				float temp_z = light_pos.z_ + t * (pass_vec_[pass_idx_]->object_vec_[i]->vlist_tran_[j].pos.z_ - light_pos.z_);
				pass_vec_[pass_idx_]->object_vec_[i]->vlist_tran_.push_back(KZEngine::Vertex(temp_x, 0.0f, temp_z, 128, 128, 128));
			}
		}
	}
}

//物体消除，包围球测试
void KZPipeLine::OcclusionCulling() {
	KZMath::KZVector4D<float> temp_camera_pos;
	KZMath::KZMatrix44 view;
	main_camera_.GetViewMatrix(view);
	float camera_right_over_near = main_camera_.GetViewRight() / main_camera_.GetCameraNearClip();
	float camera_top_over_near = main_camera_.GetViewTop() / main_camera_.GetCameraNearClip();
	//不透明物体
	for (uint32_t i = 0; i < pass_vec_[pass_idx_]->object_num_; ++i) {
		temp_camera_pos = view * pass_vec_[pass_idx_]->object_vec_[i]->world_pos_;
		//根据远近平面裁剪
		if ((temp_camera_pos.z_ + pass_vec_[pass_idx_]->object_vec_[i]->max_radius_ < main_camera_.GetCameraFarClip()) || (temp_camera_pos.z_ - pass_vec_[pass_idx_]->object_vec_[i]->max_radius_ > main_camera_.GetCameraNearClip())) {
			pass_vec_[pass_idx_]->object_vec_[i]->active_ = false;
			continue;
		}
		//根据左右平面裁剪
		float w_test = camera_right_over_near * temp_camera_pos.z_;
		if ((temp_camera_pos.x_ - pass_vec_[pass_idx_]->object_vec_[i]->max_radius_ > w_test) || (temp_camera_pos.x_ + pass_vec_[pass_idx_]->object_vec_[i]->max_radius_ < -w_test)) {
			pass_vec_[pass_idx_]->object_vec_[i]->active_ = false;
			continue;
		}

		//根据上下平面裁剪
		float h_test = camera_top_over_near * temp_camera_pos.z_;
		if ((temp_camera_pos.y_ - pass_vec_[pass_idx_]->object_vec_[i]->max_radius_ > h_test) || (temp_camera_pos.y_ + pass_vec_[pass_idx_]->object_vec_[i]->max_radius_ < -h_test)) {
			pass_vec_[pass_idx_]->object_vec_[i]->active_ = false;
			continue;
		}
	}

	return;
}

//背面消除
void KZPipeLine::BackfaceCulling() {
	//普通物体
	for (uint32_t i = 0; i < pass_vec_[pass_idx_]->object_num_; ++i) {
		if (pass_vec_[pass_idx_]->object_vec_[i]->active_) {
			uint32_t face_index = 0;
			for (uint32_t j = 0; j < pass_vec_[pass_idx_]->object_vec_[i]->num_index_; face_index++, j += 3) {
				KZMath::KZVector4D<float> observe_vec = main_camera_.GetCameraPos() - pass_vec_[pass_idx_]->object_vec_[i]->vlist_tran_[pass_vec_[pass_idx_]->object_vec_[i]->index_[j]].pos;
				if (pass_vec_[pass_idx_]->object_vec_[i]->face_normal_[face_index].Vector3Dot(observe_vec) < 0) {
					continue;
				}
				else
				{
					++pass_vec_[pass_idx_]->tri_num_;
					if (pass_vec_[pass_idx_]->tri_num_ > pass_vec_[pass_idx_]->render_list_->tri_list_.size()) {
						uint32_t old_len = pass_vec_[pass_idx_]->render_list_->tri_list_.size();
						pass_vec_[pass_idx_]->render_list_->tri_list_.resize(pass_vec_[pass_idx_]->tri_num_ * 2, nullptr);
						uint32_t new_len = pass_vec_[pass_idx_]->render_list_->tri_list_.size();
						for (uint32_t i = old_len; i < new_len; ++i)
						{
							pass_vec_[pass_idx_]->render_list_->tri_list_[i] = new KZEngine::Triangle();
						}
					}
					KZEngine::TrianglePtr tri = pass_vec_[pass_idx_]->render_list_->tri_list_[pass_vec_[pass_idx_]->tri_num_ - 1];
					uint32_t idx0 = j, idx1 = j + 1, idx2 = j + 2;
					if (pass_vec_[pass_idx_]->object_vec_[i]->is_light_) {
						uint32_t light_num = static_cast<uint32_t>(light_vec_.size());
						for (uint32_t k = 0; k < light_num; ++k) {
							if (light_active_vec_[k]) {
								pass_vec_[pass_idx_]->object_vec_[i]->vlist_tran_[pass_vec_[pass_idx_]->object_vec_[i]->index_[idx0]].color += 
									mat_vec_[pass_vec_[pass_idx_]->object_vec_[i]->mat_id_[face_index]].CalculateFinalColor(light_vec_[k],
										pass_vec_[pass_idx_]->object_vec_[i]->vlist_tran_[pass_vec_[pass_idx_]->object_vec_[i]->index_[idx0]].pos,
										pass_vec_[pass_idx_]->object_vec_[i]->vlist_tran_[pass_vec_[pass_idx_]->object_vec_[i]->index_[idx0]].normal);
								pass_vec_[pass_idx_]->object_vec_[i]->vlist_tran_[pass_vec_[pass_idx_]->object_vec_[i]->index_[idx1]].color += 
									mat_vec_[pass_vec_[pass_idx_]->object_vec_[i]->mat_id_[face_index]].CalculateFinalColor(light_vec_[k],
										pass_vec_[pass_idx_]->object_vec_[i]->vlist_tran_[pass_vec_[pass_idx_]->object_vec_[i]->index_[idx1]].pos,
										pass_vec_[pass_idx_]->object_vec_[i]->vlist_tran_[pass_vec_[pass_idx_]->object_vec_[i]->index_[idx1]].normal);
								pass_vec_[pass_idx_]->object_vec_[i]->vlist_tran_[pass_vec_[pass_idx_]->object_vec_[i]->index_[idx2]].color += 
									mat_vec_[pass_vec_[pass_idx_]->object_vec_[i]->mat_id_[face_index]].CalculateFinalColor(light_vec_[k],
										pass_vec_[pass_idx_]->object_vec_[i]->vlist_tran_[pass_vec_[pass_idx_]->object_vec_[i]->index_[idx2]].pos,
										pass_vec_[pass_idx_]->object_vec_[i]->vlist_tran_[pass_vec_[pass_idx_]->object_vec_[i]->index_[idx2]].normal);
							}
						}
					}
					tri->vertex_list[0] = pass_vec_[pass_idx_]->object_vec_[i]->vlist_tran_[pass_vec_[pass_idx_]->object_vec_[i]->index_[idx0]];
					tri->vertex_list[1] = pass_vec_[pass_idx_]->object_vec_[i]->vlist_tran_[pass_vec_[pass_idx_]->object_vec_[i]->index_[idx1]];
					tri->vertex_list[2] = pass_vec_[pass_idx_]->object_vec_[i]->vlist_tran_[pass_vec_[pass_idx_]->object_vec_[i]->index_[idx2]];
					tri->material = pass_vec_[pass_idx_]->object_vec_[i]->mat_id_[face_index];
					tri->active = true;
					tri->alpha = pass_vec_[pass_idx_]->object_vec_[i]->alpha_;
				}
			}
		}
	}
	return;
}

//转化到透视坐标
void KZPipeLine::TransformWorldToPer(Projection projection) {
	KZMath::KZMatrix44 view, proj;
	main_camera_.GetViewMatrix(view);
	if (projection == Projection::PERSPECTIVE) {
		main_camera_.GetPerspectiveMatrix(proj);
	}
	else if(projection == Projection::ORTHOGONAL)
	{
		main_camera_.GetOrthogonalMatrix(proj);
	}

	for (uint32_t i = 0; i < pass_vec_[pass_idx_]->tri_num_; ++i) {
		for (uint32_t j = 0; j < 3; ++j) {
			pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[j].pos = view * pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[j].pos;
		}
	}
	PolyCulling();
	for (uint32_t i = 0; i < pass_vec_[pass_idx_]->tri_num_; ++i) {
		if (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->active) {
			for (uint32_t j = 0; j < 3; ++j) {
				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[j].pos = proj * pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[j].pos;
				float w_inverse = 1 / pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[j].pos.w_;
				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[j].pos.x_ *= w_inverse;
				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[j].pos.y_ *= w_inverse;
				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[j].pos.z_ *= w_inverse;
			}
		}
	}
}

//转化到视口坐标
void KZPipeLine::TransformPerToViewPort() {
	float alpha = 0.5f * view_width_ - 0.5f;
	float beta = 0.5f * view_height_ - 0.5f;
	for (uint32_t i = 0; i < pass_vec_[pass_idx_]->tri_num_; ++i) {
		if (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->active) {
			for (uint32_t j = 0; j < 3; ++j) {
				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[j].pos.x_ = alpha + alpha * pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[j].pos.x_;
				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[j].pos.y_ = beta - beta * pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[j].pos.y_;
			}
		}
	}
}

//每帧更新
void KZPipeLine::FrameUpdate() {
	DWORD start = ::GetTickCount();
	
	fill_n(z_buffer_, view_width_ * view_height_, 2.0f);
	memset(frame_buffer_, 0, view_height_ * view_width_ * 3 * sizeof(unsigned char));
	
	for (pass_idx_ = 0; pass_idx_ < pass_num_; ++pass_idx_) {
		////初始化
		pass_vec_[pass_idx_]->tri_num_ = 0;
		/*KZMath::KZMatrix44 matrix;
		matrix.RotationZ(1);
		object_vec_[0].RotationMatrix(matrix);*/

		/*KZMath::KZQuat quat;
		quat.SetFromVector3DTheta(KZMath::KZVector3D(0, 0, 1), 1);
		object_vec_[0].RotationQuat(quat);*/

		//固定管线
		TransformModelToWorld();
		SceneManageCulling();
		TransformWorldToPer();
		TransformPerToViewPort();
	}

	for (pass_idx_ = 0; pass_idx_ < pass_num_; ++pass_idx_) {
		RasterizationDepthTest();
	}

	SwapBuffer();
	DWORD delta_time = ::GetTickCount() - start;
	record_time_ += delta_time;
	++fps_count_;
}

//浮点数版本光栅化
void KZPipeLine::RasterizationDepthTest() {
	for (uint32_t i = 0; i < pass_vec_[pass_idx_]->tri_num_; ++i) {
		// 判断三角形是否active，即是否被裁剪掉
		if (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->active) {
			Vertex temp_v(0,0,0);
			Vertex v0 = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[0];
			Vertex v1 = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[1];
			Vertex v2 = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[2];
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
			if (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->material != -1 && mat_vec_[pass_vec_[pass_idx_]->render_list_->tri_list_[i]->material].has_texture_) {
				has_texture = true;
			}

			if (v0.pos.y_ > view_height_ || v2.pos.y_ < 0)
			{
				continue;
			}

			uint32_t floor_v0_y = (uint32_t)(v0.pos.y_ + 0.5f);
			uint32_t floor_v1_y = (uint32_t)(v1.pos.y_ + 0.5f);
			uint32_t floor_v2_y = (uint32_t)(v2.pos.y_ + 0.5f);

			if (floor_v0_y == floor_v1_y && floor_v1_y == floor_v2_y)
			{
				continue;
			}

			//检查是否平底三角形
			if (floor_v1_y == floor_v2_y) {
				//DrawBottomTriFast(v0, v1, v2, has_texture, render_list_->tri_list_[i].material);
				//std::thread thread_bottom(&KZPipeLine::DrawBottomTri, KZPipeLine::GetInstance(), v0, v1, v2, has_texture, render_list_->tri_list_[i].material);
				//thread_bottom.detach();
				DrawBottomTri(v0, v1, v2, has_texture, pass_vec_[pass_idx_]->render_list_->tri_list_[i]->material, pass_vec_[pass_idx_]->render_list_->tri_list_[i]->alpha);
			}
			//检查是否平顶三角形
			else if (floor_v0_y == floor_v1_y) {
				//std::thread thread_top(&KZPipeLine::DrawTopTri, KZPipeLine::GetInstance(), v0, v1, v2, has_texture, render_list_->tri_list_[i].material);
				//thread_top.detach();
				DrawTopTri(v0, v1, v2, has_texture, pass_vec_[pass_idx_]->render_list_->tri_list_[i]->material, pass_vec_[pass_idx_]->render_list_->tri_list_[i]->alpha);
			}
			//一般三角形先做分割
			else
			{
				float one_over_diffy = 1 / (v2.pos.y_ - v0.pos.y_);
				float diffy_v1v0 = (v1.pos.y_ - v0.pos.y_);
				//计算分割的新顶点的pos
				float new_x = v0.pos.x_ + diffy_v1v0 * (v2.pos.x_ - v0.pos.x_) * one_over_diffy;
				float new_y = v1.pos.y_;
				float new_z = v0.pos.z_ + diffy_v1v0 * (v2.pos.z_ - v0.pos.z_) * one_over_diffy;
				//计算分割的新顶点的color
				unsigned char new_r = v0.color.r_ + static_cast<unsigned char>(diffy_v1v0 * (v2.color.r_ - v0.color.r_) * one_over_diffy);
				unsigned char new_g = v0.color.g_ + static_cast<unsigned char>(diffy_v1v0 * (v2.color.g_ - v0.color.g_) * one_over_diffy);
				unsigned char new_b = v0.color.b_ + static_cast<unsigned char>(diffy_v1v0 * (v2.color.b_ - v0.color.b_) * one_over_diffy);
				//计算分割的新顶点的uv
				float new_u = (v0.uv.x_*v0.pos.z_ + diffy_v1v0 * (v2.uv.x_*v2.pos.z_ - v0.uv.x_*v0.pos.z_) * one_over_diffy) / new_z;
				float new_v = (v0.uv.y_*v0.pos.z_ + diffy_v1v0 * (v2.uv.y_*v2.pos.z_ - v0.uv.y_*v0.pos.z_) * one_over_diffy) / new_z;
				//设置新顶点的属性
				Vertex new_vertex(0,0,0);
				new_vertex.pos.Set(new_x, new_y, new_z);
				new_vertex.color.Set(new_r, new_g, new_b);
				new_vertex.uv.x_ = new_u;
				new_vertex.uv.y_ = new_v;
				new_vertex.normal = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->face_normal;
				//将一般三角形分为平顶三角形和平底三角形
				//std::thread thread_bottom(&KZPipeLine::DrawBottomTri, KZPipeLine::GetInstance(), v0, v1, new_vertex, has_texture, render_list_->tri_list_[i].material);
				//thread_bottom.detach();
				//std::thread thread_top(&KZPipeLine::DrawTopTri, KZPipeLine::GetInstance(), new_vertex, v1, v2, has_texture, render_list_->tri_list_[i].material);
				//thread_top.detach();
				if (new_vertex.pos.y_ >= 0)
				{
					DrawBottomTri(v0, v1, new_vertex, has_texture, pass_vec_[pass_idx_]->render_list_->tri_list_[i]->material, pass_vec_[pass_idx_]->render_list_->tri_list_[i]->alpha);
				}
				if (new_vertex.pos.y_ <= view_height_)
				{
					DrawTopTri(new_vertex, v1, v2, has_texture, pass_vec_[pass_idx_]->render_list_->tri_list_[i]->material, pass_vec_[pass_idx_]->render_list_->tri_list_[i]->alpha);
				}
			}
		}
	}
}

//浮点数版本光栅化平底三角形
void KZPipeLine::DrawBottomTri(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id, float alpha) {
	//pos:x关于y的左边斜率，关于y的右边斜率，z关于y的左边斜率，z关于y的右边斜率
	float dx_left = 0.0f, dx_right = 0.0f, dz_left = 0.0f, dz_right = 0.0f;
	//color:r关于y的左边斜率，r关于y的右边斜率，g关于y的左边斜率，g关于y的右边斜率，b关于y的左边斜率，b关于y的右边斜率
	float dr_left = 0.0f, dr_right = 0.0f, dg_left = 0.0f, dg_right = 0.0f, db_left = 0.0f, db_right = 0.0f;
	//uv: s关于y的左边斜率，s关于y的右边斜率，t关于y的左边斜率，t关于y的右边斜率
	float ds_left = 0.0f, ds_right = 0.0f, dt_left = 0.0f, dt_right = 0.0f;
	//光栅化最终y的起始坐标和终止坐标
	uint32_t y_start, y_end;
	//每一行扫描x的起始坐标和终止坐标
	float x_start = 0.0f, x_end = 0.0f;
	//每一行扫描z的起始坐标和终止坐标
	float z_left = 0.0f, z_right = 0.0f;
	//每一行扫描r的起始坐标和终止坐标
	float r_left = 0.0f, r_right = 0.0f;
	//每一行扫描g的起始坐标和终止坐标
	float g_left = 0.0f, g_right = 0.0f;
	//每一行扫描b的起始坐标和终止坐标
	float b_left = 0.0f, b_right = 0.0f;
	//每一行扫描s和t的起始坐标和终止坐标
	float s_left = 0.0f, s_right = 0.0f, t_left = 0.0f, t_right = 0.0f;

	//三角形三个顶点中最小的x的值和最大的x值
	float min_x = 0.0f, max_x = 0.0f;
	//修复的x的起始和终止坐标用于三角形裁剪
	float fix_x_start = 0.0f, fix_x_end = 0.0f;
	//v0的s/z和t/z的值,用于三角形透视投影
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

	if (min_x < 0) {
		min_x = 0;
	}
	if (max_x > view_width_) {
		max_x = (float)view_width_;
	}

	//垂直裁剪
	y_start = v0.pos.y_ < 0 ? 0 : static_cast<uint32_t>(v0.pos.y_ + 0.5f);
	y_end = v2.pos.y_ > view_height_ - 1 ? view_height_ - 1 : static_cast<uint32_t>(v2.pos.y_ + 0.5f);

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

	uint32_t mip_level = 0;
	uint32_t cur_level = 0;
	if (mat_id != -1)
	{
		mip_level = mat_vec_[mat_id].GetMipLevel();
	}
	if (mip_level > 1)
	{
		float ave_z = (v0.pos.z_ + v1.pos.z_ + v2.pos.z_) * 0.33f + 1.0f;
		cur_level = mip_level - 1 - (uint32_t)(mip_level * (ave_z / 2.0f));
	}
	
	uint32_t z_buffer_idx = y_start * view_width_;

	for (uint32_t j = y_start; j <= y_end; ++j) {
		
		//z关于x的斜率
		float delta_zx = 0.0f;
		//r关于x的斜率
		float delta_rx = 0.0f;
		//g关于x的斜率
		float delta_gx = 0.0f;
		//b关于x的斜率
		float delta_bx = 0.0f;

		//s关于x的斜率
		float delta_sx = 0.0f;
		//t关于x的斜率
		float delta_tx = 0.0f;

		//分段线性纹理方法，只计算扫描线端点的准确纹理坐标，线性插值从左到右计算纹理坐标
		float one_over_z_left = 1 / z_left;
		float one_over_z_right = 1 / z_right;
		float s_left_cord = s_left * one_over_z_left;
		float s_right_cord = s_right * one_over_z_right;
		float t_left_cord = t_left * one_over_z_left;
		float t_right_cord = t_right * one_over_z_right;

		if (x_end - x_start != 0) {
			//计算z,r,g,b,s,t相对于x的误差
			float coeff = 1.0f / (x_end - x_start);
			delta_zx = (z_right - z_left) * coeff ;
			delta_rx = (r_right - r_left) * coeff;
			delta_gx = (g_right - g_left) * coeff;
			delta_bx = (b_right - b_left) * coeff;

			//检查是否贴图，避免不必要计算
			if (has_texture) {
				delta_sx = (s_right_cord - s_left_cord) * coeff;
				delta_tx = (t_right_cord - t_left_cord) * coeff;
			}
		}

		//扫描中当前的z值，不直接用left防止裁剪导致的误差
		float z_cur = z_left;
		//扫描中当前的r值，不直接用left防止裁剪导致的误差
		float r_cur = r_left;
		//扫描中当前的g值，不直接用left防止裁剪导致的误差
		float g_cur = g_left;
		//扫描中当前的b值，不直接用left防止裁剪导致的误差
		float b_cur = b_left;
		//扫描中当前的s值，不直接用left防止裁剪导致的误差
		float s_cur = s_left_cord;
		//扫描中当前的t值，不直接用left防止裁剪导致的误差
		float t_cur = t_left_cord;
		//修复的x的起始值
		fix_x_start = x_start;
		//修复的x的终止值
		fix_x_end = x_end;

		//水平裁剪
		if (x_start < min_x) {
			float dx = min_x - x_start;
			//根据x值误差修正z,r,g,b,s,t
			z_cur += dx * delta_zx;
			r_cur += dx * delta_rx;
			g_cur += dx * delta_gx;
			b_cur += dx * delta_bx;
			
			//检查是否贴图，避免不必要计算
			if (has_texture) {
				s_cur += dx * delta_sx;
				t_cur += dx * delta_tx;
			}
			fix_x_start = min_x;
		}
		
		if (x_end < 0) {
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
			z_buffer_idx += view_width_;

			//检查是否贴图，避免不必要计算
			if (has_texture) {
				s_left += ds_left;
				s_right += ds_right;
				t_left += dt_left;
				t_right += dt_right;
			}
			continue;
		}

		if (x_end > max_x) {
			fix_x_end = max_x;
		}

		uint32_t xs = static_cast<uint32_t>(fix_x_start + 0.5f);
		uint32_t xe = static_cast<uint32_t>(fix_x_end + 0.5f);

		for (uint32_t k = xs; k <= xe; ++k) {
			//CVV裁剪z
			if (z_cur >= -1 && z_cur <= 1) {
				//深度缓冲
				uint32_t z_buffer_index = z_buffer_idx + k;
				//uint32_t z_buffer_index = j * view_width_ + k;
				if (z_cur <= z_buffer_[z_buffer_index]) {
					//失败的多线程光栅化
					//mutex_buffer_[z_buffer_index].lock();

					z_buffer_[z_buffer_index] = z_cur;
					Color final_color(static_cast<unsigned char>(r_cur), static_cast<unsigned char>(g_cur), static_cast<unsigned char>(b_cur));
					if (has_texture) {
						/*float one_over_z_cur = 1 / z_cur;
						float s_cord = s_cur * one_over_z_cur;
						float t_cord = t_cur * one_over_z_cur;
						s_cord = s_cord < 0 ? 0 : s_cord > 1 ? 1 : s_cord;
						t_cord = t_cord < 0 ? 0 : t_cord > 1 ? 1 : t_cord;
						final_color = final_color * mat_vec_[mat_id].GetTextureColor(s_cord, t_cord);*/
						float s_cord = s_cur < 0 ? 0 : s_cur > 1 ? 1 : s_cur;
						float t_cord = t_cur < 0 ? 0 : t_cur > 1 ? 1 : t_cur;
						Color texture_color;
						mat_vec_[mat_id].GetTextureColor(s_cord, t_cord, cur_level, texture_color);
						final_color = final_color * texture_color;
					}
					//draw point
					//uint32_t dist = j * view_width_ * 3 + k * 3;
					uint32_t dist = z_buffer_index * 3;
					if (alpha == 1.0f && final_color.a_ != 0) 
					{
						frame_buffer_[dist] = final_color.b_;//Blue
						frame_buffer_[dist + 1] = final_color.g_;//Green
						frame_buffer_[dist + 2] = final_color.r_;//Red 
					}
					else if(alpha != 1.0f && final_color.a_ != 0)
					{
						frame_buffer_[dist] = (unsigned char)(alpha * final_color.b_ + (1 - alpha) * frame_buffer_[dist]);//Blue
						frame_buffer_[dist + 1] = (unsigned char)(alpha * final_color.g_ + (1 - alpha) * frame_buffer_[dist + 1]);//Green
						frame_buffer_[dist + 2] = (unsigned char)(alpha * final_color.r_ + (1 - alpha) * frame_buffer_[dist + 2]);//Red 
					}

					//失败的多线程光栅化
					//mutex_buffer_[z_buffer_index].unlock();
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

		z_buffer_idx += view_width_;
		//检查是否贴图，避免不必要计算
		if (has_texture) {
			s_left += ds_left;
			s_right += ds_right;
			t_left += dt_left;
			t_right += dt_right;
		}
	}
}

//浮点数版本光栅化平顶三角形
void KZPipeLine::DrawTopTri(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool has_texture, int32_t mat_id, float alpha) {
	//pos:x关于y的左边斜率，关于y的右边斜率，z关于y的左边斜率，z关于y的右边斜率
	float dx_left = 0.0f, dx_right = 0.0f, dz_left = 0.0f, dz_right = 0.0f;
	//color:r关于y的左边斜率，r关于y的右边斜率，g关于y的左边斜率，g关于y的右边斜率，b关于y的左边斜率，b关于y的右边斜率
	float dr_left = 0.0f, dr_right = 0.0, dg_left = 0.0f, dg_right = 0.0f, db_left = 0.0f, db_right = 0.0f;
	//uv: s关于y的左边斜率，s关于y的右边斜率，t关于y的左边斜率，t关于y的右边斜率
	float ds_left = 0.0f, ds_right = 0.0f, dt_left = 0.0f, dt_right = 0.0f;
	//光栅化最终y的起始坐标和终止坐标
	uint32_t y_start, y_end;
	//每一行扫描x的起始坐标和终止坐标
	float x_start = 0.0f, x_end = 0.0f;
	//每一行扫描z的起始坐标和终止坐标
	float z_left = 0.0f, z_right = 0.0f;
	//每一行扫描r的起始坐标和终止坐标
	float r_left = 0.0f, r_right = 0.0f;
	//每一行扫描g的起始坐标和终止坐标
	float g_left = 0.0f, g_right = 0.0f;
	//每一行扫描b的起始坐标和终止坐标
	float b_left = 0.0f, b_right = 0.0f;
	//每一行扫描s和t的起始坐标和终止坐标
	float s_left = 0.0f, s_right = 0.0f, t_left = 0.0f, t_right = 0.0f;

	//三角形三个顶点中最小的x的值和最大的x值
	float min_x = 0.0f, max_x = 0.0f;
	//修复的x的起始和终止坐标用于三角形裁剪
	float fix_x_start = 0.0f, fix_x_end = 0.0f;
	//v0的s/z和t/z的值,用于三角形透视投影
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
		if (v1.pos.x_ < v2.pos.x_) {
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

	if (min_x < 0) {
		min_x = 0;
	}
	if (max_x > view_width_) {
		max_x = (float)view_width_;
	}

	//垂直裁剪
	y_end = v0.pos.y_ < 0 ? 0 : static_cast<uint32_t>(v0.pos.y_ + 0.5f);
	y_start = v2.pos.y_ > view_height_ - 1 ? view_height_ - 1 : static_cast<uint32_t>(v2.pos.y_ + 0.5f);

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

	uint32_t mip_level = 0;
	uint32_t cur_level = 0;
	if (mat_id != -1)
	{
		mip_level = mat_vec_[mat_id].GetMipLevel();
	}
	if (mip_level > 1)
	{
		float ave_z = (v0.pos.z_ + v1.pos.z_ + v2.pos.z_) * 0.33f + 1.0f;
		cur_level = mip_level - 1 - (uint32_t)(mip_level * (ave_z / 2.0f));
	}

	uint32_t z_buffer_idx = y_start * view_width_;

	for (uint32_t j = y_start; j > y_end; --j) {
		//z关于x的斜率
		float delta_zx = 0.0f;
		//r关于x的斜率
		float delta_rx = 0.0f;
		//g关于x的斜率
		float delta_gx = 0.0f;
		//b关于x的斜率
		float delta_bx = 0.0f;

		//s关于x的斜率
		float delta_sx = 0.0f;
		//t关于x的斜率
		float delta_tx = 0.0f;

		//分段线性纹理方法，只计算扫描线端点的准确纹理坐标，线性插值从左到右计算纹理坐标
		float one_over_z_left = 1 / z_left;
		float one_over_z_right = 1 / z_right;
		float s_left_cord = s_left * one_over_z_left;
		float s_right_cord = s_right * one_over_z_right;
		float t_left_cord = t_left * one_over_z_left;
		float t_right_cord = t_right * one_over_z_right;

		if (x_end - x_start != 0) {
			float coeff = 1.0f / (x_end - x_start);
			//计算z,r,g,b,s,t相对于x的梯度
			delta_zx = (z_right - z_left) * coeff;
			delta_rx = (r_right - r_left) * coeff;
			delta_gx = (g_right - g_left) * coeff;
			delta_bx = (b_right - b_left) * coeff;

			//检查是否贴图，避免不必要计算
			if (has_texture) {
				delta_sx = (s_right_cord - s_left_cord) * coeff;
				delta_tx = (t_right_cord - t_left_cord) * coeff;
			}
		}
		//扫描中当前的z值，不直接用left防止裁剪导致的误差
		float z_cur = z_left;
		//扫描中当前的r值，不直接用left防止裁剪导致的误差
		float r_cur = r_left;
		//扫描中当前的g值，不直接用left防止裁剪导致的误差
		float g_cur = g_left;
		//扫描中当前的b值，不直接用left防止裁剪导致的误差
		float b_cur = b_left;
		//扫描中当前的s值，不直接用left防止裁剪导致的误差
		float s_cur = s_left_cord;
		//扫描中当前的t值，不直接用left防止裁剪导致的误差
		float t_cur = t_left_cord;
		//修复的x的起始值
		fix_x_start = x_start;
		//修复的x的终止值
		fix_x_end = x_end;
		//水平裁剪
		if (x_start < min_x) {
			float dx = min_x - x_start;
			//根据x值误差修正z,r,g,b,s,t
			z_cur += dx * delta_zx;
			r_cur += dx * delta_rx;
			g_cur += dx * delta_gx;
			b_cur += dx * delta_bx;

			//检查是否贴图，避免不必要计算
			if (has_texture) {
				s_cur += dx * delta_sx;
				t_cur += dx * delta_tx;
			}
			fix_x_start = min_x;
		}

		if (x_end < 0) {
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
			z_buffer_idx -= view_width_;

			//检查是否贴图，避免不必要计算
			if (has_texture) {
				s_left += ds_left;
				s_right += ds_right;
				t_left += dt_left;
				t_right += dt_right;
			}
			continue;
		}

		if (fix_x_end > max_x) {
			fix_x_end = max_x;
		}

		uint32_t xs = static_cast<uint32_t>(fix_x_start + 0.5f);
		uint32_t xe = static_cast<uint32_t>(fix_x_end + 0.5f);
		for (uint32_t k = xs; k <= xe; ++k) {
			//CVV裁剪z
			if (z_cur >= -1 && z_cur <= 1) {
				//uint32_t z_buffer_index = j * view_width_ + k;
				uint32_t z_buffer_index = z_buffer_idx + k;
				//深度缓冲检测
				if (z_cur <= z_buffer_[z_buffer_index]) {
					//失败的多线程光栅化
					//mutex_buffer_[z_buffer_index].lock();

					z_buffer_[z_buffer_index] = z_cur;
					Color final_color(static_cast<unsigned char>(r_cur), static_cast<unsigned char>(g_cur), static_cast<unsigned char>(b_cur));
					if (has_texture) {
						/*float one_over_z_cur = 1 / z_cur;
						float s_cord = s_cur * one_over_z_cur;
						float t_cord = t_cur * one_over_z_cur;
						s_cord = s_cord < 0 ? 0 : s_cord > 1 ? 1 : s_cord;
						t_cord = t_cord < 0 ? 0 : t_cord > 1 ? 1 : t_cord;
						final_color = final_color * mat_vec_[mat_id].GetTextureColor(s_cord, t_cord);*/
						float s_cord = s_cur < 0 ? 0 : s_cur > 1 ? 1 : s_cur;
						float t_cord = t_cur < 0 ? 0 : t_cur > 1 ? 1 : t_cur;
						Color texture_color;
						mat_vec_[mat_id].GetTextureColor(s_cord, t_cord, cur_level, texture_color);
						final_color = final_color * texture_color;
					}
					//draw point
					//uint32_t dist = j * view_width_ * 3 + k * 3;
					uint32_t dist = z_buffer_index * 3;
					//draw point

					if (alpha == 1.0f && final_color.a_ != 0)
					{
						frame_buffer_[dist] = final_color.b_;//Blue
						frame_buffer_[dist + 1] = final_color.g_;//Green
						frame_buffer_[dist + 2] = final_color.r_;//Red 
					}
					else if(alpha != 1.0f && final_color.a_ != 0)
					{
						frame_buffer_[dist] = (unsigned char)(alpha * final_color.b_ + (1 - alpha) * frame_buffer_[dist]);//Blue
						frame_buffer_[dist + 1] = (unsigned char)(alpha * final_color.g_ + (1 - alpha) * frame_buffer_[dist + 1]);//Green
						frame_buffer_[dist + 2] = (unsigned char)(alpha * final_color.r_ + (1 - alpha) * frame_buffer_[dist + 2]);//Red 
					}

					//失败的多线程光栅化
					//mutex_buffer_[z_buffer_index].unlock();
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
		z_buffer_idx -= view_width_;

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
	if (record_time_ != 0) {
		fps_ = 1000.0f * fps_count_ / record_time_;
	}
	else
	{
		fps_ = static_cast<float>(record_time_);
	}
	char window_title[64];
	sprintf_s(window_title, "Engine FPS:%f", fps_);
	WCHAR w_window_title[64];
	MultiByteToWideChar(CP_ACP, 0, window_title, (int)strlen(window_title) + 1, w_window_title, sizeof(w_window_title) / sizeof(w_window_title[0]));
	SetWindowText(hwnd_, w_window_title);
	fps_ = 0;
	record_time_ = 0;
	fps_count_ = 0;
}

//增加材质
uint32_t KZPipeLine::AddMaterial(const KZEngine::KZMaterial& new_mat) {
	uint32_t mat_num = (uint32_t)mat_vec_.size();
	uint32_t index;
	//遍历材质数组防止重复材质
	for (index = 0; index < mat_num; ++index) {
		if (mat_vec_[index].GetHashCode() == new_mat.GetHashCode()) {
			return index;
		}
	}

	if (index == mat_num) {
		mat_vec_.push_back(new_mat);
	}
	return index;
}

//改变光状态
void KZPipeLine::ChangeLight(uint32_t light_index) {
	light_active_vec_[light_index] = !(light_active_vec_[light_index]);
}

//双缓冲交换
void KZPipeLine::SwapBuffer() {
	RECT rc;
	GetClientRect(hwnd_, &rc);
	uint32_t w = KZEngine::KZPipeLine::GetInstance()->GetWindowWidth();
	uint32_t h = KZEngine::KZPipeLine::GetInstance()->GetWindowHeight();
	//创建GDI+掩码位图（画布）
	Gdiplus::Bitmap bitmap(w, h, 3 * w, PixelFormat24bppRGB, (BYTE*)KZEngine::KZPipeLine::GetInstance()->GetFrameBuffer());
	Gdiplus::Graphics bmp_graphics(&bitmap);
	bmp_graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	HDC hdc = GetDC(hwnd_);
	Gdiplus::Graphics graphics(hdc);
	Gdiplus::CachedBitmap cachedBmp(&bitmap, &graphics);
	graphics.DrawCachedBitmap(&cachedBmp, 0, 0);
	ReleaseDC(hwnd_, hdc);
}

//定点数版本光栅化与深度测试
void KZPipeLine::RasterizationDepthTestFast() {
	for (uint32_t i = 0; i <  pass_vec_[pass_idx_]->tri_num_; ++i) {
		// 判断三角形是否active，即是否被裁剪掉
		if (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->active) {
			Vertex temp_v(0, 0, 0);
			Vertex vertex0 = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[0];
			Vertex vertex1 = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[1];
			Vertex vertex2 = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[2];
			float dx_left = 0.0f, dx_right = 0.0f, dz_left = 0.0f, dz_right = 0.0f;

			//按照y值升序vertex0,vertex1,vertex2
			if (vertex0.pos.y_ > vertex1.pos.y_) {
				temp_v = vertex0;
				vertex0 = vertex1;
				vertex1 = temp_v;
			}

			if (vertex1.pos.y_ > vertex2.pos.y_) {
				temp_v = vertex1;
				vertex1 = vertex2;
				vertex2 = temp_v;
				if (vertex0.pos.y_ > vertex1.pos.y_) {
					temp_v = vertex0;
					vertex0 = vertex1;
					vertex1 = temp_v;
				}
			}

			if (vertex0.pos.y_ > view_height_ || vertex2.pos.y_ < 0)
			{
				continue;
			}

			uint32_t floor_v0_y = (uint32_t)(vertex0.pos.y_ + 0.5f);
			uint32_t floor_v1_y = (uint32_t)(vertex1.pos.y_ + 0.5f);
			uint32_t floor_v2_y = (uint32_t)(vertex2.pos.y_ + 0.5f);

			if (floor_v0_y == floor_v1_y && floor_v1_y == floor_v2_y)
			{
				continue;
			}

			bool has_texture = false;
			if (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->material != -1 && mat_vec_[pass_vec_[pass_idx_]->render_list_->tri_list_[i]->material].has_texture_) {
				has_texture = true;
			}

			//检查是否平底三角形
			if (floor_v1_y == floor_v2_y) {
				DrawBottomTriFast(vertex0, vertex1, vertex2, has_texture, pass_vec_[pass_idx_]->render_list_->tri_list_[i]->material, pass_vec_[pass_idx_]->render_list_->tri_list_[i]->alpha);
			}
			//检查是否平顶三角形
			else if (floor_v0_y == floor_v1_y) {
				DrawTopTriFast(vertex0, vertex1, vertex2, has_texture, pass_vec_[pass_idx_]->render_list_->tri_list_[i]->material, pass_vec_[pass_idx_]->render_list_->tri_list_[i]->alpha);
			}
			//一般三角形先做分割
			else
			{
				int32_t decimal_bit = 12;
				float decimal_bit_float = 4096.0;
				int32_t x0 = (int32_t)(vertex0.pos.x_ * decimal_bit_float + 0.5f);
				int32_t x1 = (int32_t)(vertex1.pos.x_ * decimal_bit_float + 0.5f);
				int32_t x2 = (int32_t)(vertex2.pos.x_ * decimal_bit_float + 0.5f);
				int32_t y0 = (int32_t)(vertex0.pos.y_ * decimal_bit_float + 0.5f);
				int32_t y1 = (int32_t)(vertex1.pos.y_ * decimal_bit_float + 0.5f);
				int32_t y2 = (int32_t)(vertex2.pos.y_ * decimal_bit_float + 0.5f);
				int32_t z0 = (int32_t)(vertex0.pos.z_ * decimal_bit_float + 0.5f);
				int32_t z1 = (int32_t)(vertex1.pos.z_ * decimal_bit_float + 0.5f);
				int32_t z2 = (int32_t)(vertex2.pos.z_ * decimal_bit_float + 0.5f);
				/*KZMath::KZFix32 x0(vertex0.pos.x_), y0(vertex0.pos.y_), z0(vertex0.pos.z_);
				KZMath::KZFix32 x1(vertex1.pos.x_), y1(vertex1.pos.y_), z1(vertex1.pos.z_);
				KZMath::KZFix32 x2(vertex2.pos.x_), y2(vertex2.pos.y_), z2(vertex2.pos.z_);*/

				int32_t diff_y1y0 = y1 - y0;
				int32_t diff_y2y0 = y2 - y0;

				/*KZMath::KZFix32 diff_y1y0(y1 - y0);
				KZMath::KZFix32 diff_y2y0(y2 - y0);*/

				int64_t temp_cache = (int64_t)diff_y1y0;
				temp_cache *= (x2 - x0);
				int32_t result = (int32_t)(temp_cache >> decimal_bit);
				temp_cache = result;
				temp_cache <<= decimal_bit;
				temp_cache /= diff_y2y0;
				result = (int32_t)temp_cache;
				int32_t new_fix_x = x0 + result;
				/*KZMath::KZFix32 new_fix_x = x0 + diff_y1y0 * (x2 - x0) / diff_y2y0;*/

				temp_cache = (int64_t)diff_y1y0;
				temp_cache *= (int64_t)(z2 - z0);
				result = (int32_t)(temp_cache >> decimal_bit);
				temp_cache = result;
				temp_cache <<= decimal_bit;
				temp_cache /= diff_y2y0;
				result = (int32_t)temp_cache;
				int32_t new_fix_z = z0 + result;
				/*KZMath::KZFix32 new_fix_z = z0 + diff_y1y0 * (z2 - z0) / diff_y2y0;*/

				float new_x = (float)new_fix_x / decimal_bit_float;
				float new_y = vertex1.pos.y_;
				float new_z = (float)new_fix_z / decimal_bit_float;

				int32_t r0 = ((int32_t)vertex0.color.r_) << decimal_bit;
				int32_t r1 = ((int32_t)vertex1.color.r_) << decimal_bit;
				int32_t r2 = ((int32_t)vertex2.color.r_) << decimal_bit;
				int32_t g0 = ((int32_t)vertex0.color.g_) << decimal_bit;
				int32_t g1 = ((int32_t)vertex1.color.g_) << decimal_bit;
				int32_t g2 = ((int32_t)vertex2.color.g_) << decimal_bit;
				int32_t b0 = ((int32_t)vertex0.color.b_) << decimal_bit;
				int32_t b1 = ((int32_t)vertex1.color.b_) << decimal_bit;
				int32_t b2 = ((int32_t)vertex2.color.b_) << decimal_bit;

				/*KZMath::KZFix32 r0((int32_t)vertex0.color.r_), g0((int32_t)vertex0.color.g_), b0((int32_t)vertex0.color.b_);
				KZMath::KZFix32 r1((int32_t)vertex1.color.r_), g1((int32_t)vertex1.color.g_), b1((int32_t)vertex1.color.b_);
				KZMath::KZFix32 r2((int32_t)vertex2.color.r_), g2((int32_t)vertex2.color.g_), b2((int32_t)vertex2.color.b_);*/

				temp_cache = (int64_t)diff_y1y0;
				temp_cache *= (r2 - r0);
				result = (int32_t)(temp_cache >> decimal_bit);
				temp_cache = result;
				temp_cache <<= decimal_bit;
				temp_cache /= diff_y2y0;
				result = (int32_t)temp_cache;
				unsigned char new_r = vertex0.color.r_ + (result >> decimal_bit);
				temp_cache = (int64_t)diff_y1y0;
				temp_cache *= (g2 - g0);
				result = (int32_t)(temp_cache >> decimal_bit);
				temp_cache = result;
				temp_cache <<= decimal_bit;
				temp_cache /= diff_y2y0;
				result = (int32_t)temp_cache;
				unsigned char new_g = vertex0.color.g_ + (result >> decimal_bit);
				temp_cache = (int64_t)diff_y1y0;
				temp_cache *= (b2 - b0);
				result = (int32_t)(temp_cache >> decimal_bit);
				temp_cache = result;
				temp_cache <<= decimal_bit;
				temp_cache /= diff_y2y0;
				result = (int32_t)temp_cache;
				unsigned char new_b = vertex0.color.b_ + (result >> decimal_bit);
				/*unsigned char new_r = vertex0.color.r_ + (diff_y1y0 * (r2 - r0) / diff_y2y0).GetIntValue();
				unsigned char new_g = vertex0.color.g_ + (diff_y1y0 * (g2 - g0) / diff_y2y0).GetIntValue();
				unsigned char new_b = vertex0.color.b_ + (diff_y1y0 * (b2 - b0) / diff_y2y0).GetIntValue();*/

				float one_over_diffy = 1 / (vertex2.pos.y_ - vertex0.pos.y_);
				float diffy_v1v0 = (vertex1.pos.y_ - vertex0.pos.y_);
				float new_u = (vertex0.uv.x_*vertex0.pos.z_ + diffy_v1v0 * (vertex2.uv.x_*vertex2.pos.z_ - vertex0.uv.x_*vertex0.pos.z_) * one_over_diffy) / new_z;
				float new_v = (vertex0.uv.y_*vertex0.pos.z_ + diffy_v1v0 * (vertex2.uv.y_*vertex2.pos.z_ - vertex0.uv.y_*vertex0.pos.z_) * one_over_diffy) / new_z;

				Vertex new_vertex(0, 0, 0);
				new_vertex.pos.Set(new_x, new_y, new_z);
				new_vertex.color.Set(new_r, new_g, new_b);
				new_vertex.uv.x_ = new_u;
				new_vertex.uv.y_ = new_v;
				new_vertex.normal = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->face_normal;
				DrawBottomTriFast(vertex0, vertex1, new_vertex, has_texture, pass_vec_[pass_idx_]->render_list_->tri_list_[i]->material, pass_vec_[pass_idx_]->render_list_->tri_list_[i]->alpha);
				DrawTopTriFast(new_vertex, vertex1, vertex2, has_texture, pass_vec_[pass_idx_]->render_list_->tri_list_[i]->material, pass_vec_[pass_idx_]->render_list_->tri_list_[i]->alpha);
			}
		}
	}
}

//定点数版本光栅化平底三角形
//注释部分为用数学库实现，目前全部用c风格手动写出来，避免函数调用的消耗，性能较浮点数版本略有提升，但是存在某些视觉误差
void KZPipeLine::DrawBottomTriFast(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2, bool has_texture, int32_t mat_id, float alpha) {
	//定点数小数位数，整数倍数2^12
	int32_t decimal_bit = 12;
	//定点数浮点数倍数
	float decimal_bit_float = 4096.0;

	//获取三角形三个顶点对应x,y,z的定点数值
	int32_t x0 = (int32_t)(vertex0.pos.x_ * decimal_bit_float + 0.5f);
	int32_t x1 = (int32_t)(vertex1.pos.x_ * decimal_bit_float + 0.5f);
	int32_t x2 = (int32_t)(vertex2.pos.x_ * decimal_bit_float + 0.5f);
	int32_t y0 = (int32_t)(vertex0.pos.y_ * decimal_bit_float + 0.5f);
	int32_t y1 = (int32_t)(vertex1.pos.y_ * decimal_bit_float + 0.5f);
	int32_t y2 = (int32_t)(vertex2.pos.y_ * decimal_bit_float + 0.5f);
	int32_t z0 = (int32_t)(vertex0.pos.z_ * decimal_bit_float + 0.5f);
	int32_t z1 = (int32_t)(vertex1.pos.z_ * decimal_bit_float + 0.5f);
	int32_t z2 = (int32_t)(vertex2.pos.z_ * decimal_bit_float + 0.5f);
	/*KZMath::KZFix32 x0(vertex0.pos.x_), y0(vertex0.pos.y_), z0(vertex0.pos.z_);
	KZMath::KZFix32 x1(vertex1.pos.x_), y1(vertex1.pos.y_), z1(vertex1.pos.z_);
	KZMath::KZFix32 x2(vertex2.pos.x_), y2(vertex2.pos.y_), z2(vertex2.pos.z_);*/

	//获取三角形三个顶点对应r,g,b的定点数值
	int32_t r0 = ((int32_t)vertex0.color.r_) << decimal_bit;
	int32_t r1 = ((int32_t)vertex1.color.r_) << decimal_bit;
	int32_t r2 = ((int32_t)vertex2.color.r_) << decimal_bit;
	int32_t g0 = ((int32_t)vertex0.color.g_) << decimal_bit;
	int32_t g1 = ((int32_t)vertex1.color.g_) << decimal_bit;
	int32_t g2 = ((int32_t)vertex2.color.g_) << decimal_bit;
	int32_t b0 = ((int32_t)vertex0.color.b_) << decimal_bit;
	int32_t b1 = ((int32_t)vertex1.color.b_) << decimal_bit;
	int32_t b2 = ((int32_t)vertex2.color.b_) << decimal_bit;
	/*KZMath::KZFix32 r0((int32_t)vertex0.color.r_), g0((int32_t)vertex0.color.g_), b0((int32_t)vertex0.color.b_);
	KZMath::KZFix32 r1((int32_t)vertex1.color.r_), g1((int32_t)vertex1.color.g_), b1((int32_t)vertex1.color.b_);
	KZMath::KZFix32 r2((int32_t)vertex2.color.r_), g2((int32_t)vertex2.color.g_), b2((int32_t)vertex2.color.b_);*/

	//x对于y的左边斜率，x对于y的右边斜率，z对于y的左边斜率，z对于y的右边斜率
	int32_t dxdyl = 0, dxdyr = 0, dzdyl = 0, dzdyr = 0;
	//r对于y的左边斜率，r对于y的右边斜率，g对于y的左边斜率，g对于y的右边斜率，b对于y的左边斜率，b对于y的右边斜率
	int32_t drdyl = 0, drdyr = 0, dgdyl = 0, dgdyr = 0, dbdyl = 0, dbdyr = 0;
	/*KZMath::KZFix32 dxdyl, dxdyr, dzdyl, dzdyr;
	KZMath::KZFix32 drdyl, drdyr, dgdyl, dgdyr, dbdyl, dbdyr;*/

	//pos: 每条扫描线x的起始值，x的终止值，z的起始值，z的终止值
	int32_t x_start = 0, x_end = 0, z_left = 0, z_right = 0;
	//color: 每条扫描线r的起始值，r的终止值，g的起始值，g的终止值，b的起始值，b的终止值
	int32_t r_left = 0, r_right = 0, g_left = 0, g_right = 0, b_left = 0, b_right = 0;
	/*KZMath::KZFix32 x_start, x_end, z_left, z_right;
	KZMath::KZFix32 r_left, r_right, g_left, g_right, b_left, b_right;*/

	//uv: 每条扫描线s的起始值,s的终止值，t的起始值，t的终止值
	float s_left = 0.0f, s_right = 0.0f, t_left = 0.0f, t_right = 0.0f;
	//顶点v0的s/z 和 t/z的值
	float v0_s_overz = 0.0f, v0_t_overz = 0.0f;
	//s对于y的左边斜率，s对于y的右边斜率，t对于y的左边斜率，t对于y的右边斜率
	float ds_left = 0.0f, ds_right = 0.0f, dt_left = 0.0f, dt_right = 0.0f;

	//修正的x的起始值,x的终止值，防止水平裁剪的误差
	int32_t fix_x_start = 0, fix_x_end = 0;
	/*KZMath::KZFix32 fix_x_start, fix_x_end;*/

	//三角形三个顶点最大的x值和最小的x值，用于约束扫描线的x的值，防止斜率造成的误差
	int32_t max_x = 0, min_x = 0;
	/*KZMath::KZFix32 max_x, min_x;*/
	//扫描线的起始值和终止值
	uint32_t y_start = 0, y_end = 0;
	//用于临时存储，定点数的运算需要
	int64_t temp_cache = 0;
	//用于临时存储，定点数的运算需要
	int32_t result = 0;

	//最大颜色值255的定点数表示
	int32_t max_color_value = 255 << decimal_bit;
	//检查是否贴图，避免不必要计算
	if (has_texture) {
		v0_s_overz = vertex0.uv.x_ * vertex0.pos.z_;
		v0_t_overz = vertex0.uv.y_ * vertex0.pos.z_;
	}

	//计算左右两边斜率
	if (x2 < x1) {
		int32_t y_diff_left(y0 - y2);
		int32_t y_diff_right(y0 - y1);
		/*KZMath::KZFix32 y_diff_left(y0 - y2);
		KZMath::KZFix32 y_diff_right(y0- y1);*/

		//求x,z的y的左右梯度
		temp_cache = (x0 - x2);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_left;
		dxdyl = (int32_t)temp_cache;
		//
		temp_cache = (x0 - x1);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_right;
		dxdyr = (int32_t)temp_cache;
		//
		temp_cache = (z0 - z2);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_left;
		dzdyl = (int32_t)temp_cache;
		//
		temp_cache = (z0 - z1);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_right;
		dzdyr = (int32_t)temp_cache;
		//
		/*dxdyl = (x0 - x2) / y_diff_left;
		dxdyr = (x0 - x1) / y_diff_right;
		dzdyl = (z0 - z2) / y_diff_left;
		dzdyr = (z0 - z1) / y_diff_right;*/

		//求颜色r,g,b的y的左右梯度
		temp_cache = (r0 - r2);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_left;
		drdyl = (int32_t)temp_cache;
		//
		temp_cache = (r0 - r1);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_right;
		drdyr = (int32_t)temp_cache;
		//
		temp_cache = (g0 - g2);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_left;
		dgdyl = (int32_t)temp_cache;
		//
		temp_cache = (g0 - g1);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_right;
		dgdyr = (int32_t)temp_cache;
		//
		temp_cache = (b0 - b2);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_left;
		dbdyl = (int32_t)temp_cache;
		//
		temp_cache = (b0 - b1);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_right;
		dbdyr = (int32_t)temp_cache;
		//
		/*drdyl = (r0 - r2) / y_diff_left;
		drdyr = (r0 - r1) / y_diff_right;
		dgdyl = (g0 - g2) / y_diff_left;
		dgdyr = (g0 - g1) / y_diff_right;
		dbdyl = (b0 - b2) / y_diff_left;
		dbdyr = (b0 - b1) / y_diff_right;*/

		//检查是否贴图，避免不必要计算
		if (has_texture) {
			float one_over_y_diff_left_float = 1 / (vertex0.pos.y_ - vertex2.pos.y_);
			float one_over_y_diff_right_float = 1 / (vertex0.pos.y_ - vertex1.pos.y_);
			//求纹理s/z,t/z的y的左右梯度
			ds_left = (v0_s_overz - vertex2.uv.x_ * vertex2.pos.z_) * one_over_y_diff_left_float;
			ds_right = (v0_s_overz - vertex1.uv.x_ * vertex1.pos.z_) * one_over_y_diff_right_float;
			dt_left = (v0_t_overz - vertex2.uv.y_ * vertex2.pos.z_) * one_over_y_diff_left_float;
			dt_right = (v0_t_overz - vertex1.uv.y_ * vertex1.pos.z_) * one_over_y_diff_right_float;
		}

		//计算x最大值最小值,修正误差
		if (x1 < x0) {
			min_x = x2;
			max_x = x0;
		}
		else
		{
			max_x = x1;
			if (x0 < x2) {
				min_x = x0;
			}
			else
			{
				min_x = x2;
			}
		}
	}
	else
	{
		int32_t y_diff_left(y0 - y1);
		int32_t y_diff_right(y0 - y2);
		/*KZMath::KZFix32 y_diff_left(y0 - y1);
		KZMath::KZFix32 y_diff_right(y0 - y2);*/

		//求x,z的y的左右梯度
		temp_cache = (x0 - x1);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_left;
		dxdyl = (int32_t)temp_cache;
		//
		temp_cache = (x0 - x2);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_right;
		dxdyr = (int32_t)temp_cache;
		//
		temp_cache = (z0 - z1);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_left;
		dzdyl = (int32_t)temp_cache;
		//
		temp_cache = (z0 - z2);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_right;
		dzdyr = (int32_t)temp_cache;
		//
		/*dxdyl = (x0 - x1) / y_diff_left;
		dxdyr = (x0 - x2) / y_diff_right;
		dzdyl = (z0 - z1) / y_diff_left;
		dzdyr = (z0 - z2) / y_diff_right;*/

		//求颜色r,g,b的y的左右梯度
		temp_cache = (r0 - r1);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_left;
		drdyl = (int32_t)temp_cache;
		//
		temp_cache = (r0 - r2);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_right;
		drdyr = (int32_t)temp_cache;
		//
		temp_cache = (g0 - g1);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_left;
		dgdyl = (int32_t)temp_cache;
		//
		temp_cache = (g0 - g2);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_right;
		dgdyr = (int32_t)temp_cache;
		//
		temp_cache = (b0 - b1);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_left;
		dbdyl = (int32_t)temp_cache;
		//
		temp_cache = (b0 - b2);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_right;
		dbdyr = (int32_t)temp_cache;
		/*drdyl = (r0 - r1) / y_diff_left;
		drdyr = (r0 - r2) / y_diff_right;
		dgdyl = (g0 - g1) / y_diff_left;
		dgdyr = (g0 - g2) / y_diff_right;
		dbdyl = (b0 - b1) / y_diff_left;
		dbdyr = (b0 - b2) / y_diff_right;*/

		//检查是否贴图，避免不必要计算
		if (has_texture) {
			float one_over_y_diff_left_float = 1 / (vertex0.pos.y_ - vertex1.pos.y_);
			float one_over_y_diff_right_float = 1 / (vertex0.pos.y_ - vertex2.pos.y_);
			//求纹理s/z,t/z的y的左右梯度
			ds_left = (v0_s_overz - vertex1.uv.x_ * vertex1.pos.z_) * one_over_y_diff_left_float;
			ds_right = (v0_s_overz - vertex2.uv.x_ * vertex2.pos.z_) * one_over_y_diff_right_float;
			dt_left = (v0_t_overz - vertex1.uv.y_ * vertex1.pos.z_) * one_over_y_diff_left_float;
			dt_right = (v0_t_overz - vertex2.uv.y_ * vertex2.pos.z_) * one_over_y_diff_right_float;
		}

		//计算x最大值最小值,修正误差
		if (x2 < x0) {
			min_x = x1;
			max_x = x0;
		}
		else
		{
			max_x = x2;
			if (x0 < x1) {
				min_x = x0;
			}
			else
			{
				min_x = x1;
			}
		}
	}

	if (min_x < 0) {
		min_x = 0;
	}
	/*KZMath::KZFix32 view_width_fix32((int32_t)view_width_);*/
	result = view_width_ << decimal_bit;
	if (max_x > result) {
		max_x = result;
	}
	/*if (max_x > view_width_fix32) {
		max_x = view_width_fix32;
	}*/

	//垂直裁剪
	if (vertex0.pos.y_ < 0.0f) {
		y_start = 0;
	}
	else if(vertex0.pos.y_ > view_height_)
	{
		y_start = view_height_ - 1;
	}
	else
	{
		y_start = static_cast<uint32_t>(vertex0.pos.y_ + 0.5f);
	}

	//垂直裁剪
	if (vertex2.pos.y_ > view_height_ - 1) {
		y_end = view_height_ - 1;
	}
	else if(vertex2.pos.y_ < 0)
	{
		y_end = 0;
	}
	else
	{
		y_end = static_cast<uint32_t>(vertex2.pos.y_ + 0.5f);
	}

	int32_t y_error = (int32_t)((y_start - vertex0.pos.y_) * decimal_bit_float + 0.5f);
	/*KZMath::KZFix32 y_error(y_start - vertex0.pos.y_);*/
	//根据y值误差修正z,x,r,g,b,s,t
	temp_cache = (int64_t)y_error;
	temp_cache *= dzdyl;
	z_left = (int32_t)(temp_cache >> decimal_bit) + z0;
	//
	temp_cache = (int64_t)y_error;
	temp_cache *= dzdyr;
	z_right = (int32_t)(temp_cache >> decimal_bit) + z0;
	//
	temp_cache = (int64_t)y_error;
	temp_cache *= dxdyl;
	x_start = (int32_t)(temp_cache >> decimal_bit) + x0;
	//
	temp_cache = (int64_t)y_error;
	temp_cache *= dxdyr;
	x_end = (int32_t)(temp_cache >> decimal_bit) + x0;
	//
	/*z_left = y_error * dzdyl + z0;
	z_right = y_error * dzdyr + z0;
	x_start = y_error * dxdyl + x0;
	x_end = y_error * dxdyr + x0;*/

	temp_cache = (int64_t)y_error;
	temp_cache *= drdyl;
	r_left = (int32_t)(temp_cache >> decimal_bit) + r0;
	//
	temp_cache = (int64_t)y_error;
	temp_cache *= drdyr;
	r_right = (int32_t)(temp_cache >> decimal_bit) + r0;
	//
	temp_cache = (int64_t)y_error;
	temp_cache *= dgdyl;
	g_left = (int32_t)(temp_cache >> decimal_bit) + g0;
	//
	temp_cache = (int64_t)y_error;
	temp_cache *= dgdyr;
	g_right = (int32_t)(temp_cache >> decimal_bit) + g0;
	//
	temp_cache = (int64_t)y_error;
	temp_cache *= dbdyl;
	b_left = (int32_t)(temp_cache >> decimal_bit) + b0;
	//
	temp_cache = (int64_t)y_error;
	temp_cache *= dbdyr;
	b_right = (int32_t)(temp_cache >> decimal_bit) + b0;
	//
	/*r_left = y_error * drdyl + r0;
	r_right = y_error * drdyr + r0;
	g_left = y_error * dgdyl + g0;
	g_right = y_error * dgdyr + g0;
	b_left = y_error * dbdyl + b0;
	b_right = y_error * dbdyr + b0;*/

	//检查是否贴图，避免不必要计算
	if (has_texture) {
		float y_error_float = (float)y_error / decimal_bit_float;
		s_left = y_error_float * ds_left + v0_s_overz;
		s_right = y_error_float * ds_right + v0_s_overz;
		t_left = y_error_float * dt_left + v0_t_overz;
		t_right = y_error_float * dt_right + v0_t_overz;
	}

	uint32_t mip_level = 0;
	uint32_t cur_level = 0;
	if (mat_id != -1)
	{
		mip_level = mat_vec_[mat_id].GetMipLevel();
	}

	if (mip_level > 1)
	{
		float ave_z = (vertex0.pos.z_ + vertex1.pos.z_ + vertex2.pos.z_) * 0.33f + 1.0f;
		cur_level = mip_level * (uint32_t)(ave_z / 2.0f);
	}

	uint32_t z_buffer_idx = y_start * view_width_;

	for (uint32_t j = y_start; j <= y_end; ++j) {
		//z关于x的斜率
		int32_t delta_zx = 0;
		//r关于x的斜率
		int32_t delta_rx = 0;
		//g关于x的斜率
		int32_t delta_gx = 0;
		//b关于x的斜率
		int32_t delta_bx = 0;
		/*KZMath::KZFix32 delta_zx;
		KZMath::KZFix32 delta_rx;
		KZMath::KZFix32 delta_gx;
		KZMath::KZFix32 delta_bx;*/

		//s关于x的斜率
		float delta_sx = 0.0f;
		//t关于x的斜率
		float delta_tx = 0.0f;

		//分段线性纹理方法，只计算扫描线端点的准确纹理坐标，线性插值从左到右计算纹理坐标
		float one_over_z_left = decimal_bit_float / (float)z_left;
		float one_over_z_right = decimal_bit_float / (float)z_right;
		float s_left_cord = s_left * one_over_z_left;
		float s_right_cord = s_right * one_over_z_right;
		float t_left_cord = t_left * one_over_z_left;
		float t_right_cord = t_right * one_over_z_right;

		if (x_end != x_start) {
			//计算z,r,g,b,s,t相对于x的误差
			int32_t coeff(x_end - x_start);
			/*KZMath::KZFix32 coeff(x_end - x_start);*/
			temp_cache = z_right - z_left;
			temp_cache <<= decimal_bit;
			temp_cache /= coeff;
			delta_zx = (int32_t)temp_cache;
			//
			temp_cache = r_right - r_left;
			temp_cache <<= decimal_bit;
			temp_cache /= coeff;
			delta_rx = (int32_t)temp_cache;
			//
			temp_cache = g_right - g_left;
			temp_cache <<= decimal_bit;
			temp_cache /= coeff;
			delta_gx = (int32_t)temp_cache;
			//
			temp_cache = b_right - b_left;
			temp_cache <<= decimal_bit;
			temp_cache /= coeff;
			delta_bx = (int32_t)temp_cache;
			//
			/*delta_zx = (z_right - z_left) / coeff;
			delta_rx = (r_right - r_left) / coeff;
			delta_gx = (g_right - g_left) / coeff;
			delta_bx = (b_right - b_left) / coeff;*/

			//检查是否贴图，避免不必要计算
			if (has_texture) {
				float coefficient = (1 / ((float)x_end / decimal_bit_float - (float)x_start / decimal_bit_float));
				//float coefficient = (1 / (x_end.GetFloatValue() - x_start.GetFloatValue()));
				delta_sx = (s_right_cord - s_left_cord) * coefficient;
				delta_tx = (t_right_cord - t_left_cord) * coefficient;
			}
		}

		int32_t z_cur = z_left;
		int32_t r_cur = r_left;
		int32_t g_cur = g_left;
		int32_t b_cur = b_left;
		/*KZMath::KZFix32 z_cur = z_left;
		KZMath::KZFix32 r_cur = r_left;
		KZMath::KZFix32 g_cur = g_left;
		KZMath::KZFix32 b_cur = b_left;*/
		float s_cur = s_left_cord;
		float t_cur = t_left_cord;
		fix_x_start = x_start;
		fix_x_end = x_end;

		//水平裁剪
		if (x_start < min_x) {
			int32_t dx = min_x - x_start;
			/*KZMath::KZFix32 dx = min_x - x_start;*/
			//根据x值误差修正z,r,g,b,s,t
			temp_cache = (int64_t)dx;
			temp_cache *= delta_zx;
			z_cur += (int32_t)(temp_cache >> decimal_bit);
			temp_cache = (int64_t)dx;
			temp_cache *= delta_rx;
			r_cur += (int32_t)(temp_cache >> decimal_bit);
			temp_cache = (int64_t)dx;
			temp_cache *= delta_gx;
			g_cur += (int32_t)(temp_cache >> decimal_bit);
			temp_cache = (int64_t)dx;
			temp_cache *= delta_bx;
			b_cur += (int32_t)(temp_cache >> decimal_bit);
			/*z_cur += dx * delta_zx;
			r_cur += dx * delta_rx;
			g_cur += dx * delta_gx;
			b_cur += dx * delta_bx;*/

			//检查是否贴图，避免不必要计算
			if (has_texture) {
				temp_cache = (int64_t)dx;
				temp_cache *= (int32_t)(delta_sx * decimal_bit_float + 0.5f);
				result = (int32_t)(temp_cache >> decimal_bit);
				s_cur += (float)result / decimal_bit_float;
				temp_cache = (int64_t)dx;
				temp_cache *= (int32_t)(delta_tx * decimal_bit_float + 0.5f);
				result = (int32_t)(temp_cache >> decimal_bit);
				t_cur += (float)result / decimal_bit_float;
				/*s_cur += (dx * delta_sx).GetFloatValue();
				t_cur += (dx * delta_tx).GetFloatValue();*/
			}
			fix_x_start = min_x;
		}

		if (x_end < 0) {
			x_start += dxdyl;
			x_end += dxdyr;
			z_left += dzdyl;
			z_right += dzdyr;
			r_left += drdyl;
			r_right += drdyr;
			g_left += dgdyl;
			g_right += dgdyr;
			b_left += dbdyl;
			b_right += dbdyr;
			z_buffer_idx += view_width_;

			//检查是否贴图，避免不必要计算
			if (has_texture) {
				s_left += ds_left;
				s_right += ds_right;
				t_left += dt_left;
				t_right += dt_right;
			}
			continue;
		}

		if (x_end > max_x) {
			fix_x_end = max_x;
		}

		uint32_t xs = static_cast<uint32_t>((float)fix_x_start / decimal_bit_float + 0.5f);
		uint32_t xe = static_cast<uint32_t>((float)fix_x_end / decimal_bit_float + 0.5f);
		/*uint32_t xs = static_cast<uint32_t>(fix_x_start.GetFloatValue() + 0.5f);
		uint32_t xe = static_cast<uint32_t>(fix_x_end.GetFloatValue() + 0.5f);*/

		for (uint32_t k = xs; k <= xe; ++k) {
			float z_cur_f = (float)z_cur / decimal_bit_float;
			/*float z_cur_f = z_cur.GetFloatValue();*/
			//CVV裁剪z
			if (z_cur_f >= -1 && z_cur_f <= 1) {
				//深度缓冲
				//uint32_t z_buffer_index = j * view_width_ + k;
				uint32_t z_buffer_index = z_buffer_idx + k;
				if (z_cur_f <= z_buffer_[z_buffer_index]) {
					z_buffer_[z_buffer_index] = z_cur_f;
					int32_t r_cur_int = r_cur >> decimal_bit;
					int32_t g_cur_int = g_cur >> decimal_bit;
					int32_t b_cur_int = b_cur >> decimal_bit;
					/*int32_t r_cur_int = r_cur.GetIntValue();
					int32_t g_cur_int = g_cur.GetIntValue();
					int32_t b_cur_int = b_cur.GetIntValue();*/
					Color final_color(static_cast<unsigned char>(r_cur_int), static_cast<unsigned char>(g_cur_int), static_cast<unsigned char>(b_cur_int));
					if (has_texture) {
						float s_cord = s_cur < 0 ? 0 : s_cur > 1 ? 1 : s_cur;
						float t_cord = t_cur < 0 ? 0 : t_cur > 1 ? 1 : t_cur;
						Color texture_color;
						mat_vec_[mat_id].GetTextureColor(s_cord, t_cord, cur_level, texture_color);
						final_color *= texture_color;
					}
					//draw point
					uint32_t dist = z_buffer_index * 3;
					if (alpha == 1.0f && final_color.a_ != 0)
					{
						frame_buffer_[dist] = final_color.b_;//Blue
						frame_buffer_[dist + 1] = final_color.g_;//Green
						frame_buffer_[dist + 2] = final_color.r_;//Red 
					}
					else if (alpha != 1.0f && final_color.a_ != 0)
					{
						frame_buffer_[dist] = (unsigned char)(alpha * final_color.b_ + (1 - alpha) * frame_buffer_[dist]);//Blue
						frame_buffer_[dist + 1] = (unsigned char)(alpha * final_color.g_ + (1 - alpha) * frame_buffer_[dist + 1]);//Green
						frame_buffer_[dist + 2] = (unsigned char)(alpha * final_color.r_ + (1 - alpha) * frame_buffer_[dist + 2]);//Red 
					}
				}
			}
			z_cur += delta_zx;
			//限制颜色范围
			r_cur += delta_rx;
			if (r_cur < 0) {
				r_cur = 0;
			}
			else if(r_cur > max_color_value)
			{
				r_cur = max_color_value;
			}
			//r_cur = r_cur < 0 ? 0 : r_cur > 255 ? 255 : r_cur;
			g_cur += delta_gx;
			if (g_cur < 0)
			{
				g_cur = 0;
			}
			else if(g_cur > max_color_value)
			{
				g_cur = max_color_value;
			}
			//g_cur = g_cur < 0 ? 0 : g_cur > 255 ? 255 : g_cur;
			b_cur += delta_bx;
			if (b_cur < 0)
			{
				b_cur = 0;
			}
			else if(b_cur > max_color_value)
			{
				b_cur = max_color_value;
			}
			//b_cur = b_cur < 0 ? 0 : b_cur > 255 ? 255 : b_cur;

			//检查是否贴图，避免不必要计算
			if (has_texture) {
				//限制纹理坐标范围
				s_cur += delta_sx;
				s_cur = s_cur < 0 ? 0 : s_cur > 1 ? 1 : s_cur;
				t_cur += delta_tx;
				t_cur = t_cur < 0 ? 0 : t_cur > 1 ? 1 : t_cur;
			}
		}
		x_start += dxdyl;
		x_end += dxdyr;
		z_left += dzdyl;
		z_right += dzdyr;
		r_left += drdyl;
		r_right += drdyr;
		g_left += dgdyl;
		g_right += dgdyr;
		b_left += dbdyl;
		b_right += dbdyr;
		z_buffer_idx += view_width_;

		//检查是否贴图，避免不必要计算
		if (has_texture) {
			s_left += ds_left;
			s_right += ds_right;
			t_left += dt_left;
			t_right += dt_right;
		}
	}
}

//定点数版本光栅化平顶三角形
//注释部分为用数学库实现，目前全部用c风格手动写出来，避免函数调用的消耗，性能较浮点数版本略有提升，但是存在某些视觉误差
void KZPipeLine::DrawTopTriFast(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2, bool has_texture, int32_t mat_id, float alpha) {
	//定点数小数位数，整数倍数2^12
	int32_t decimal_bit = 12;
	//定点数浮点数倍数
	float decimal_bit_float = 4096.0;

	//获取三角形三个顶点对应x,y,z的定点数值
	int32_t x0 = (int32_t)(vertex0.pos.x_ * decimal_bit_float + 0.5f);
	int32_t x1 = (int32_t)(vertex1.pos.x_ * decimal_bit_float + 0.5f);
	int32_t x2 = (int32_t)(vertex2.pos.x_ * decimal_bit_float + 0.5f);
	int32_t y0 = (int32_t)(vertex0.pos.y_ * decimal_bit_float + 0.5f);
	int32_t y1 = (int32_t)(vertex1.pos.y_ * decimal_bit_float + 0.5f);
	int32_t y2 = (int32_t)(vertex2.pos.y_ * decimal_bit_float + 0.5f);
	int32_t z0 = (int32_t)(vertex0.pos.z_ * decimal_bit_float + 0.5f);
	int32_t z1 = (int32_t)(vertex1.pos.z_ * decimal_bit_float + 0.5f);
	int32_t z2 = (int32_t)(vertex2.pos.z_ * decimal_bit_float + 0.5f);
	/*KZMath::KZFix32 x0(vertex0.pos.x_), y0(vertex0.pos.y_), z0(vertex0.pos.z_);
	KZMath::KZFix32 x1(vertex1.pos.x_), y1(vertex1.pos.y_), z1(vertex1.pos.z_);
	KZMath::KZFix32 x2(vertex2.pos.x_), y2(vertex2.pos.y_), z2(vertex2.pos.z_);*/

	//获取三角形三个顶点对应r,g,b的定点数值
	int32_t r0 = ((int32_t)vertex0.color.r_) << decimal_bit;
	int32_t r1 = ((int32_t)vertex1.color.r_) << decimal_bit;
	int32_t r2 = ((int32_t)vertex2.color.r_) << decimal_bit;
	int32_t g0 = ((int32_t)vertex0.color.g_) << decimal_bit;
	int32_t g1 = ((int32_t)vertex1.color.g_) << decimal_bit;
	int32_t g2 = ((int32_t)vertex2.color.g_) << decimal_bit;
	int32_t b0 = ((int32_t)vertex0.color.b_) << decimal_bit;
	int32_t b1 = ((int32_t)vertex1.color.b_) << decimal_bit;
	int32_t b2 = ((int32_t)vertex2.color.b_) << decimal_bit;
	/*KZMath::KZFix32 r0((int32_t)vertex0.color.r_), g0((int32_t)vertex0.color.g_), b0((int32_t)vertex0.color.b_);
	KZMath::KZFix32 r1((int32_t)vertex1.color.r_), g1((int32_t)vertex1.color.g_), b1((int32_t)vertex1.color.b_);
	KZMath::KZFix32 r2((int32_t)vertex2.color.r_), g2((int32_t)vertex2.color.g_), b2((int32_t)vertex2.color.b_);*/

	//x对于y的左边斜率，x对于y的右边斜率，z对于y的左边斜率，z对于y的右边斜率
	int32_t dxdyl = 0, dxdyr = 0, dzdyl = 0, dzdyr = 0;
	//r对于y的左边斜率，r对于y的右边斜率，g对于y的左边斜率，g对于y的右边斜率，b对于y的左边斜率，b对于y的右边斜率
	int32_t drdyl = 0, drdyr = 0, dgdyl = 0, dgdyr = 0, dbdyl = 0, dbdyr = 0;

	/*KZMath::KZFix32 dxdyl, dxdyr, dzdyl, dzdyr;
	KZMath::KZFix32 drdyl, drdyr, dgdyl, dgdyr, dbdyl, dbdyr;*/
	//pos: 每条扫描线x的起始值，x的终止值，z的起始值，z的终止值
	int32_t x_start = 0, x_end = 0, z_left = 0, z_right = 0;
	//color: 每条扫描线r的起始值，r的终止值，g的起始值，g的终止值，b的起始值，b的终止值
	int32_t r_left = 0, r_right = 0, g_left = 0, g_right = 0, b_left = 0, b_right = 0;
	/*KZMath::KZFix32 x_start, x_end, z_left, z_right;
	KZMath::KZFix32 r_left, r_right, g_left, g_right, b_left, b_right;*/

	//uv: 每条扫描线s的起始值,s的终止值，t的起始值，t的终止值
	float s_left = 0.0f, s_right = 0.0f, t_left = 0.0f, t_right = 0.0f;
	//顶点v0的s/z 和 t/z的值
	float v2_s_overz = 0.0f, v2_t_overz = 0.0f;
	//s对于y的左边斜率，s对于y的右边斜率，t对于y的左边斜率，t对于y的右边斜率
	float ds_left = 0.0f, ds_right = 0.0f, dt_left = 0.0f, dt_right = 0.0f;

	//修正的x的起始值,x的终止值，防止水平裁剪的误差
	int32_t fix_x_start = 0, fix_x_end = 0;
	/*KZMath::KZFix32 fix_x_start, fix_x_end;*/

	//三角形三个顶点最大的x值和最小的x值，用于约束扫描线的x的值，防止斜率造成的误差
	int32_t max_x = 0, min_x = 0;
	/*KZMath::KZFix32 max_x, min_x;*/

	//扫描线的起始值和终止值
	uint32_t y_start = 0, y_end = 0;
	//用于临时存储，定点数的运算需要
	int64_t temp_cache = 0;
	//用于临时存储，定点数的运算需要
	int32_t result = 0;

	//最大颜色值255的定点数表示
	int32_t max_color_value = 255 << decimal_bit;
	//检查是否贴图，避免不必要计算
	if (has_texture) {
		v2_s_overz = vertex2.uv.x_ * vertex2.pos.z_;
		v2_t_overz = vertex2.uv.y_ * vertex2.pos.z_;
	}

	//计算左右两边斜率
	if (x0 < x1) {
		int32_t y_diff_left(y2 - y0);
		int32_t y_diff_right(y2 - y1);
		/*KZMath::KZFix32 y_diff_left(y2 - y0);
		KZMath::KZFix32 y_diff_right(y2 - y1);*/

		//求x,z的y的左右梯度
		temp_cache = -(x2 - x0);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_left;
		dxdyl = (int32_t)temp_cache;
		//
		temp_cache = -(x2 - x1);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_right;
		dxdyr = (int32_t)temp_cache;
		//
		temp_cache = -(z2 - z0);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_left;
		dzdyl = (int32_t)temp_cache;
		//
		temp_cache = -(z2 - z1);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_right;
		dzdyr = (int32_t)temp_cache;
		//
		/*dxdyl = -(x2 - x0) / y_diff_left;
		dxdyr = -(x2 - x1) / y_diff_right;
		dzdyl = -(z2 - z0) / y_diff_left;
		dzdyr = -(z2 - z1) / y_diff_right;*/

		//求颜色r,g,b的y的左右梯度
		temp_cache = -(r2 - r0);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_left;
		drdyl = (int32_t)temp_cache;
		//
		temp_cache = -(r2 - r1);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_right;
		drdyr = (int32_t)temp_cache;
		//
		temp_cache = -(g2 - g0);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_left;
		dgdyl = (int32_t)temp_cache;
		//
		temp_cache = -(g2 - g1);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_right;
		dgdyr = (int32_t)temp_cache;
		//
		temp_cache = -(b2 - b0);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_left;
		dbdyl = (int32_t)temp_cache;
		//
		temp_cache = -(b2 - b1);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_right;
		dbdyr = (int32_t)temp_cache;
		/*drdyl = -(r2 - r0) / y_diff_left;
		drdyr = -(r2 - r1) / y_diff_right;
		dgdyl = -(g2 - g0) / y_diff_left;
		dgdyr = -(g2 - g1) / y_diff_right;
		dbdyl = -(b2 - b0) / y_diff_left;
		dbdyr = -(b2 - b1) / y_diff_right;*/

		//检查是否贴图，避免不必要计算
		if (has_texture) {
			float one_over_y_diff_left_float = 1 / (vertex2.pos.y_ - vertex0.pos.y_);
			float one_over_y_diff_right_float = 1 / (vertex2.pos.y_ - vertex1.pos.y_);
			//求纹理s/z,t/z的y的左右梯度
			ds_left = -(v2_s_overz - vertex0.uv.x_ * vertex0.pos.z_) * one_over_y_diff_left_float;
			ds_right = -(v2_s_overz - vertex1.uv.x_ * vertex1.pos.z_) * one_over_y_diff_right_float;
			dt_left = -(v2_t_overz - vertex0.uv.y_ * vertex0.pos.z_) * one_over_y_diff_left_float;
			dt_right = -(v2_t_overz - vertex1.uv.y_ * vertex1.pos.z_) * one_over_y_diff_right_float;
		}

		//计算x最大值最小值,修正误差
		if (x1 < x2) {
			min_x = x0;
			max_x = x2;
		}
		else
		{
			max_x = x1;
			if (x0 < x2) {
				min_x = x0;
			}
			else
			{
				min_x = x2;
			}
		}
	}
	else
	{
		int32_t y_diff_left(y2 - y1);
		int32_t y_diff_right(y2 - y0);
		/*KZMath::KZFix32 y_diff_left(y2 - y1);
		KZMath::KZFix32 y_diff_right(y2 - y0);*/
		//求x,z的y的左右梯度
		temp_cache = -(x2 - x1);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_left;
		dxdyl = (int32_t)temp_cache;
		//
		temp_cache = -(x2 - x0);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_right;
		dxdyr = (int32_t)temp_cache;
		//
		temp_cache = -(z2 - z1);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_left;
		dzdyl = (int32_t)temp_cache;
		//
		temp_cache = -(z2 - z0);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_right;
		dzdyr = (int32_t)temp_cache;
		//
		/*dxdyl = -(x2 - x1) / y_diff_left;
		dxdyr = -(x2 - x0) / y_diff_right;
		dzdyl = -(z2 - z1) / y_diff_left;
		dzdyr = -(z2 - z0) / y_diff_right;*/

		//求颜色r,g,b的y的左右梯度
		temp_cache = -(r2 - r1);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_left;
		drdyl = (int32_t)temp_cache;
		//
		temp_cache = -(r2 - r0);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_right;
		drdyr = (int32_t)temp_cache;
		//
		temp_cache = -(g2 - g1);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_left;
		dgdyl = (int32_t)temp_cache;
		//
		temp_cache = -(g2 - g0);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_right;
		dgdyr = (int32_t)temp_cache;
		//
		temp_cache = -(b2 - b1);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_left;
		dbdyl = (int32_t)temp_cache;
		//
		temp_cache = -(b2 - b0);
		temp_cache <<= decimal_bit;
		temp_cache /= y_diff_right;
		dbdyr = (int32_t)temp_cache;
		//
		/*drdyl = -(r2 - r1) / y_diff_left;
		drdyr = -(r2 - r0) / y_diff_right;
		dgdyl = -(g2 - g1) / y_diff_left;
		dgdyr = -(g2 - g0) / y_diff_right;
		dbdyl = -(b2 - b1) / y_diff_left;
		dbdyr = -(b2 - b0) / y_diff_right;*/

		//检查是否贴图，避免不必要计算
		if (has_texture) {
			float one_over_y_diff_left_float = 1 / (vertex2.pos.y_ - vertex1.pos.y_);
			float one_over_y_diff_right_float = 1 / (vertex2.pos.y_ - vertex0.pos.y_);
			//求纹理s/z,t/z的y的左右梯度
			ds_left = -(v2_s_overz - vertex1.uv.x_ * vertex1.pos.z_) * one_over_y_diff_left_float;
			ds_right = -(v2_s_overz - vertex0.uv.x_ * vertex0.pos.z_) * one_over_y_diff_right_float;
			dt_left = -(v2_t_overz - vertex1.uv.y_ * vertex1.pos.z_) * one_over_y_diff_left_float;
			dt_right = -(v2_t_overz - vertex0.uv.y_ * vertex0.pos.z_) * one_over_y_diff_right_float;
		}

		//计算x最大值最小值,修正误差

		if (x0 < x2) {
			min_x = x0;
			max_x = x2;
		}
		else
		{
			max_x = x0;
			if (x1 < x2) {
				min_x = x1;
			}
			else
			{
				min_x = x2;
			}
		}
	}

	/*if (min_x < KZMath::KZFix32::ZERO) {
		min_x = KZMath::KZFix32::ZERO;
	}
	KZMath::KZFix32 view_width_fix32((int32_t)view_width_);
	if (max_x > view_width_fix32) {
		max_x = view_width_fix32;
	}*/

	if (min_x < 0) {
		min_x = 0;
	}
	result = view_width_ << decimal_bit;
	if (max_x > result) {
		max_x = result;
	}

	//垂直裁剪
	if (vertex0.pos.y_ < 0)
	{
		y_end = 0;
	}
	else if(vertex0.pos.y_ > view_height_ - 1)
	{
		y_end = view_height_ - 1;
	}
	else
	{
		y_end = static_cast<uint32_t>(vertex0.pos.y_ + 0.5f);
	}

	//垂直裁剪
	if (vertex2.pos.y_ > view_height_ - 1)
	{
		y_start = view_height_ - 1;
	}
	else if(vertex2.pos.y_ < 0)
	{
		y_start = 0;
	}
	else
	{
		y_start = static_cast<uint32_t>(vertex2.pos.y_ + 0.5f);
	}

	int32_t y_error = (int32_t)((vertex2.pos.y_ - y_start) * decimal_bit_float + 0.5f);
	/*KZMath::KZFix32 y_error(vertex2.pos.y_ - y_start);*/
	//根据y值误差修正z,x,r,g,b,s,t
	temp_cache = (int64_t)y_error;
	temp_cache *= dzdyl;
	z_left = (int32_t)(temp_cache >> decimal_bit) + z2;
	//
	temp_cache = (int64_t)y_error;
	temp_cache *= dzdyr;
	z_right = (int32_t)(temp_cache >> decimal_bit) + z2;
	//
	temp_cache = (int64_t)y_error;
	temp_cache *= dxdyl;
	x_start = (int32_t)(temp_cache >> decimal_bit) + x2;
	//
	temp_cache = (int64_t)y_error;
	temp_cache *= dxdyr;
	x_end = (int32_t)(temp_cache >> decimal_bit) + x2;
	//
	/*z_left = y_error * dzdyl + z2;
	z_right = y_error * dzdyr + z2;
	x_start = y_error * dxdyl + x2;
	x_end = y_error * dxdyr + x2;*/

	temp_cache = (int64_t)y_error;
	temp_cache *= drdyl;
	r_left = (int32_t)(temp_cache >> decimal_bit) + r2;
	//
	temp_cache = (int64_t)y_error;
	temp_cache *= drdyr;
	r_right = (int32_t)(temp_cache >> decimal_bit) + r2;
	//
	temp_cache = (int64_t)y_error;
	temp_cache *= dgdyl;
	g_left = (int32_t)(temp_cache >> decimal_bit) + g2;
	//
	temp_cache = (int64_t)y_error;
	temp_cache *= dgdyr;
	g_right = (int32_t)(temp_cache >> decimal_bit) + g2;
	//
	temp_cache = (int64_t)y_error;
	temp_cache *= dbdyl;
	b_left = (int32_t)(temp_cache >> decimal_bit) + b2;
	//
	temp_cache = (int64_t)y_error;
	temp_cache *= dbdyr;
	b_right = (int32_t)(temp_cache >> decimal_bit) + b2;
	/*r_left = y_error * drdyl + r2;
	r_right = y_error * drdyr + r2;
	g_left = y_error * dgdyl + g2;
	g_right = y_error * dgdyr + g2;
	b_left = y_error * dbdyl + b2;
	b_right = y_error * dbdyr + b2;*/

	//检查是否贴图，避免不必要计算
	if (has_texture) {
		float y_error_float = (float)y_error / decimal_bit_float;
		s_left = y_error_float * ds_left + v2_s_overz;
		s_right = y_error_float * ds_right + v2_s_overz;
		t_left = y_error_float * dt_left + v2_t_overz;
		t_right = y_error_float * dt_right + v2_t_overz;
	}

	uint32_t mip_level = 0;
	uint32_t cur_level = 0;

	if (mat_id != -1)
	{
		mip_level = mat_vec_[mat_id].GetMipLevel();
	}
	if (mip_level > 1)
	{
		float ave_z = (vertex0.pos.z_ + vertex1.pos.z_ + vertex2.pos.z_) * 0.33f + 1.0f;
		cur_level = mip_level * (uint32_t)(ave_z / 2.0f);
	}

	uint32_t z_buffer_idx = y_start * view_width_;

	for (uint32_t j = y_start; j > y_end; --j) {
		//z关于x的斜率
		int32_t delta_zx = 0;
		//r关于x的斜率
		int32_t delta_rx = 0;
		//g关于x的斜率
		int32_t delta_gx = 0;
		//b关于x的斜率
		int32_t delta_bx = 0;
		/*KZMath::KZFix32 delta_zx;
		KZMath::KZFix32 delta_rx;
		KZMath::KZFix32 delta_gx;
		KZMath::KZFix32 delta_bx;*/

		//s关于x的斜率
		float delta_sx = 0.0f;
		//t关于x的斜率
		float delta_tx = 0.0f;

		//分段线性纹理方法，只计算扫描线端点的准确纹理坐标，线性插值从左到右计算纹理坐标
		float one_over_z_left = decimal_bit_float / (float)z_left;
		float one_over_z_right = decimal_bit_float / (float)z_right;
		float s_left_cord = s_left * one_over_z_left;
		float s_right_cord = s_right * one_over_z_right;
		float t_left_cord = t_left * one_over_z_left;
		float t_right_cord = t_right * one_over_z_right;

		if (x_end - x_start != 0) {
			int32_t coeff(x_end - x_start);
			/*KZMath::KZFix32 coeff(x_end - x_start);*/
			//计算z,r,g,b,s,t相对于x的梯度
			temp_cache = z_right - z_left;
			temp_cache <<= decimal_bit;
			temp_cache /= coeff;
			delta_zx = (int32_t)temp_cache;
			//
			temp_cache = r_right - r_left;
			temp_cache <<= decimal_bit;
			temp_cache /= coeff;
			delta_rx = (int32_t)temp_cache;
			//
			temp_cache = g_right - g_left;
			temp_cache <<= decimal_bit;
			temp_cache /= coeff;
			delta_gx = (int32_t)temp_cache;
			//
			temp_cache = b_right - b_left;
			temp_cache <<= decimal_bit;
			temp_cache /= coeff;
			delta_bx = (int32_t)temp_cache;
			/*delta_zx = (z_right - z_left) / coeff;
			delta_rx = (r_right - r_left) / coeff;
			delta_gx = (g_right - g_left) / coeff;
			delta_bx = (b_right - b_left) / coeff;*/

			//检查是否贴图，避免不必要计算
			if (has_texture) {
				/*float coefficient = (1 / (x_end.GetFloatValue() - x_start.GetFloatValue()));*/
				float coefficient = (1 / ((float)x_end/decimal_bit_float - (float)x_start/decimal_bit_float));
				delta_sx = (s_right_cord - s_left_cord) * coefficient;
				delta_tx = (t_right_cord - t_left_cord) * coefficient;
			}
		}
		int32_t z_cur = z_left;
		int32_t r_cur = r_left;
		int32_t g_cur = g_left;
		int32_t b_cur = b_left;
		/*KZMath::KZFix32 z_cur = z_left;
		KZMath::KZFix32 r_cur = r_left;
		KZMath::KZFix32 g_cur = g_left;
		KZMath::KZFix32 b_cur = b_left;*/
		float s_cur = s_left_cord;
		float t_cur = t_left_cord;
		fix_x_start = x_start;
		fix_x_end = x_end;
		//水平裁剪
		if (x_start < min_x) {
			int32_t dx = min_x - x_start;
			/*KZMath::KZFix32 dx = min_x - x_start;*/
			//根据x值误差修正z,r,g,b,s,t
			temp_cache = (int64_t)dx;
			temp_cache *= delta_zx;
			z_cur += (int32_t)(temp_cache >> decimal_bit);
			temp_cache = (int64_t)dx;
			temp_cache *= delta_rx;
			r_cur += (int32_t)(temp_cache >> decimal_bit);
			temp_cache = (int64_t)dx;
			temp_cache *= delta_gx;
			g_cur += (int32_t)(temp_cache >> decimal_bit);
			temp_cache = (int64_t)dx;
			temp_cache *= delta_bx;
			b_cur += (int32_t)(temp_cache >> decimal_bit);
			/*z_cur += dx * delta_zx;
			r_cur += dx * delta_rx;
			g_cur += dx * delta_gx;
			b_cur += dx * delta_bx;*/

			//检查是否贴图，避免不必要计算
			if (has_texture) {
				temp_cache = (int64_t)dx;
				temp_cache *= (int32_t)(delta_sx * decimal_bit_float + 0.5f);
				result = (int32_t)(temp_cache >> decimal_bit);
				s_cur += (float)result / decimal_bit_float;
				temp_cache = (int64_t)dx;
				temp_cache *= (int32_t)(delta_tx * decimal_bit_float + 0.5f);
				result = (int32_t)(temp_cache >> decimal_bit);
				t_cur += (float)result / decimal_bit_float;
				/*s_cur += (dx * delta_sx).GetFloatValue();
				t_cur += (dx * delta_tx).GetFloatValue();*/
			}
			fix_x_start = min_x;
		}

		if (x_end < 0) {
			x_start += dxdyl;
			x_end += dxdyr;
			z_left += dzdyl;
			z_right += dzdyr;

			r_left += drdyl;
			r_right += drdyr;
			g_left += dgdyl;
			g_right += dgdyr;
			b_left += dbdyl;
			b_right += dbdyr;
			z_buffer_idx -= view_width_;

			//检查是否贴图，避免不必要计算
			if (has_texture) {
				s_left += ds_left;
				s_right += ds_right;
				t_left += dt_left;
				t_right += dt_right;
			}
			continue;
		}

		if (fix_x_end > max_x) {
			fix_x_end = max_x;
		}

		uint32_t xs = static_cast<uint32_t>((float)fix_x_start/decimal_bit_float + 0.5f);
		uint32_t xe = static_cast<uint32_t>((float)fix_x_end/decimal_bit_float + 0.5f);
		/*uint32_t xs = static_cast<uint32_t>(fix_x_start.GetFloatValue() + 0.5f);
		uint32_t xe = static_cast<uint32_t>(fix_x_end.GetFloatValue() + 0.5f);*/
		for (uint32_t k = xs; k <= xe; ++k) {
			float z_cur_f = (float)z_cur / decimal_bit_float;
			/*float z_cur_f = z_cur.GetFloatValue();*/
			//CVV裁剪z
			if (z_cur_f >= -1 && z_cur_f <= 1) {
				//uint32_t z_buffer_index = j * view_width_ + k;
				uint32_t z_buffer_index = z_buffer_idx + k;
				//深度缓冲检测
				if (z_cur_f <= z_buffer_[z_buffer_index]) {
					z_buffer_[z_buffer_index] = z_cur_f;
					int32_t r_cur_int = r_cur >> decimal_bit;
					int32_t g_cur_int = g_cur >> decimal_bit;
					int32_t b_cur_int = b_cur >> decimal_bit;
					/*int32_t r_cur_int = r_cur.GetIntValue();
					int32_t g_cur_int = g_cur.GetIntValue();
					int32_t b_cur_int = b_cur.GetIntValue();*/
					Color final_color(static_cast<unsigned char>(r_cur_int), static_cast<unsigned char>(g_cur_int), static_cast<unsigned char>(b_cur_int));
					if (has_texture) {
						float s_cord = s_cur < 0 ? 0 : s_cur > 1 ? 1 : s_cur;
						float t_cord = t_cur < 0 ? 0 : t_cur > 1 ? 1 : t_cur;
						Color texture_color;
						mat_vec_[mat_id].GetTextureColor(s_cord, t_cord, cur_level, texture_color);
						final_color = final_color * texture_color;
					}
					//draw point
					uint32_t dist = z_buffer_index * 3;

					if (alpha == 1.0f && final_color.a_ != 0)
					{
						frame_buffer_[dist] = final_color.b_;//Blue
						frame_buffer_[dist + 1] = final_color.g_;//Green
						frame_buffer_[dist + 2] = final_color.r_;//Red 
					}
					else if (alpha != 1.0f && final_color.a_ != 0)
					{
						frame_buffer_[dist] = (unsigned char)(alpha * final_color.b_ + (1 - alpha) * frame_buffer_[dist]);//Blue
						frame_buffer_[dist + 1] = (unsigned char)(alpha * final_color.g_ + (1 - alpha) * frame_buffer_[dist + 1]);//Green
						frame_buffer_[dist + 2] = (unsigned char)(alpha * final_color.r_ + (1 - alpha) * frame_buffer_[dist + 2]);//Red 
					}
				}
				z_cur += delta_zx;
				//限制颜色范围
				r_cur += delta_rx;
				if (r_cur < 0) {
					r_cur = 0;
				}
				else if (r_cur > max_color_value)
				{
					r_cur = max_color_value;
				}
				//r_cur = r_cur < 0 ? 0 : r_cur > 255 ? 255 : r_cur;
				g_cur += delta_gx;
				if (g_cur < 0)
				{
					g_cur = 0;
				}
				else if (g_cur > max_color_value)
				{
					g_cur = max_color_value;
				}
				//g_cur = g_cur < 0 ? 0 : g_cur > 255 ? 255 : g_cur;
				b_cur += delta_bx;
				if (b_cur < 0)
				{
					b_cur = 0;
				}
				else if (b_cur > max_color_value)
				{
					b_cur = max_color_value;
				}
				//b_cur = b_cur < 0 ? 0 : b_cur > 255 ? 255 : b_cur;

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
		x_start += dxdyl;
		x_end += dxdyr;
		z_left += dzdyl;
		z_right += dzdyr;

		r_left += drdyl;
		r_right += drdyr;
		g_left += dgdyl;
		g_right += dgdyr;
		b_left += dbdyl;
		b_right += dbdyr;
		z_buffer_idx -= view_width_;

		//检查是否贴图，避免不必要计算
		if (has_texture) {
			s_left += ds_left;
			s_right += ds_right;
			t_left += dt_left;
			t_right += dt_right;
		}
	}
}

//三角形裁剪
void KZPipeLine::PolyCulling() {
	uint32_t tri_num = pass_vec_[pass_idx_]->tri_num_;
	float near_clip = main_camera_.GetCameraNearClip();
	float far_clip = main_camera_.GetCameraFarClip();
	float w_factor = main_camera_.GetViewRight() / near_clip;
	float h_factor = main_camera_.GetViewTop() / near_clip;
	//#pragma omp parallel for
	for (uint32_t i = 0; i < tri_num; ++i) {
		//考虑左右截面的裁剪,利用三角形相似
		float x0_test = w_factor * pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[0].pos.z_;
		float x1_test = w_factor * pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[1].pos.z_;
		float x2_test = w_factor * pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[2].pos.z_;
		//对于左右截面只考虑完全接受或者完全拒绝，即所有点都在视锥体右边或者都在视锥体左边
		if ((pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[0].pos.x_ > x0_test
			&& pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[1].pos.x_ > x1_test
			&& pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[2].pos.x_ > x2_test)
			|| (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[0].pos.x_ < -x0_test
				&& pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[1].pos.x_ < -x1_test
				&& pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[2].pos.x_ < -x2_test))
		{
			pass_vec_[pass_idx_]->render_list_->tri_list_[i]->active = false;
			continue;
		}

		//考虑上下截面的裁剪，利用三角形相似
		float y0_test = h_factor * pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[0].pos.z_;
		float y1_test = h_factor * pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[1].pos.z_;
		float y2_test = h_factor * pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[2].pos.z_;
		//对于上下截面只考虑完全接受或者完全拒绝，即所有点都在视锥体上边或者都在视锥体下边
		if ((pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[0].pos.y_ > y0_test
			&& pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[1].pos.y_ > y1_test
			&& pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[2].pos.y_ > y2_test)
			|| (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[0].pos.y_ < -y0_test
				&& pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[1].pos.y_ < -y1_test
				&& pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[2].pos.y_ < -y2_test))
		{
			pass_vec_[pass_idx_]->render_list_->tri_list_[i]->active = false;
			continue;
		}

		//考虑远近截面裁剪
		//在视锥体内的顶点数
		int num_verts_inside = 0;
		//在视锥体内近截面外的顶点数
		int num_verts_outside_near_clip = 0;
		//在视锥体内的顶点
		bool verts_inside[3] = { false, false, false };
		//在视锥体近截面外的顶点
		bool verts_outside[3] = { false, false, false };

		//对每一个顶点比较z值与远近平面的关系，计数并填入索引
		if ((pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[0].pos.z_ < near_clip
			&& pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[0].pos.z_ > far_clip)) {
			verts_inside[0] = true;
			++num_verts_inside;
		}
		else if(pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[0].pos.z_ > near_clip)
		{
			verts_outside[0] = true;
			++num_verts_outside_near_clip;
		}

		if ((pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[1].pos.z_ < near_clip
			&& pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[1].pos.z_ > far_clip)) {
			verts_inside[1] = true;
			++num_verts_inside;
		}
		else if(pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[1].pos.z_ > near_clip)
		{
			verts_outside[1] = true;
			++num_verts_outside_near_clip;
		}

		if ((pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[2].pos.z_ < near_clip
			&& pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[2].pos.z_ > far_clip)) {
			verts_inside[2] = true;
			++num_verts_inside;
		}
		else if(pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[2].pos.z_ > near_clip)
		{
			verts_outside[2] = true;
			++num_verts_outside_near_clip;
		}

		//当所有的顶点都在视锥体外面
		if (num_verts_inside == 0)
		{
			pass_vec_[pass_idx_]->render_list_->tri_list_[i]->active = false;
			continue;
		}
		//当所有的顶点都在视锥体里面
		else if(num_verts_inside == 3)
		{
			continue;
		}

		if (num_verts_outside_near_clip >= 1) {
			//当只有一个顶点在视锥体里面
			if (num_verts_inside == 1)
			{
				int v0_idx = 0, v1_idx = 0, v2_idx = 0;
				if (verts_inside[0] == true) {
					v0_idx = 0; v1_idx = 1; v2_idx = 2;
				}
				else if (verts_inside[1] == true)
				{
					v0_idx = 1; v1_idx = 2; v2_idx = 0;
				}
				else
				{
					v0_idx = 2; v1_idx = 0; v2_idx = 1;
				}


				//根据线段参数方程p = v0_idx + v * t (0 < t < 1)
				float diff = (near_clip - pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].pos.z_);
				KZMath::KZVector4D<float> vec1(pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v1_idx].pos - pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].pos);
				float t1 = diff / vec1.z_;

				//根据参数t计算出交点的pos,代替原来顶点的pos
				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v1_idx].pos.x_ = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].pos.x_ + t1 * vec1.x_;
				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v1_idx].pos.y_ = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].pos.y_ + t1 * vec1.y_;
				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v1_idx].pos.z_ = near_clip;

				//根据参数t计算出交点的color,代替原来顶点的color
				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v1_idx].color.r_ = 
					(unsigned char)(pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.r_
						+ t1 * (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v1_idx].color.r_ - 
							pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.r_));

				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v1_idx].color.g_ = 
					(unsigned char)(pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.g_
						+ t1 * (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v1_idx].color.g_ - 
							pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.g_));

				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v1_idx].color.b_ = 
					(unsigned char)(pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.b_
						+ t1 * (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v1_idx].color.b_ -
							pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.b_));

				//根据线段参数方程p = v0 + v * t (0 < t < 1)
				KZMath::KZVector4D<float> vec2(pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].pos 
					- pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].pos);

				float t2 = diff / vec2.z_;
				//根据参数t计算出交点pos，代替原来pos
				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].pos.x_ = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].pos.x_ + t2 * vec2.x_;
				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].pos.y_ = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].pos.y_ + t2 * vec2.y_;
				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].pos.z_ = near_clip;

				//根据参数t计算出交点的color,代替原来顶点的color
				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].color.r_ = 
					(unsigned char)(pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.r_
					+ t2 * (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].color.r_ - 
						pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.r_));

				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].color.g_ = 
					(unsigned char)(pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.g_
					+ t2 * (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].color.g_ - 
						pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.g_));

				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].color.b_ = 
					(unsigned char)(pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.b_
					+ t2 * (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].color.b_ - 
						pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.b_));

				//默认法线不变，点法线插值计算不值得特别精确用原本点法线代替即可,并且已经计算了光照

				//根据是否有贴图计算交点的uv,代替原来的uv
				if (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->material != -1 && mat_vec_[pass_vec_[pass_idx_]->render_list_->tri_list_[i]->material].has_texture_) {

					pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v1_idx].uv.x_ = 
						pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].uv.x_
						+ t1 * (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v1_idx].uv.x_ - 
							pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].uv.x_);

					pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v1_idx].uv.y_ = 
						pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].uv.y_
						+ t1 * (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v1_idx].uv.y_ - 
							pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].uv.y_);

					pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].uv.x_ = 
						pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].uv.x_
						+ t2 * (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].uv.x_ - 
							pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].uv.x_);

					pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].uv.y_ = 
						pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].uv.y_
						+ t2 * (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].uv.y_ - 
							pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].uv.y_);
				}
			}
			//当有两个顶点在视锥体里面,需要重新分割一个三角形
			else if (num_verts_inside == 2 && num_verts_outside_near_clip == 1)
			{
				int v0_idx, v1_idx, v2_idx;
				if (verts_outside[0] == true) {
					v0_idx = 0; v1_idx = 1; v2_idx = 2;
				}
				else if(verts_outside[1] == true)
				{
					v0_idx = 1; v1_idx = 2; v2_idx = 0;
				}
				else
				{
					v0_idx = 2; v1_idx = 0; v2_idx = 1;
				}

				Triangle new_tri(*pass_vec_[pass_idx_]->render_list_->tri_list_[i]);

				//根据线段参数方程p = v0 + v * t (0 < t < 1)
				float diff = (near_clip - pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].pos.z_);
				KZMath::KZVector4D<float> vec1(pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v1_idx].pos - pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].pos);
				float t1 = diff / vec1.z_;

				KZMath::KZVector4D<float> ori_v0_pos = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].pos;
				//根据参数t计算出交点的pos,代替外部顶点的pos
				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].pos.x_ = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].pos.x_ + t1 * vec1.x_;
				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].pos.y_ = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].pos.y_ + t1 * vec1.y_;
				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].pos.z_ = near_clip;

				//根据参数t计算出交点的color,代替外部顶点的color
				float ori_v0_r = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.r_;
				float ori_v0_g = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.g_;
				float ori_v0_b = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.b_;
				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.r_ = 
					(unsigned char)(pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.r_
					+ t1 * (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v1_idx].color.r_ - 
						pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.r_));
				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.g_ = 
					(unsigned char)(pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.g_
					+ t1 * (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v1_idx].color.g_ - 
						pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.g_));

				pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.b_ = 
					(unsigned char)(pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.b_
					+ t1 * (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v1_idx].color.b_ - 
						pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].color.b_));

				//默认法线不变，点法线插值计算不值得特别精确用原本点法线代替即可,并且已经计算了光照

				float ori_v0_u = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].uv.x_;
				float ori_v0_v = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].uv.y_;
				//根据是否有贴图计算交点的uv,代替原来的uv
				if (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->material != -1 && mat_vec_[pass_vec_[pass_idx_]->render_list_->tri_list_[i]->material].has_texture_)
				{
					pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].uv.x_ = 
						pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].uv.x_
						+ t1 * (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v1_idx].uv.x_ - 
							pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].uv.x_);

					pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].uv.y_ = 
						pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].uv.y_
						+ t1 * (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v1_idx].uv.y_ - 
							pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx].uv.y_);
				}

				//新三角形的保留v2不变
				new_tri.vertex_list[v1_idx] = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v0_idx];

				//根据线段参数方程p = v0 + v * t (0 < t < 1)
				//diff = (near_clip - render_list_->tri_list_[i].vertex_list[v0_idx].pos.z_);
				KZMath::KZVector4D<float> vec2(pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].pos - ori_v0_pos);
				float t2 = diff / vec2.z_;

				//根据参数t计算出交点的pos
				new_tri.vertex_list[v0_idx].pos.x_ = ori_v0_pos.x_ + t2 * vec2.x_;
				new_tri.vertex_list[v0_idx].pos.y_ = ori_v0_pos.y_ + t2 * vec2.y_;
				new_tri.vertex_list[v0_idx].pos.z_ = near_clip;

				//根据参数t计算出交点的color
				new_tri.vertex_list[v0_idx].color.r_ = (unsigned char)(ori_v0_r +
					t2 * (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].color.r_ - ori_v0_r));
				new_tri.vertex_list[v0_idx].color.g_ = (unsigned char)(ori_v0_g +
					t2 * (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].color.g_ - ori_v0_g));
				new_tri.vertex_list[v0_idx].color.b_ = (unsigned char)(ori_v0_b +
					t2 * (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].color.b_ - ori_v0_b));

				//默认法线不变，点法线插值计算不值得特别精确用原本点法线代替即可,并且已经计算了光照
				new_tri.vertex_list[1].normal = pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].normal;

				//根据是否有贴图计算交点的uv,代替原来的uv
				if (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->material != -1 && mat_vec_[pass_vec_[pass_idx_]->render_list_->tri_list_[i]->material].has_texture_)
				{
					new_tri.vertex_list[v0_idx].uv.x_ = ori_v0_u
						+ t2 * (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].uv.x_ - ori_v0_u);

					new_tri.vertex_list[v0_idx].uv.y_ = ori_v0_v
						+ t2 * (pass_vec_[pass_idx_]->render_list_->tri_list_[i]->vertex_list[v2_idx].uv.y_ - ori_v0_v);
				}

				//将三角形插入渲染队列
				++pass_vec_[pass_idx_]->tri_num_;
				if (pass_vec_[pass_idx_]->tri_num_ > pass_vec_[pass_idx_]->render_list_->tri_list_.size()) {
					pass_vec_[pass_idx_]->render_list_->tri_list_.resize(pass_vec_[pass_idx_]->tri_num_ * 2);
				}
				pass_vec_[pass_idx_]->render_list_->tri_list_[pass_vec_[pass_idx_]->tri_num_ - 1] = &new_tri;

			}
		}
		
	}
}

//获取场景AABB包围盒
void KZPipeLine::getSceneAABB(bool include_transparent) {
	aabb_min_.x_ = FLT_MAX;
	aabb_min_.y_ = FLT_MAX;
	aabb_min_.z_ = FLT_MAX;
	aabb_max_.x_ = -FLT_MAX;
	aabb_max_.y_ = -FLT_MAX;
	aabb_max_.z_ = -FLT_MAX;
	for (uint32_t i = 0; i < pass_num_; ++i) {
		for (uint32_t j = 0; j < pass_vec_[i]->object_num_; ++j) {
			if (!include_transparent && pass_vec_[i]->render_list_->transparent)
			{
				continue;
			}
			pass_vec_[i]->object_vec_[j]->GetObjectAABB(aabb_min_, aabb_max_);
		}
	}
}


//场景剔除：物体剔除，背面消除
void KZPipeLine::SceneManageCulling() {
	switch (scene_manage_)
	{
	case KZEngine::SceneManage::NONE:
		OcclusionCulling();
		BackfaceCulling();
		break;
	case KZEngine::SceneManage::BHV:
		KZBHV::BHVTreeCulling(bhv_root_);
		break;
	case KZEngine::SceneManage::BSP:
		break;
	default:
		break;
	}
}