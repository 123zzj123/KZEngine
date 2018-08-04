#pragma once
#ifndef IMAGE_H
#define IMAGE_H
#include<iostream>
#include<Windows.h>
#include <gdiplus.h>
#include<string>

namespace KZEngine {
	//ͼƬ���ؽṹ
	struct KZPixel
	{
		//��
		unsigned char red;
		//��
		unsigned char green;
		//��
		unsigned char blue;
	};
	//ͼƬ��
	class KZImage
	{
	public:
		//���캯��
		KZImage(const std::string& file_name);
		//��������
		~KZImage();
		//��ȡ���
		inline uint32_t GetWidth() { return width_; }
		//��ȡ�߶�
		inline uint32_t GetHeight() { return height_; }
		//��ȡָ��λ������
		void GetPixel(uint32_t x, uint32_t y, KZPixel& pixel) { pixel = pixel_[y * width_ + x]; }
	private:
		//��stringתwstring
		std::wstring StringToWString(const std::string& str);
	private:
		//���
		uint32_t width_;
		//�߶�
		uint32_t height_;
		//��������
		KZEngine::KZPixel* pixel_;
	};
}

#endif // !IMAGE_H
