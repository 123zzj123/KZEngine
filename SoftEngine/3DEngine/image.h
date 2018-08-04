#pragma once
#ifndef IMAGE_H
#define IMAGE_H
#include<iostream>
#include<Windows.h>
#include <gdiplus.h>
#include<string>

namespace KZEngine {
	//图片像素结构
	struct KZPixel
	{
		//红
		unsigned char red;
		//绿
		unsigned char green;
		//蓝
		unsigned char blue;
	};
	//图片类
	class KZImage
	{
	public:
		//构造函数
		KZImage(const std::string& file_name);
		//析构函数
		~KZImage();
		//获取宽度
		inline uint32_t GetWidth() { return width_; }
		//获取高度
		inline uint32_t GetHeight() { return height_; }
		//获取指定位置像素
		void GetPixel(uint32_t x, uint32_t y, KZPixel& pixel) { pixel = pixel_[y * width_ + x]; }
	private:
		//将string转wstring
		std::wstring StringToWString(const std::string& str);
	private:
		//宽度
		uint32_t width_;
		//高度
		uint32_t height_;
		//像素数组
		KZEngine::KZPixel* pixel_;
	};
}

#endif // !IMAGE_H
