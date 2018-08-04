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
		KZMaterial(const Color& color, const std::string& texture_path, float ka, float kd, float ks, float power);
		//析构函数
		~KZMaterial();
		//计算光照最终颜色
		Color CalculateFinalColor(LightBase* light, const KZMath::KZVector4D& vertex_pos, const KZMath::KZVector4D& vertex_normal = KZMath::KZVector4D(), const KZMath::KZVector4D& camera_pos = KZMath::KZVector4D());
		//获取贴图颜色
		Color GetTextureColor(float s, float t);
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
		float power_;
		//贴图数据
		KZImage bitmap_;
	};
}


#endif // !MATERIAL_H
