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
		KZMaterial(const Color& color, const std::string& texture_path, float ka, float kd, float ks, float power);
		//��������
		~KZMaterial();
		//�������������ɫ
		Color CalculateFinalColor(LightBase* light, const KZMath::KZVector4D& vertex_pos, const KZMath::KZVector4D& vertex_normal = KZMath::KZVector4D(), const KZMath::KZVector4D& camera_pos = KZMath::KZVector4D());
		//��ȡ��ͼ��ɫ
		Color GetTextureColor(float s, float t);
		//���ʵ�id
		uint32_t id_;
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
		float power_;
		//��ͼ����
		KZImage bitmap_;
	};
}


#endif // !MATERIAL_H