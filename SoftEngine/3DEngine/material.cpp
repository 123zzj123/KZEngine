#include"material.h"
using namespace KZEngine;

//构造函数
KZMaterial::KZMaterial(const std::string& mat_name, const Color& color, const std::string& texture_path, float ka, float kd, float ks, uint32_t power, uint32_t mip_level)
	:color_(color), texture_path_(texture_path), ka_(ka), kd_(kd), ks_(ks), power_(power), mip_level_(mip_level)
{
	BKDRHash(mat_name);
	KZImage source_img(texture_path_);
	bitmap_ = new KZImage[mip_level_]();
	bitmap_[0] = std::move(source_img);
	for (uint32_t i = 1; i < mip_level_; ++i)
	{
		bitmap_[i - 1].SetMipMapImage(bitmap_[i]);
	}
}

//拷贝构造函数
KZMaterial::KZMaterial(const KZMaterial& other)
{
	color_ = other.color_;
	texture_path_ = other.texture_path_;
	ka_ = other.ka_;
	kd_ = other.kd_;
	ks_ = other.ks_;
	power_ = other.power_;
	mip_level_ = other.mip_level_;
	hash_ = other.hash_;
	has_texture_ = other.has_texture_;
	id_ = other.id_;
	bitmap_ = new KZImage[mip_level_]();
	for (uint32_t i = 0; i < mip_level_; ++i)
	{
		bitmap_[i] = std::move(other.bitmap_[i]);
	}
}

//析构函数
KZMaterial::~KZMaterial() {
	delete[] bitmap_;
}

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
		return light_color * color_ * kd_ * vertex_normal.Vector3Dot(light_vec) + light_color * color_ * ks_ * (float)pow(max(reflect_vec.Vector3Dot(observe_vec), 0.0f), power_);
	}
}

//获取贴图颜色
void KZMaterial::GetTextureColor(float s, float t, uint32_t level, Color& color) {
	uint32_t x, y;
	uint32_t height = bitmap_[level].GetHeight();
	uint32_t width = bitmap_[level].GetWidth();
	if (width == 0 || height == 0) {
		return;
	}
	else
	{
		x = static_cast<int>(s * (width - 1));
		y = static_cast<int>(t * (height - 1));
		bitmap_[level].GetPixel(x, y, color);
		return;
	}
}

//计算并设置材质hash
void KZMaterial::BKDRHash(string str)
{
	unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;
	uint32_t size = (uint32_t)str.length();
	for (uint32_t i = 0; i < size; ++i)
	{
		hash = hash * seed + str[i];
	}

	hash_ = (hash & 0x7FFFFFFF);
	return;
}