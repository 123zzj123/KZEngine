#include"material.h"
using namespace KZEngine;

//构造函数
KZMaterial::KZMaterial(const Color& color, const std::string& texture_path, float ka, float kd, float ks, uint32_t power, uint32_t mip_level)
	:color_(color), texture_path_(texture_path), ka_(ka), kd_(kd), ks_(ks), power_(power), mip_level_(mip_level)
{
	KZImage source_img(texture_path_);
	bitmap_ = new KZImage[mip_level_]();
	bitmap_[0] = std::move(source_img);
	for (uint32_t i = 1; i < mip_level_; ++i)
	{
		bitmap_[i - 1].SetMipMapImage(bitmap_[i]);
	}
}

////拷贝构造函数
//KZMaterial::KZMaterial(const KZMaterial& other)
//{
//	*this = other;
//}
//
////析构函数
//KZMaterial::~KZMaterial() {
//}

//计算光照最终颜色
Color KZMaterial::CalculateFinalColor(LightBase* light, const KZMath::KZVector4D<float>& vertex_pos, const KZMath::KZVector4D<float>& vertex_normal, const KZMath::KZVector4D<float>& camera_pos) {
	if (light->GetType() == LightType::AMBIENT) {
		//只计算环境光
		return light->CalculateLightIntensity(vertex_pos, vertex_normal) * color_ * ka_;
	}
	else
	{
		Color light_color = light->CalculateLightIntensity(vertex_pos, vertex_normal);
		KZMath::KZVector4D<float> light_vec = light->GetLightVec(vertex_pos);
		KZMath::KZVector4D<float> reflect_vec = (vertex_normal * (2.0f * light_vec.Vector3Dot(vertex_normal))) - light_vec;
		KZMath::KZVector4D<float> observe_vec = camera_pos - vertex_pos;
		//漫反射与镜面反射
		return light_color * color_ * kd_ * vertex_normal.Vector3Dot(light_vec) + light_color * color_ * ks_ * pow(max(reflect_vec.Vector3Dot(observe_vec), 0.0f), power_);
	}
}

//获取贴图颜色
Color KZMaterial::GetTextureColor(float s, float t, uint32_t level) {
	uint32_t x, y;
	uint32_t height = bitmap_[level].GetHeight();
	uint32_t width = bitmap_[level].GetWidth();
	Color result;
	if (width == 0 || height == 0) {
		return result;
	}
	else
	{
		x = static_cast<int>(s * (width - 1));
		y = static_cast<int>(t * (height - 1));
		bitmap_[level].GetPixel(x, y, result);
		return result;
	}
}