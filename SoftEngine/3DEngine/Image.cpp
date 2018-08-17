#include"image.h"
using namespace KZEngine;

//���캯��
KZImage::KZImage(const std::string& file_name) {
	//�ж��ַ����Ƿ�Ϊ��
	assert(file_name != "");
	//�ж��ļ������Ҹ�ʽ��ȷ
	if (CheckFile(file_name)) {
		std::wstring infilename = StringToWString(file_name);
		//��ͼƬ
		Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(infilename.c_str());

		height_ = bmp->GetHeight();
		width_ = bmp->GetWidth();
		Gdiplus::Color color;
		pixel_ = new Color[height_ * width_];
		for (uint32_t i = 0; i < height_; ++i) {
			for (uint32_t j = 0; j < width_; ++j) {
				bmp->GetPixel(j, i, &color);
				pixel_[i * width_ + j].r_ = color.GetRed();
				pixel_[i * width_ + j].g_ = color.GetGreen();
				pixel_[i * width_ + j].b_ = color.GetBlue();
				pixel_[i * width_ + j].a_ = color.GetAlpha();
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

//��������
KZImage::~KZImage() {
	if(pixel_ != NULL)
	delete[] pixel_;
}

//string ת wstring
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

//����ļ������Ҹ�ʽ��ȷ
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