#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include"light.h"
#include"image.h"

namespace KZEngine {

	class KZMaterial
	{
	public:
		KZMaterial() = default;
		//构造函数
		KZMaterial(const std::string& mat_name, const Color& color, const std::string& texture_path, float ka, float kd, float ks, uint32_t power, uint32_t mip_level = 1);
		//拷贝构造函数
		KZMaterial(const KZMaterial& other);
		//析构函数
		~KZMaterial();
		//计算光照最终颜色
		Color CalculateFinalColor(LightBase* light, const KZMath::KZVector4D<float>& vertex_pos, const KZMath::KZVector4D<float>& vertex_normal = KZMath::KZVector4D<float>(), const KZMath::KZVector4D<float>& camera_pos = KZMath::KZVector4D<float>());
		//获取贴图颜色
		void GetTextureColor(float s, float t, uint32_t level, Color& color);
		//获取材质贴图
		inline uint32_t GetBitMapWidth() const
		{
			return bitmap_[0].GetWidth();
		}
		inline uint32_t GetBitMapHeight() const
		{
			return bitmap_[0].GetHeight();
		}
		inline bool operator<(const KZMaterial& other) const {
			if (id_ == other.id_)
			{
				return false;
			}
			else
			{
				return id_ < other.id_;
			}
		}
		inline uint32_t GetMipLevel() const
		{
			return mip_level_;
		}
		inline uint32_t GetHashCode() const
		{
			return hash_;
		}
		//材质的id
		uint32_t id_ = -1;
		//是否有贴图
		bool has_texture_ = false;
	private:
		//计算并设置材质hash
		void BKDRHash(string str);
	private:
		//材质颜色
		Color color_;
		//纹理贴图路径
		std::string texture_path_;
		//环境光系数
		float ka_;
		//漫反射系数
		float kd_;
		//镜面反射系数
		float ks_;
		//镜面光权值
		uint32_t power_;
		//mipmap level
		uint32_t mip_level_ = 1;
		//材质hash值
		uint32_t hash_;
		//贴图数据
		KZImage* bitmap_;
	};
}


#endif // !MATERIAL_H
