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
		//构造函数
		KZMaterial(const Color& color, const std::string& texture_path, float ka, float kd, float ks, uint32_t power);
		////拷贝构造函数
		//KZMaterial(const KZMaterial& other);
		////析构函数
		//~KZMaterial();
		//计算光照最终颜色
		Color CalculateFinalColor(LightBase* light, const KZMath::KZVector4D<float>& vertex_pos, const KZMath::KZVector4D<float>& vertex_normal = KZMath::KZVector4D<float>(), const KZMath::KZVector4D<float>& camera_pos = KZMath::KZVector4D<float>());
		//获取贴图颜色
		Color GetTextureColor(float s, float t);
		//获取材质贴图
		inline uint32_t GetBitMapWidth()
		{
			return bitmap_.GetWidth();
		}
		inline uint32_t GetBitMapHeight()
		{
			return bitmap_.GetHeight();
		}
		inline bool operator<(const KZMaterial& other)const {
			if (id_ == other.id_)
			{
				return false;
			}
			else
			{
				return id_ < other.id_;
			}
		}
		//材质的id
		uint32_t id_;
		//是否有贴图
		bool has_texture_ = false;
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
		int power_;
		//贴图数据
		KZImage bitmap_;
	};
}


#endif // !MATERIAL_H
