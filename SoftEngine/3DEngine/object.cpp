#include"object.h"
using namespace KZEngine;

//物体通过矩阵从模型坐标系转世界坐标系
void KZObject::TransformModelToWorldMatrix() {
	KZMath::KZMatrix44 transform;
	transform.Translation(world_pos_);
	Vertex temp_vertex(0,0,0);
	vlist_tran_.clear();
	vlist_tran_.reserve(num_vertices_);
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		temp_vertex.pos = transform * vlist_local_[i].pos;
		vlist_tran_.push_back(temp_vertex);
	}
	return;
}

//物体通过数学公式从模型坐标系转世界坐标系
void KZObject::TransformModelToWorldMath() {
	Vertex temp_vertex(0, 0, 0, 0, 0, 0);
	vlist_tran_.clear();
	vlist_tran_.reserve(num_vertices_);
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		temp_vertex = vlist_local_[i];
		temp_vertex.pos += world_pos_;
		vlist_tran_.push_back(temp_vertex);
	}
	return;
}

//获取物体包围盒
void KZObject::GetObjectAABB(KZMath::KZVector4D<float>&vec_min, KZMath::KZVector4D<float>& vec_max) const
{
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		float temp_x = vlist_local_[i].pos.x_ + world_pos_.x_;
		float temp_y = vlist_local_[i].pos.y_ + world_pos_.y_;
		float temp_z = vlist_local_[i].pos.z_ + world_pos_.z_;

		if (temp_x > vec_max.x_)
		{
			vec_max.x_ = temp_x;
		}
		else if (temp_x < vec_min.x_)
		{
			vec_min.x_ = temp_x;
		}

		if (temp_y > vec_max.y_)
		{
			vec_max.y_ = temp_y;
		}
		else if(temp_y < vec_min.y_)
		{
			vec_min.y_ = temp_y;
		}

		if (temp_z > vec_max.z_) {
			vec_max.z_ = temp_z;
		}
		else if(temp_z < vec_min.z_)
		{
			vec_min.z_ = temp_z;
		}
	}
}

//通过矩阵缩放
void KZObject::ScaleMatrix(const KZMath::KZVector4D<float>& vector) {
	KZMath::KZMatrix44 transform;
	transform.Scaling(vector);
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		vlist_local_[i].pos = transform * vlist_local_[i].pos;
	}
	return;
}

//通过数学公式缩放
void KZObject::ScaleMath(const KZMath::KZVector4D<float>& vector) {
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		vlist_local_[i].pos *= vector;
	}
	return;
}

//通过矩阵旋转
void KZObject::RotationMatrix(const KZMath::KZMatrix44& matrix) {
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		vlist_local_[i].pos = matrix * vlist_local_[i].pos;
		vlist_local_[i].normal = matrix * vlist_local_[i].normal;
	}
	for (uint32_t i = 0; i < num_face_; ++i) {
		face_normal_[i] = matrix * face_normal_[i];
	}
	ux_ = matrix * ux_;
	uy_ = matrix * uy_;
	uz_ = matrix * uz_;
	return;
}

//通过四元数旋转
void KZObject::RotationQuat(const KZMath::KZQuat& quat) {
	KZMath::KZQuat temp_quat;
	KZMath::KZQuat quat_conj;
	KZMath::KZQuat ans_quat;
	Vertex ans_vertex(0,0,0);
	quat.Conjugate(quat_conj);
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		temp_quat.Set(0, vlist_local_[i].pos.x_, vlist_local_[i].pos.y_, vlist_local_[i].pos.z_);
		quat.TripleProduct(temp_quat, quat_conj, ans_quat);
		vlist_local_[i].pos.x_ = ans_quat.x_;
		vlist_local_[i].pos.y_ = ans_quat.y_;
		vlist_local_[i].pos.z_ = ans_quat.z_;
	}
	for (uint32_t i = 0; i < num_face_; ++i) {
		temp_quat.Set(0, face_normal_[i].x_, face_normal_[i].y_, face_normal_[i].z_);
		quat.TripleProduct(temp_quat, quat_conj, ans_quat);
		face_normal_[i].x_ = ans_quat.x_;
		face_normal_[i].y_ = ans_quat.y_;
		face_normal_[i].z_ = ans_quat.z_;
	}
	temp_quat.x_ = ux_.x_;
	temp_quat.y_ = ux_.y_;
	temp_quat.z_ = ux_.z_;
	return;
}

