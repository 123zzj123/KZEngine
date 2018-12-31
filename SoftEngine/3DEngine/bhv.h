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
		//BHV树最大层数
		static const uint32_t kMaxBhvLevel = 5;
		//BHV树节点最少物体数
		static const uint32_t kMinBhvObject = 4;
	public:
		//建造场景BHV树
		static void BuildSceneBHVTree(BHVNodePtr& bhv_root, uint32_t level, uint32_t num_divisions);
		//BHV树剔除
		static void BHVTreeCulling(BHVNodePtr bhv_root);
		//清除BHV场景树
		static void ClearSceneBHVTree(BHVNodePtr& bhv_root_);
	};
}

#endif