#include"object.h"
using namespace KZEngine;

//����ͨ�������ģ������ϵת��������ϵ
void KZObject::TransformModelToWorldMatrix() {
	KZMath::KZMatrix44 transform;
	transform.Translation(world_pos_);
	Vertex temp_vertex(0,0,0);
	mesh.vlist_tran_.clear();
	mesh.vlist_tran_.reserve(mesh.num_vertices_);
	for (uint32_t i = 0; i < mesh.num_vertices_; ++i) {
		temp_vertex.pos = transform * mesh.vlist_local_[i].pos;
		mesh.vlist_tran_.push_back(temp_vertex);
	}
	return;
}

//����ͨ����ѧ��ʽ��ģ������ϵת��������ϵ
void KZObject::TransformModelToWorldMath() {
	Vertex temp_vertex(0, 0, 0, 0, 0, 0);
	mesh.vlist_tran_.clear();
	mesh.vlist_tran_.reserve(mesh.num_vertices_);
	for (uint32_t i = 0; i < mesh.num_vertices_; ++i) {
		temp_vertex = mesh.vlist_local_[i];
		temp_vertex.pos += world_pos_;
		mesh.vlist_tran_.push_back(temp_vertex);
	}
	return;
}

//��ȡ�����Χ��
void KZObject::GetObjectAABB(KZMath::KZVector4D<float>&vec_min, KZMath::KZVector4D<float>& vec_max) const
{
	for (uint32_t i = 0; i < mesh.num_vertices_; ++i) {
		float temp_x = mesh.vlist_local_[i].pos.x_ + world_pos_.x_;
		float temp_y = mesh.vlist_local_[i].pos.y_ + world_pos_.y_;
		float temp_z = mesh.vlist_local_[i].pos.z_ + world_pos_.z_;

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

//ͨ����������
void KZObject::ScaleMatrix(const KZMath::KZVector4D<float>& vector) {
	KZMath::KZMatrix44 transform;
	transform.Scaling(vector);
	for (uint32_t i = 0; i < mesh.num_vertices_; ++i) {
		mesh.vlist_local_[i].pos = transform * mesh.vlist_local_[i].pos;
	}
	return;
}

//ͨ����ѧ��ʽ����
void KZObject::ScaleMath(const KZMath::KZVector4D<float>& vector) {
	for (uint32_t i = 0; i < mesh.num_vertices_; ++i) {
		mesh.vlist_local_[i].pos *= vector;
	}
	return;
}

//ͨ��������ת
void KZObject::RotationMatrix(const KZMath::KZMatrix44& matrix) {
	for (uint32_t i = 0; i < mesh.num_vertices_; ++i) {
		mesh.vlist_local_[i].pos = matrix * mesh.vlist_local_[i].pos;
		mesh.vlist_local_[i].normal = matrix * mesh.vlist_local_[i].normal;
	}
	for (uint32_t i = 0; i < mesh.num_face_; ++i) {
		mesh.face_normal_[i] = matrix * mesh.face_normal_[i];
	}
	ux_ = matrix * ux_;
	uy_ = matrix * uy_;
	uz_ = matrix * uz_;
	return;
}

//ͨ����Ԫ����ת
void KZObject::RotationQuat(const KZMath::KZQuat& quat) {
	KZMath::KZQuat temp_quat;
	KZMath::KZQuat quat_conj;
	KZMath::KZQuat ans_quat;
	Vertex ans_vertex(0,0,0);
	quat.Conjugate(quat_conj);
	for (uint32_t i = 0; i < mesh.num_vertices_; ++i) {
		temp_quat.Set(0, mesh.vlist_local_[i].pos.x_, mesh.vlist_local_[i].pos.y_, mesh.vlist_local_[i].pos.z_);
		quat.TripleProduct(temp_quat, quat_conj, ans_quat);
		mesh.vlist_local_[i].pos.x_ = ans_quat.x_;
		mesh.vlist_local_[i].pos.y_ = ans_quat.y_;
		mesh.vlist_local_[i].pos.z_ = ans_quat.z_;
	}
	for (uint32_t i = 0; i < mesh.num_face_; ++i) {
		temp_quat.Set(0, mesh.face_normal_[i].x_, mesh.face_normal_[i].y_, mesh.face_normal_[i].z_);
		quat.TripleProduct(temp_quat, quat_conj, ans_quat);
		mesh.face_normal_[i].x_ = ans_quat.x_;
		mesh.face_normal_[i].y_ = ans_quat.y_;
		mesh.face_normal_[i].z_ = ans_quat.z_;
	}
	temp_quat.x_ = ux_.x_;
	temp_quat.y_ = ux_.y_;
	temp_quat.z_ = ux_.z_;
	return;
}

//�����Χ��뾶
void KZObject::CalculateRadian() {
	float max_r = 0.0f;
	for (uint32_t i = 0; i < mesh.num_vertices_; ++i) {
		if (mesh.vlist_local_[i].pos.Vector3Length() > max_r) {
			max_r = mesh.vlist_local_[i].pos.Vector3Length();
		}
	}
	max_radius_ = sqrt(max_r);
}

//Ԥ�ȼ����淨�ߺͶ��㷨��
void KZObject::CalculateNormal(bool need_vertex) {
	mesh.face_normal_.reserve(mesh.num_face_);
	uint32_t* poly_vertex = new uint32_t[mesh.num_vertices_]();
	for (uint32_t i = 0; i < mesh.num_index_; i += 3) {
		KZMath::KZVector4D<float> vec1 = mesh.vlist_local_[mesh.index_[i + 1]].pos - mesh.vlist_local_[mesh.index_[i]].pos;
		vec1.w_ = 0;
		KZMath::KZVector4D<float> vec2 = mesh.vlist_local_[mesh.index_[i + 2]].pos - mesh.vlist_local_[mesh.index_[i + 1]].pos;
		vec2.w_ = 0;
		KZMath::KZVector4D<float> face_normal;
		vec1.Vector3Cross(face_normal, vec2);
		mesh.face_normal_.push_back(face_normal);
		if (need_vertex) {
			mesh.vlist_local_[mesh.index_[i]].normal += face_normal;
			mesh.vlist_local_[mesh.index_[i + 1]].normal += face_normal;
			mesh.vlist_local_[mesh.index_[i + 2]].normal += face_normal;
			++poly_vertex[mesh.index_[i]];
			++poly_vertex[mesh.index_[i + 1]];
			++poly_vertex[mesh.index_[i + 2]];
		}
	}
	if (need_vertex) {
		for (uint32_t i = 0; i < mesh.num_vertices_; ++i) {
			mesh.vlist_local_[i].normal /= (float)poly_vertex[i];
			mesh.vlist_local_[i].normal.Vector3Normalize();
		}
	}
	delete[]poly_vertex;
	return;
}

//����ת��
void KZObject::Transform(const KZMath::KZMatrix44& matrix)
{
	for (uint32_t i = 0; i < mesh.num_vertices_; ++i) {
		mesh.vlist_tran_[i].pos = matrix * mesh.vlist_tran_[i].pos;
	}
}

//����Cube
KZCube::KZCube(float width, float length, float height, bool light_cube, const KZMath::KZVector4D<float>& world_pos, const KZMath::KZQuat& quat, const KZMath::KZVector4D<float>& scale)
{

	name_ = "cube";
	world_pos_ = world_pos;
	/*cube_obj.ux_ = KZMath::KZVector4D<float>(1, 0, 0, 0);
	cube_obj.uy_ = KZMath::KZVector4D<float>(0, 1, 0, 0);
	cube_obj.uz_ = KZMath::KZVector4D<float>(0, 0, 1, 0);*/
	mesh.num_vertices_ = 8;
	mesh.vlist_local_.reserve(mesh.num_vertices_);


	//���ö����ģ������
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
		//���ö������ɫ
		v1.color.Set(255, 0, 0);
		v2.color.Set(0, 255, 0);
		v3.color.Set(0, 0, 255);
		v4.color.Set(255, 255, 0);
		v5.color.Set(255, 0, 255);
		v6.color.Set(0, 255, 255);
		v7.color.Set(255, 255, 255);
		v8.color.Set(128, 128, 128);

		//���ö����uv����
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

	//����������б�
	mesh.vlist_local_.push_back(move(v1));
	mesh.vlist_local_.push_back(move(v2));
	mesh.vlist_local_.push_back(move(v3));
	mesh.vlist_local_.push_back(move(v4));
	mesh.vlist_local_.push_back(move(v5));
	mesh.vlist_local_.push_back(move(v6));
	mesh.vlist_local_.push_back(move(v7));
	mesh.vlist_local_.push_back(move(v8));

	mesh.num_index_ = 36;
	mesh.num_face_ = 12;
	mesh.index_.reserve(36);

	//���ö�������
	//front face
	mesh.index_.push_back(0);
	mesh.index_.push_back(2);
	mesh.index_.push_back(1);

	mesh.index_.push_back(0);
	mesh.index_.push_back(3);
	mesh.index_.push_back(2);

	//back face
	mesh.index_.push_back(7);
	mesh.index_.push_back(5);
	mesh.index_.push_back(6);

	mesh.index_.push_back(7);
	mesh.index_.push_back(4);
	mesh.index_.push_back(5);

	//left face
	mesh.index_.push_back(3);
	mesh.index_.push_back(6);
	mesh.index_.push_back(2);

	mesh.index_.push_back(3);
	mesh.index_.push_back(7);
	mesh.index_.push_back(6);

	//right face
	mesh.index_.push_back(4);
	mesh.index_.push_back(1);
	mesh.index_.push_back(5);

	mesh.index_.push_back(4);
	mesh.index_.push_back(0);
	mesh.index_.push_back(1);

	//top face
	mesh.index_.push_back(4);
	mesh.index_.push_back(3);
	mesh.index_.push_back(0);

	mesh.index_.push_back(4);
	mesh.index_.push_back(7);
	mesh.index_.push_back(3);

	//bottom face
	mesh.index_.push_back(1);
	mesh.index_.push_back(6);
	mesh.index_.push_back(5);

	mesh.index_.push_back(1);
	mesh.index_.push_back(2);
	mesh.index_.push_back(6);

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

	//+2��Ϊ�����͵ײ����и����ĵĶ���
	mesh.num_vertices_ = slice * n_rows + 2;
	mesh.vlist_local_.resize(mesh.num_vertices_);

	//stack * slice * 2 * 3 + slice * 2 * 3
	mesh.num_face_ = (stack + 1) * 2 * slice;
	mesh.num_index_ = mesh.num_face_ * 3;
	mesh.index_.reserve(mesh.num_index_);


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
			mesh.vlist_local_[vertex_idx].pos.Set(temp_radius * cos(theta), temp_y, temp_radius * sin(theta));
			mesh.vlist_local_[vertex_idx].color = ini_color;
			theta += d_theta;
			if (i != stack)
			{
				mesh.index_.push_back(vertex_idx);
				mesh.index_.push_back(vertex_idx + 1);
				mesh.index_.push_back(vertex_idx + 1 + slice);

				mesh.index_.push_back(vertex_idx);
				mesh.index_.push_back(vertex_idx + 1 + slice);
				mesh.index_.push_back(vertex_idx + slice);
			}

			++vertex_idx;
		}
		temp_y -= d_height;
		temp_radius += d_radius;
	}

	mesh.vlist_local_[vertex_idx].pos.Set(0, half_height, 0);
	mesh.vlist_local_[vertex_idx].color = Color(255, 0, 0);
	++vertex_idx;
	mesh.vlist_local_[vertex_idx].pos.Set(0, -half_height, 0);
	mesh.vlist_local_[vertex_idx].color = Color(255, 0, 0);

	uint32_t next_index = 0;
	uint32_t diff = stack * slice;
	for (uint32_t i = 0; i < slice; ++i)
	{
		next_index = i + 1;
		if (next_index == slice)
		{
			next_index = 0;
		}
		mesh.index_.push_back(next_index);
		mesh.index_.push_back(i);
		mesh.index_.push_back(mesh.num_vertices_ - 2);

		mesh.index_.push_back(diff + next_index);
		mesh.index_.push_back(diff + i);
		mesh.index_.push_back(mesh.num_vertices_ - 1);
	}

	mat_id_.resize(mesh.num_face_, -1);
	
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
	mesh.num_vertices_ = 4;
	mesh.vlist_local_.reserve(mesh.num_vertices_);

	//���ö����ģ������
	KZEngine::Vertex v1(-1, 0, 0), v2(1, 0, 0), v3(0, 0, 2), v4(0, 2, 1);
	/*v1.pos.Set(-1, 0, 0);
	v2.pos.Set(1, 0, 0);
	v3.pos.Set(0, 0, 2);
	v4.pos.Set(0, 2, 1);*/

	v1.color.Set(0, 0, 0);
	v2.color.Set(0, 0, 0);
	v3.color.Set(0, 0, 0);
	v4.color.Set(0, 0, 0);

	//����������б�
	mesh.vlist_local_.push_back(v1);
	mesh.vlist_local_.push_back(v2);
	mesh.vlist_local_.push_back(v3);
	mesh.vlist_local_.push_back(v4);

	mesh.num_index_ = 12;
	mesh.num_face_ = 4;
	mesh.index_.reserve(12);

	//���ö�������
	//face 1
	mesh.index_.push_back(3);
	mesh.index_.push_back(0);
	mesh.index_.push_back(2);

	//face 2
	mesh.index_.push_back(3);
	mesh.index_.push_back(2);
	mesh.index_.push_back(1);

	//face 3
	mesh.index_.push_back(3);
	mesh.index_.push_back(1);
	mesh.index_.push_back(0);

	//face 4
	mesh.index_.push_back(1);
	mesh.index_.push_back(2);
	mesh.index_.push_back(0);



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

	//+2��Ϊ�����͵ײ����и����ĵĶ���
	mesh.num_vertices_ = slice * (stack - 1) + 2;
	mesh.vlist_local_.resize(mesh.num_vertices_);

	//(stack - 2) * slice * 2 + slice * 2
	mesh.num_face_ = (stack - 1) * 2 * slice;
	mesh.num_index_ = mesh.num_face_ * 3;
	mesh.index_.reserve(mesh.num_index_);

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
			mesh.vlist_local_[vertex_idx].pos.Set(temp_radius * cos(theta), radius * cos(phy), temp_radius * sin(theta));
			mesh.vlist_local_[vertex_idx].color = ini_color;
			if (i != stack - 1)
			{
				mesh.index_.push_back(vertex_idx);
				mesh.index_.push_back(vertex_idx + 1);
				mesh.index_.push_back(vertex_idx + 1 + slice);

				mesh.index_.push_back(vertex_idx);
				mesh.index_.push_back(vertex_idx + 1 + slice);
				mesh.index_.push_back(vertex_idx + slice);
			}
			theta += d_theta;
			++vertex_idx;
		}
		phy += d_phy;
	}

	mesh.vlist_local_[vertex_idx].pos.Set(0, radius, 0);
	mesh.vlist_local_[vertex_idx].color = Color(255, 0, 0);
	++vertex_idx;
	mesh.vlist_local_[vertex_idx].pos.Set(0, -radius, 0);
	mesh.vlist_local_[vertex_idx].color = Color(255, 0, 0);

	uint32_t next_index = 0;
	uint32_t diff = (stack - 2) * slice;
	for (uint32_t i = 0; i < slice; ++i)
	{
		next_index = i + 1;
		if (next_index == slice)
		{
			next_index = 0;
		}
		mesh.index_.push_back(next_index);
		mesh.index_.push_back(i);
		mesh.index_.push_back(mesh.num_vertices_ - 2);

		mesh.index_.push_back(diff + next_index);
		mesh.index_.push_back(diff + i);
		mesh.index_.push_back(mesh.num_vertices_ - 1);
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