//计算包围球半径
void KZObject::CalculateRadian() {
	float max_r = 0.0f;
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		if (vlist_local_[i].pos.Vector3Length() > max_r) {
			max_r = vlist_local_[i].pos.Vector3Length();
		}
	}
	max_radius_ = sqrt(max_r);
}

//预先计算面法线和顶点法线
void KZObject::CalculateNormal(bool need_vertex) {
	face_normal_.reserve(num_face_);
	uint32_t* poly_vertex = new uint32_t[num_vertices_]();
	for (uint32_t i = 0; i < num_index_; i += 3) {
		KZMath::KZVector4D<float> vec1 = vlist_local_[index_[i + 1]].pos - vlist_local_[index_[i]].pos;
		vec1.w_ = 0;
		KZMath::KZVector4D<float> vec2 = vlist_local_[index_[i + 2]].pos - vlist_local_[index_[i + 1]].pos;
		vec2.w_ = 0;
		KZMath::KZVector4D<float> face_normal;
		vec1.Vector3Cross(face_normal, vec2);
		face_normal_.push_back(face_normal);
		if (need_vertex) {
			vlist_local_[index_[i]].normal += face_normal;
			vlist_local_[index_[i + 1]].normal += face_normal;
			vlist_local_[index_[i + 2]].normal += face_normal;
			++poly_vertex[index_[i]];
			++poly_vertex[index_[i + 1]];
			++poly_vertex[index_[i + 2]];
		}
	}
	if (need_vertex) {
		for (uint32_t i = 0; i < num_vertices_; ++i) {
			vlist_local_[i].normal /= (float)poly_vertex[i];
			vlist_local_[i].normal.Vector3Normalize();
		}
	}
	delete[]poly_vertex;
	return;
}

//矩阵转换
void KZObject::Transform(const KZMath::KZMatrix44& matrix)
{
	for (uint32_t i = 0; i < num_vertices_; ++i) {
		vlist_tran_[i].pos = matrix * vlist_tran_[i].pos;
	}
}

//创建Cube
KZCube::KZCube(float width, float length, float height, bool light_cube, const KZMath::KZVector4D<float>& world_pos, const KZMath::KZQuat& quat, const KZMath::KZVector4D<float>& scale)
{

	name_ = "cube";
	world_pos_ = world_pos;
	/*cube_obj.ux_ = KZMath::KZVector4D<float>(1, 0, 0, 0);
	cube_obj.uy_ = KZMath::KZVector4D<float>(0, 1, 0, 0);
	cube_obj.uz_ = KZMath::KZVector4D<float>(0, 0, 1, 0);*/
	num_vertices_ = 8;
	vlist_local_.reserve(num_vertices_);


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
		has_shadow = false;
	}

	//将顶点加入列表
	vlist_local_.push_back(move(v1));
	vlist_local_.push_back(move(v2));
	vlist_local_.push_back(move(v3));
	vlist_local_.push_back(move(v4));
	vlist_local_.push_back(move(v5));
	vlist_local_.push_back(move(v6));
	vlist_local_.push_back(move(v7));
	vlist_local_.push_back(move(v8));

	num_index_ = 36;
	num_face_ = 12;
	index_.reserve(36);

	//设置顶点索引
	//front face
	index_.push_back(0);
	index_.push_back(2);
	index_.push_back(1);

	index_.push_back(0);
	index_.push_back(3);
	index_.push_back(2);

	//back face
	index_.push_back(7);
	index_.push_back(5);
	index_.push_back(6);

	index_.push_back(7);
	index_.push_back(4);
	index_.push_back(5);

	//left face
	index_.push_back(3);
	index_.push_back(6);
	index_.push_back(2);

	index_.push_back(3);
	index_.push_back(7);
	index_.push_back(6);

	//right face
	index_.push_back(4);
	index_.push_back(1);
	index_.push_back(5);

	index_.push_back(4);
	index_.push_back(0);
	index_.push_back(1);

	//top face
	index_.push_back(4);
	index_.push_back(3);
	index_.push_back(0);

	index_.push_back(4);
	index_.push_back(7);
	index_.push_back(3);

	//bottom face
	index_.push_back(1);
	index_.push_back(6);
	index_.push_back(5);

	index_.push_back(1);
	index_.push_back(2);
	index_.push_back(6);

	CalculateRadian();
	CalculateNormal();


	if (quat != KZMath::KZQuat::ZERO) {
		RotationQuat(quat);
	}

	if (scale != KZMath::KZVector4D<float>(1, 1, 1))
	{
		ScaleMath(scale);
	}
}

