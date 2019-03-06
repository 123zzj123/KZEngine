#ifndef KZTERRIAN_H
#define KZTERRIAN_H

#include "object.h"
#include "image.h"

namespace KZEngine {
	class KZTerrian : public KZObject
	{
	public:
		KZTerrian(float width, float height, float vscale, const char* height_map_file_name, const char* texture_map_file_name,
			const KZEngine::Color& ini_color, const KZMath::KZVector4D<float>& world_pos, const KZMath::KZQuat& quat);
	public:
		float width;
		float height;
		float vscale;
		uint32_t h_img_height_;
		uint32_t h_img_width_;
		KZImage height_map_;
	};
}

#endif