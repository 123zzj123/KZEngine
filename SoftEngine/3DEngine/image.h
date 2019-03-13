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
	//图片类
	class KZImage
	{
	public:
		//默认构造函数
		KZImage();
		//构造函数
		KZImage(const std::string& file_name);
		//拷贝构造函数
		KZImage(const KZImage& other);
		//移动赋值
		KZImage& operator=(KZImage&& other);
		//析构函数
		~KZImage();
		//获取宽度
		inline uint32_t GetWidth() const{ return width_; }
		//获取高度
		inline uint32_t GetHeight() const{ return height_; }
		//获取指定位置像素,行列
		inline void GetPixel(uint32_t x, uint32_t y, Color& pixel) const { pixel = pixel_[y * width_ + x]; }
		//获取指定位置像素,顶点索引
		inline void GetPixel(uint32_t idx, Color& pixel) const { pixel = pixel_[idx]; }
		//获取指定位置单通道的颜色值,行列
		inline void GetSingleChannelColor(uint32_t x, uint32_t y, uint32_t& value) const { value = (uint32_t)pixel_[y * width_ + x].r_; }
		//获取指定位置单通道的颜色值,顶点索引
		inline void GetSingleChannelColor(uint32_t idx, uint32_t& value) const { value = (uint32_t)pixel_[idx].r_; }
		//设置mipmap链下一层的图片
		void SetMipMapImage(KZImage& next) const;
	private:
		//将string转wstring
		std::wstring StringToWString(const std::string& str);
		//检查文件存在且格式正确
		bool CheckFile(const std::string& file_name);
	private:
		//宽度
		uint32_t width_;
		//高度
		uint32_t height_;
		//像素数组
		KZEngine::Color* pixel_;
	};
}

#endif // !IMAGE_H
