#include"material.h"
using namespace KZEngine;

//构造函数
KZMaterial::KZMaterial(const Color& color, const std::string& texture_path, float ka, float kd, float ks, float power)
	:color_(color), texture_path_(texture_path), ka_(ka), kd_(kd), ks_(ks), power_(power), bitmap_(texture_path_)
{

}

//析构函数
KZMaterial::~KZMaterial() {
}

//计算光照最终颜色
Color KZMaterial::CalculateFinalColor(LightBase* light, const KZMath::KZVector4D& vertex_pos, const KZMath::KZVector4D& vertex_normal, const KZMath::KZVector4D& camera_pos) {
	if (light->GetType() == LightType::AMBIENT) {
		//只计算环境光
		return light->CalculateLightIntensity(vertex_pos, vertex_normal) * color_;
	}
	else
	{
		Color light_color = light->CalculateLightIntensity(vertex_pos, vertex_normal);
		KZMath::KZVector4D light_vec = light->GetLightVec(vertex_pos);
		KZMath::KZVector4D reflect_vec = (vertex_normal * (2.0f * light_vec.Vector3Dot(vertex_normal))) - light_vec;
		KZMath::KZVector4D observe_vec = camera_pos - vertex_pos;
		//漫反射与镜面反射
		return light_color * color_ * kd_ * vertex_normal.Vector3Dot(light_vec) + light_color * color_ * ka_ * pow(max(reflect_vec.Vector3Dot(observe_vec), 0.0f), power_);
	}
}

//获取贴图颜色
Color KZMaterial::GetTextureColor(float s, float t) {
	uint32_t x, y;
	uint32_t width = bitmap_.GetHeight();
	uint32_t height = bitmap_.GetWidth();
	x = s * (width - 1);
	y = t * (height - 1);
	KZPixel pixel;
	bitmap_.GetPixel(x, y, pixel);
	Color result;
	result.Set(pixel.red, pixel.green, pixel.blue);
	return result;
}