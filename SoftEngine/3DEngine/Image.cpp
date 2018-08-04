#include"image.h"
using namespace KZEngine;

//构造函数
KZImage::KZImage(const std::string& file_name) {
	std::wstring infilename = StringToWString(file_name);
	//读图片
	Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(infilename.c_str());

	height_ = bmp->GetHeight();
	width_ = bmp->GetWidth();
	Gdiplus::Color color;
	pixel_ = new KZPixel[height_ * width_];
	for (uint32_t i = 0; i < height_; ++i) {
		for (uint32_t j = 0; j < width_; ++j) {
			bmp->GetPixel(j, i, &color);
			pixel_[i * width_ + j].red = color.GetRed();
			pixel_[i * width_ + j].green = color.GetGreen();
			pixel_[i * width_ + j].blue = color.GetBlue();
		}
	}
	delete bmp;
}

//析构函数
KZImage::~KZImage() {
	delete[] pixel_;
}

//string 转 wstring
std::wstring KZImage::StringToWString(const std::string& str)
{
	setlocale(LC_ALL, "chs");
	const char* point_to_source = str.c_str();
	size_t new_size = str.size() + 1;
	wchar_t *point_to_destination = new wchar_t[new_size];
	wmemset(point_to_destination, 0, new_size);
	size_t converted = 0;
	mbstowcs_s(&converted, point_to_destination, new_size, point_to_source, _TRUNCATE);
	std::wstring result = point_to_destination;
	delete[]point_to_destination;
	setlocale(LC_ALL, "C");
	return result;
}