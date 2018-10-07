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
		//���캯��
		KZMaterial(const Color& color, const std::string& texture_path, float ka, float kd, float ks, uint32_t power, uint32_t mip_level = 1);
		////�������캯��
		//KZMaterial(const KZMaterial& other);
		////��������
		//~KZMaterial();
		//�������������ɫ
		Color CalculateFinalColor(LightBase* light, const KZMath::KZVector4D<float>& vertex_pos, const KZMath::KZVector4D<float>& vertex_normal = KZMath::KZVector4D<float>(), const KZMath::KZVector4D<float>& camera_pos = KZMath::KZVector4D<float>());
		//��ȡ��ͼ��ɫ
		Color GetTextureColor(float s, float t, uint32_t level);
		//��ȡ������ͼ
		inline uint32_t GetBitMapWidth()
		{
			return bitmap_[0].GetWidth();
		}
		inline uint32_t GetBitMapHeight()
		{
			return bitmap_[0].GetHeight();
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
		inline uint32_t GetMipLevel()
		{
			return mip_level_;
		}
		//���ʵ�id
		uint32_t id_ = -1;
		//�Ƿ�����ͼ
		bool has_texture_ = false;
	private:
		//������ɫ
		Color color_;
		//������ͼ·��
		std::string texture_path_;
		//������ϵ��
		float ka_;
		//������ϵ��
		float kd_;
		//���淴��ϵ��
		float ks_;
		//�����Ȩֵ
		uint32_t power_;
		//mipmap level
		uint32_t mip_level_ = 1;
		//��ͼ����
		KZImage* bitmap_;
	};
}


#endif // !MATERIAL_H
