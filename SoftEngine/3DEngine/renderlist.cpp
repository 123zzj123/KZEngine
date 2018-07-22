#include"renderlist.h"
using namespace KZEngine;

//¾ØÕó×ª»»
void KZRenderList::Transform(const KZMath::KZMatrix44& matrix)
{
	for (uint32_t i = 0; i < num_poly_; ++i) {
		for (uint32_t j = 0; j < 3; ++j) {
			tri_list_[i].vertex_list[j].pos = matrix * tri_list_[i].vertex_list[j].pos;
		}
	}
}

