#pragma once
#ifndef IMAGE_H
#define IMAGE_H
#include<iostream>
#include<Windows.h>
#include <gdiplus.h>
#include<string>
#include<fstream>
#include<assert.h>
#include"light.h"

namespace KZEngine {
	//ͼƬ��
	class KZImage
	{
	public:
		//Ĭ�Ϲ��캯��
		KZImage();
		//���캯��
		KZImage(const std::string& file_name);
		//�������캯��
		KZImage(const KZImage& other);
		//�ƶ���ֵ
		KZImage& operator=(KZImage&& other);
		//��������
		~KZImage();
		//��ȡ���
		inline uint32_t GetWidth() const{ return width_; }
		//��ȡ�߶�
		inline uint32_t GetHeight() const{ return height_; }
		//��ȡָ��λ������,����
		inline void GetPixel(uint32_t x, uint32_t y, Color& pixel) const { pixel = pixel_[y * width_ + x]; }
		//��ȡָ��λ������,��������
		inline void GetPixel(uint32_t idx, Color& pixel) const { pixel = pixel_[idx]; }
		//��ȡָ��λ�õ�ͨ������ɫֵ,����
		inline void GetSingleChannelColor(uint32_t x, uint32_t y, uint32_t& value) const { value = (uint32_t)pixel_[y * width_ + x].r_; }
		//��ȡָ��λ�õ�ͨ������ɫֵ,��������
		inline void GetSingleChannelColor(uint32_t idx, uint32_t& value) const { value = (uint32_t)pixel_[idx].r_; }
		//����mipmap����һ���ͼƬ
		void SetMipMapImage(KZImage& next) const;
	private:
		//��stringתwstring
		std::wstring StringToWString(const std::string& str);
		//����ļ������Ҹ�ʽ��ȷ
		bool CheckFile(const std::string& file_name);
	private:
		//���
		uint32_t width_;
		//�߶�
		uint32_t height_;
		//��������
		KZEngine::Color* pixel_;
	};
}

#endif // !IMAGE_H
