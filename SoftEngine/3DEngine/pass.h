#pragma once
#ifndef PASS_H
#define PASS_H

#include "renderlist.h"
#include<vector>

namespace KZEngine {
	class Pass
	{
	public:
		Pass();
		~Pass();
	public:
		int32_t id = -1;
		uint32_t object_num_ = 0;
		uint32_t tri_num_ = 0;
		//存储Pass的物体数组
		vector<KZObject*> object_vec_;
		KZRenderList* render_list_ = nullptr;
	};
}
#endif