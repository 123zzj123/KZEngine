#include"renderlist.h"
using namespace KZEngine;

KZRenderList::~KZRenderList() {
	uint32_t len = tri_list_.size();
	for (uint32_t i = 0; i < len; ++i) {
		delete tri_list_[i];
		tri_list_[i] = nullptr;
	}
}

//¾ØÕó×ª»»
void KZRenderList::Transform(const KZMath::KZMatrix44& matrix)
{
	uint32_t len = tri_list_.size();
	for (uint32_t i = 0; i < len; ++i) {
		for (uint32_t j = 0; j < 3; ++j) {
			tri_list_[i]->vertex_list[j].pos = matrix * tri_list_[i]->vertex_list[j].pos;
		}
	}
}