KZCylinder::KZCylinder(float top_radius, float bottom_radius, float height, uint32_t stack, uint32_t slice, const KZEngine::Color& ini_color,
	const KZMath::KZVector4D<float>& world_pos, const KZMath::KZQuat& quat, const KZMath::KZVector4D<float>& scale) 
{
	name_ = "cylinder";
	world_pos_ = world_pos;
	ux_ = KZMath::KZVector4D<float>(1, 0, 0, 0);
	uy_ = KZMath::KZVector4D<float>(0, 1, 0, 0);
	uz_ = KZMath::KZVector4D<float>(0, 0, 1, 0);

	//uint32_t verts_per_row = slice;
	uint32_t n_rows = stack + 1;

	//+2因为顶部和底部各有个中心的顶点
	num_vertices_ = slice * n_rows + 2;
	vlist_local_.resize(num_vertices_);

	//stack * slice * 2 * 3 + slice * 2 * 3
	num_face_ = (stack + 1) * 2 * slice;
	num_index_ = num_face_ * 3;
	index_.reserve(num_index_);


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
			vlist_local_[vertex_idx].pos.Set(temp_radius * cos(theta), temp_y, temp_radius * sin(theta));
			vlist_local_[vertex_idx].color = ini_color;
			theta += d_theta;
			if (i != stack)
			{
				index_.push_back(vertex_idx);
				index_.push_back(vertex_idx + 1);
				index_.push_back(vertex_idx + 1 + slice);

				index_.push_back(vertex_idx);
				index_.push_back(vertex_idx + 1 + slice);
				index_.push_back(vertex_idx + slice);
			}

			++vertex_idx;
		}
		temp_y -= d_height;
		temp_radius += d_radius;
	}

	vlist_local_[vertex_idx].pos.Set(0, half_height, 0);
	vlist_local_[vertex_idx].color = Color(255, 0, 0);
	++vertex_idx;
	vlist_local_[vertex_idx].pos.Set(0, -half_height, 0);
	vlist_local_[vertex_idx].color = Color(255, 0, 0);

	uint32_t next_index = 0;
	uint32_t diff = stack * slice;
	for (uint32_t i = 0; i < slice; ++i)
	{
		next_index = i + 1;
		if (next_index == slice)
		{
			next_index = 0;
		}
		index_.push_back(next_index);
		index_.push_back(i);
		index_.push_back(num_vertices_ - 2);

		index_.push_back(diff + next_index);
		index_.push_back(diff + i);
		index_.push_back(num_vertices_ - 1);
	}

	mat_id_.resize(num_face_, -1);
	
	CalculateRadian();
	CalculateNormal();

	if (quat != KZMath::KZQuat::ZERO) {
		RotationQuat(quat);
	}

	if (scale != KZMath::KZVector4D<float>(1, 1, 1))
	{
		ScaleMath(scale);
	}
}

