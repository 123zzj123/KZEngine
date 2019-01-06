#include "pass.h"
using namespace KZEngine;

Pass::Pass()
{
	render_list_ = new KZRenderList();
}

Pass::~Pass() {
	delete render_list_;
	for (uint32_t i = 0; i < object_num_; ++i)
	{
		delete object_vec_[i];
		object_vec_[i] = nullptr;
	}
}