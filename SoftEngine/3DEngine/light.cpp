#include"light.h"
using namespace KZEngine;

//颜色加法
ColorTyp KZEngine::ColorTyp::operator+ (const ColorTyp& c) const {
	Color res;
	res.r_ = r_ + c.r_;
	res.r_ = res.r_ > 255 ? 255 : res.r_;
	res.g_ = g_ + c.g_;
	res.g_ = res.g_ > 255 ? 255 : res.g_;
	res.b_ = b_ + c.b_;
	res.b_ = res.b_ > 255 ? 255 : res.b_;
	res.a_ = 255;
	return res;
}

//颜色乘法
ColorTyp KZEngine::ColorTyp::operator* (const ColorTyp& c) const {
	Color res;
	res.r_ = r_ * c.r_ / 255;
	res.g_ = g_ * c.g_ / 255;
	res.b_ = b_ * c.b_ / 255;
	res.a_ = 255;
	return res;
}

//颜色浮点值乘法
void KZEngine::ColorTyp::operator*= (float num) {
	r_ *= num;
	r_ = r_ > 255 ? 255 : r_;
	g_ *= num;
	g_ = g_ > 255 ? 255 : g_;
	b_ *= num;
	b_ = b_ > 255 ? 255 : b_;
	a_ = 255;
	return;
}

//获取光照强度
Color LightBase::CalculateLightIntensity(const KZMath::KZVector4D& vertex_pos, const KZMath::KZVector4D& vertex_normal) {
	return light_colr_;
}

//获取光源向量
KZMath::KZVector4D LightBase::GetLightVec(const KZMath::KZVector4D& vertex_pos) {
	return KZMath::KZVector4D(0, 0, 0);
}

//计算光照强度
Color DirectionLight::CalculateLightIntensity(const KZMath::KZVector4D& vertex_pos, const KZMath::KZVector4D& vertex_normal) {
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

//计算光源强度
Color PointLight::CalculateLightIntensity(const KZMath::KZVector4D& vertex_pos, const KZMath::KZVector4D& vertex_normal)
{
	KZMath::KZVector4D& light_vec = pos_ - vertex_pos;
	float dot = vertex_normal.Vector3Dot(light_vec);
	if (dot > 0) {
		float dist = light_vec.Vector3Length();
		Color color = light_colr_;
		float atten = 1 / (kc_ + kl_ * sqrt(dist) + kq_ * dist);
		color *= atten;
		return color;
	}
	else
	{
		return Color(0, 0, 0, 255);
	}
}

//获取光源向量
KZMath::KZVector4D PointLight::GetLightVec(const KZMath::KZVector4D& vertex_pos) {
	return pos_ - vertex_pos;
}

//计算光源强度
Color SimpleSpotLight::CalculateLightIntensity(const KZMath::KZVector4D& vertex_pos, const KZMath::KZVector4D& vertex_normal)
{
	KZMath::KZVector4D& light_vec = pos_ - vertex_pos;
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

//获取光源向量
KZMath::KZVector4D SimpleSpotLight::GetLightVec(const KZMath::KZVector4D& vertex_pos) {
	return pos_ - vertex_pos;
}

//计算光源强度
Color ComplexSpotLight::CalculateLightIntensity(const KZMath::KZVector4D& vertex_pos, const KZMath::KZVector4D& vertex_normal)
{
	KZMath::KZVector4D& light_vec = vertex_pos - pos_;
	float angle = dir_.Vector3AngleBetweenVectors(light_vec);
	//位于半影外
	if (angle > spot_outter_angle_) {
		return Color(0, 0, 0, 255);
	}
	//位于半影内
	else if (angle < spot_outter_angle_ && angle > spot_inner_angle_) {
		float dist = light_vec.Vector3Length();
		Color color = light_colr_;
		float dis_atten = 1 / (kc_ + kl_ * sqrt(dist) + kq_ * dist);
		float angle_atten = pow(cos(KZMath::AngleToRadian(angle)) - cos(KZMath::AngleToRadian(spot_outter_angle_)), pf_) / (cos(KZMath::AngleToRadian(spot_inner_angle_)) - cos(KZMath::AngleToRadian(spot_outter_angle_)));
		color *= (dis_atten * angle_atten);
		return color;
	}
	//位于本影内
	else
	{
		float dist = light_vec.Vector3Length();
		Color color = light_colr_;
		float atten = 1 / (kc_ + kl_ * sqrt(dist) + kq_ * dist);
		color *= atten;
		return color;
	}
}

//获取光源向量
KZMath::KZVector4D ComplexSpotLight::GetLightVec(const KZMath::KZVector4D& vertex_pos) {
	return pos_ - vertex_pos;
}