KZPyramid::KZPyramid(const KZMath::KZVector4D<float>& world_pos, const KZMath::KZQuat& quat, const KZMath::KZVector4D<float>& scale) {
	name_ = "pyramid";
	world_pos_ = world_pos;
	ux_ = KZMath::KZVector4D<float>(1, 0, 0, 0);
	uy_ = KZMath::KZVector4D<float>(0, 1, 0, 0);
	uz_ = KZMath::KZVector4D<float>(0, 0, 1, 0);
	num_vertices_ = 4;
	vlist_local_.reserve(num_vertices_);

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
	vlist_local_.push_back(v1);
	vlist_local_.push_back(v2);
	vlist_local_.push_back(v3);
	vlist_local_.push_back(v4);

	num_index_ = 12;
	num_face_ = 4;
	index_.reserve(12);

	//设置顶点索引
	//face 1
	index_.push_back(3);
	index_.push_back(0);
	index_.push_back(2);

	//face 2
	index_.push_back(3);
	index_.push_back(2);
	index_.push_back(1);

	//face 3
	index_.push_back(3);
	index_.push_back(1);
	index_.push_back(0);

	//face 4
	index_.push_back(1);
	index_.push_back(2);
	index_.push_back(0);



	CalculateRadian();
	CalculateNormal();
}

KZSphere::KZSphere(float radius, uint32_t stack, uint32_t slice, const KZEngine::Color& ini_color,
	const KZMath::KZVector4D<float>& world_pos,
	const KZMath::KZQuat& quat,
	const KZMath::KZVector4D<float>& scale)
{
	name_ = "sphere";
	world_pos_ = world_pos;
	ux_ = KZMath::KZVector4D<float>(1, 0, 0, 0);
	uy_ = KZMath::KZVector4D<float>(0, 1, 0, 0);
	uz_ = KZMath::KZVector4D<float>(0, 0, 1, 0);

	uint32_t vertex_idx = 0;

	//+2因为顶部和底部各有个中心的顶点
	num_vertices_ = slice * (stack - 1) + 2;
	vlist_local_.resize(num_vertices_);

	//(stack - 2) * slice * 2 + slice * 2
	num_face_ = (stack - 1) * 2 * slice;
	num_index_ = num_face_ * 3;
	index_.reserve(num_index_);

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
			vlist_local_[vertex_idx].pos.Set(temp_radius * cos(theta), radius * cos(phy), temp_radius * sin(theta));
			vlist_local_[vertex_idx].color = ini_color;
			if (i != stack - 1)
			{
				index_.push_back(vertex_idx);
				index_.push_back(vertex_idx + 1);
				index_.push_back(vertex_idx + 1 + slice);

				index_.push_back(vertex_idx);
				index_.push_back(vertex_idx + 1 + slice);
				index_.push_back(vertex_idx + slice);
			}
			theta += d_theta;
			++vertex_idx;
		}
		phy += d_phy;
	}

	vlist_local_[vertex_idx].pos.Set(0, radius, 0);
	vlist_local_[vertex_idx].color = Color(255, 0, 0);
	++vertex_idx;
	vlist_local_[vertex_idx].pos.Set(0, -radius, 0);
	vlist_local_[vertex_idx].color = Color(255, 0, 0);

	uint32_t next_index = 0;
	uint32_t diff = (stack - 2) * slice;
	for (uint32_t i = 0; i < slice; ++i)
	{
		next_index = i + 1;
		if (next_index == slice)
		{
			next_index = 0;
		}
		index_.push_back(next_index);
		index_.push_back(i);
		index_.push_back(num_vertices_ - 2);

		index_.push_back(diff + next_index);
		index_.push_back(diff + i);
		index_.push_back(num_vertices_ - 1);
	}

	
	CalculateRadian();
	CalculateNormal();

	if (quat != KZMath::KZQuat::ZERO) {
		RotationQuat(quat);
	}

	if (scale != KZMath::KZVector4D<float>(1, 1, 1))
	{
		ScaleMath(scale);
	}
}