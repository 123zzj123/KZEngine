#pragma once
#ifndef BHV_H
#define BHV_H

#include "../KZMath/vector.h"
#include "object.h"

namespace KZEngine {
	typedef struct BHVNodeTyp {
		int num_object_ = 0;
		KZMath::KZVector4D<float> origin_pos_;
		KZMath::KZVector4D<float> world_pos_;
		KZMath::KZVector4D<float> radius_;
		vector<KZObject*> obj_list_;
		int num_children_ = 0;
		BHVNodeTyp** bhv_child;
	}BHVNode, *BHVNodePtr;

	class KZBHV 
	{
	private:
		//BHV��������
		static const uint32_t kMaxBhvLevel = 5;
		//BHV���ڵ�����������
		static const uint32_t kMinBhvObject = 4;
	public:
		//���쳡��BHV��
		static void BuildSceneBHVTree(BHVNodePtr& bhv_root, uint32_t level, uint32_t num_divisions);
		//BHV���޳�
		static void BHVTreeCulling(BHVNodePtr bhv_root);
		//���BHV������
		static void ClearSceneBHVTree(BHVNodePtr& bhv_root_);
	};
}

#endif