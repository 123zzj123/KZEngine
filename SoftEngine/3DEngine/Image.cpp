#include"image.h"
using namespace KZEngine;

//默认构造函数
KZImage::KZImage() {
	pixel_ = nullptr;
	width_ = 0;
	height_ = 0;
}

//构造函数
KZImage::KZImage(const std::string& file_name) {
	//判断字符串是否为空
	assert(file_name != "");
	//判断文件存在且格式正确
	if (true) {
		std::wstring infilename = StringToWString(file_name);
		//读图片
		Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(infilename.c_str());

		height_ = bmp->GetHeight();
		width_ = bmp->GetWidth();
		Gdiplus::Color color;
		pixel_ = new Color[height_ * width_];
		uint32_t cur_idx = 0;
		for (uint32_t i = 0; i < height_; ++i) {
			for (uint32_t j = 0; j < width_; ++j) {
				bmp->GetPixel(j, i, &color);
				pixel_[cur_idx].r_ = color.GetRed();
				pixel_[cur_idx].g_ = color.GetGreen();
				pixel_[cur_idx].b_ = color.GetBlue();
				pixel_[cur_idx].a_ = color.GetAlpha();
				++cur_idx;
			}
		}
		delete bmp;
	}
	else
	{
		pixel_ = NULL;
		width_ = 0;
		height_ = 0;
	}
}

//拷贝构造函数
KZImage::KZImage(const KZImage& other)
{
	this->width_ = other.width_;
	this->height_ = other.height_;
	this->pixel_ = new Color[height_ * width_];
	uint32_t cur_idx = 0;
	for (uint32_t i = 0; i < height_; ++i) {
		for (uint32_t j = 0; j < width_; ++j) {
			pixel_[cur_idx] = other.pixel_[cur_idx];
			++cur_idx;
		}
	}
}

//移动赋值函数
KZImage& KZImage::operator=(KZImage&& other)
{
	delete[]pixel_;
	pixel_ = other.pixel_;
	width_ = other.width_;
	height_ = other.height_;
	other.pixel_ = nullptr;
	return *this;
}

//析构函数
KZImage::~KZImage() {
	if(pixel_ != NULL)
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

//检查文件存在且格式正确
bool KZImage::CheckFile(const std::string& file_name) {
	if (file_name == "") {
		return false;
	}
	std::string suffix;
	suffix = file_name.substr(file_name.find_last_of('.'));
	if (suffix == ".bmp" || suffix == ".gif" || suffix == ".jpg" || suffix == ".png" 
		|| suffix == ".tiff" || suffix == ".exif" || suffix == ".wmf" || suffix == ".emf")
	{
		std::ifstream in_stream;
		in_stream.open(file_name);
		if (!in_stream) {
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}

//设置mipmap链下一层的图片
void KZImage::SetMipMapImage(KZImage& next) const
{
	uint32_t next_width = width_ >> 1;
	uint32_t next_height = height_ >> 1;
	next.width_ = next_width;
	next.height_ = next_height;
	KZEngine::Color pixel_lu, pixel_ru, pixel_ld, pixel_rd;
	//float gamma = 1.01f;
	float factor = 0.26f;
	uint32_t cur_idx = 0;
	next.pixel_ = new Color[next_height * next_width];
	for (uint32_t i = 0; i < next_height; ++i)
	{
		for (uint32_t j = 0; j < next_width; ++j)
		{
			GetPixel(i * 2, j * 2, pixel_lu);
			GetPixel(i * 2, j * 2 + 1, pixel_ru);
			GetPixel(i * 2 + 1, j * 2, pixel_ld);
			GetPixel(i * 2 + 1, j * 2, pixel_rd);
			next.pixel_[cur_idx] = (pixel_lu + pixel_ru + pixel_ld + pixel_rd) * factor;
			++cur_idx;
		}
	}
}