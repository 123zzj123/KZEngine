#include"light.h"
using namespace KZEngine;

//��ɫ�ӷ�
ColorTyp KZEngine::ColorTyp::operator+ (const ColorTyp& c) const {
	Color res;
	res.r_ = r_ + c.r_;
	res.g_ = g_ + c.g_;
	res.b_ = b_ + c.b_;
	res.a_ = 255;
	return res;
}

void KZEngine::ColorTyp::operator+= (const ColorTyp& c) {
	r_ += c.r_;
	g_ += c.g_;
	b_ += c.b_;
	a_ = 255;
}

//��ɫ�˷�
ColorTyp KZEngine::ColorTyp::operator* (const ColorTyp& c) const {
	Color res;
	res.r_ = r_ * c.r_ >> 8;
	res.g_ = g_ * c.g_ >> 8;
	res.b_ = b_ * c.b_ >> 8;
	res.a_ = 255;
	return res;
}

//��ɫ�˷�
ColorTyp KZEngine::ColorTyp::operator* (float num) const {
	int r_base = r_;
	int g_base = g_;
	int b_base = b_;
	int factor = static_cast<int>(num * 128);
	Color res;
	res.r_ = (r_base * factor) >> 7;
	res.g_ = (g_base * factor) >> 7;
	res.b_ = (b_base * factor) >> 7;
	res.a_ = 255;
	return res;
}

//��ɫ����ֵ�˷�
void KZEngine::ColorTyp::operator*= (float num) {
	int r_base = r_;
	int g_base = g_;
	int b_base = b_;
	int factor = static_cast<int>(num * 128);
	r_ = (r_base * factor) >> 7;
	g_ = (g_base * factor) >> 7;
	b_ = (b_base * factor) >> 7;
	a_ = 255;
	return;
}

//��ȡ����ǿ��
Color LightBase::CalculateLightIntensity(const KZMath::KZVector4D<float>& vertex_pos, const KZMath::KZVector4D<float>& vertex_normal) {
	return light_colr_;
}

//��ȡ��Դ����
KZMath::KZVector4D<float> LightBase::GetLightVec(const KZMath::KZVector4D<float>& vertex_pos) {
	return KZMath::KZVector4D<float>(0, 0, 0);
}

//��ȡ��Դ����
KZMath::KZVector4D<float> DirectionLight::GetLightVec(const KZMath::KZVector4D<float>& vertex_pos) {
	return -dir_;
}

//�������ǿ��
Color DirectionLight::CalculateLightIntensity(const KZMath::KZVector4D<float>& vertex_pos, const KZMath::KZVector4D<float>& vertex_normal) {
	float dot = vertex_normal.Vector3Dot(-dir_);
	if (dot > 0) {
		Color color = light_colr_;
		color *= dot;
		return color;
	}
	else
	{
		return Color(0, 0, 0, 255);
	}
}

//�����Դǿ��
Color PointLight::CalculateLightIntensity(const KZMath::KZVector4D<float>& vertex_pos, const KZMath::KZVector4D<float>& vertex_normal)
{
	KZMath::KZVector4D<float>& light_vec = pos_ - vertex_pos;
	float dot = vertex_normal.Vector3Dot(light_vec);
	if (dot > 0) {
		//��һ�����ӺͶ�������Ϊ0�򻯼���
		if (kl_ == 0 && kq_ == 0)
		{
			Color color = light_colr_;
			float atten = 1 / kc_;
			color *= atten;
			return color;
		}
		else
		{
			float dist = light_vec.Vector3Length();
			Color color = light_colr_;
			float atten = 1 / (kc_ + kl_ * sqrt(dist) + kq_ * dist);
			color *= atten;
			return color;
		}
	}
	else
	{
		return Color(0, 0, 0, 255);
	}
}

//��ȡ��Դ����
KZMath::KZVector4D<float> PointLight::GetLightVec(const KZMath::KZVector4D<float>& vertex_pos) {
	return pos_ - vertex_pos;
}

//�����Դǿ��
Color SimpleSpotLight::CalculateLightIntensity(const KZMath::KZVector4D<float>& vertex_pos, const KZMath::KZVector4D<float>& vertex_normal)
{
	KZMath::KZVector4D<float>& light_vec = pos_ - vertex_pos;
	float dot = vertex_normal.Vector3Dot(light_vec);
	if (dot > 0) {
		float dist = light_vec.Vector3Length();
		light_vec.Vector3Normalize();
		Color color = light_colr_;
		float atten = 1 / (kc_ + kl_ * sqrt(dist) + kq_ * dist);
		float cos_theta = (-light_vec).Vector3Dot(dir_);
		color *= (cos_theta * atten);
		return color;
	}
	else
	{
		return Color(0, 0, 0, 255);
	}
}

//��ȡ��Դ����
KZMath::KZVector4D<float> SimpleSpotLight::GetLightVec(const KZMath::KZVector4D<float>& vertex_pos) {
	return pos_ - vertex_pos;
}

//�����Դǿ��
Color ComplexSpotLight::CalculateLightIntensity(const KZMath::KZVector4D<float>& vertex_pos, const KZMath::KZVector4D<float>& vertex_normal)
{
	KZMath::KZVector4D<float>& light_vec = vertex_pos - pos_;
	float angle = dir_.Vector3AngleBetweenVectors(light_vec);
	//λ�ڰ�Ӱ��
	if (angle > spot_outter_angle_) {
		return Color(0, 0, 0, 255);
	}
	//λ�ڰ�Ӱ��
	else if (angle < spot_outter_angle_ && angle > spot_inner_angle_) {
		float dist = light_vec.Vector3Length();
		Color color = light_colr_;
		float dis_atten = 1 / (kc_ + kl_ * sqrt(dist) + kq_ * dist);
		float angle_atten = pow(cos(KZMath::AngleToRadian(angle)) - cos(KZMath::AngleToRadian(spot_outter_angle_)), pf_) / (cos(KZMath::AngleToRadian(spot_inner_angle_)) - cos(KZMath::AngleToRadian(spot_outter_angle_)));
		color *= (dis_atten * angle_atten);
		return color;
	}
	//λ�ڱ�Ӱ��
	else
	{
		float dist = light_vec.Vector3Length();
		Color color = light_colr_;
		float atten = 1 / (kc_ + kl_ * sqrt(dist) + kq_ * dist);
		color *= atten;
		return color;
	}
}

//��ȡ��Դ����
KZMath::KZVector4D<float> ComplexSpotLight::GetLightVec(const KZMath::KZVector4D<float>& vertex_pos) {
	return pos_ - vertex_pos;
}