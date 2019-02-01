#pragma once
#ifndef BSP_H
#define BSP_H

#include <vector>
#include "../KZMath/util.h"
#include "../KZMath/vector.h"
#include "renderlist.h"

namespace KZEngine {

	typedef struct BSPNodeTyp {
		vector<TrianglePtr> tri_list_;
		KZMath::KZPlane3D split_plane_;
		BSPNodeTyp* left_child_;
		BSPNodeTyp* right_child_;
	}BSPNode, *BSPNodePtr;

	// BSP�ָ�ƽ���б�
	struct BSPSplitPlane
	{
	public:
		KZMath::KZPlane3D plane_;
		TrianglePtr relative_tri;
		uint32_t id_ = 0;
		bool mark_ = false;
	public:
		BSPSplitPlane(const KZMath::KZPlane3D& plane, const TrianglePtr& tri = nullptr, const uint32_t id = 0) {
			plane_ = plane;
			relative_tri = tri;
			id_ = id;
			mark_ = true;
		}
	};

	class KZBSP {
	protected:
		static uint32_t bsp_plane_id;
		static vector<BSPSplitPlane> split_plane_vec_;
	public:
		//���쳡��BSP��
		static void BuildSceneBSPTree(BSPNodePtr& bhv_root, uint32_t level);
		//BHV���޳�
		static void BSPTreeCulling(BSPNodePtr bhv_root);
		//�������BSP��
		static void CleanSceneBSPTree(BSPNodePtr& bhv_root);
	protected:
		static void ChooseBestSplitPlane(BSPNodePtr& bhv_root);
		static void IniSplitPlaneVec();
	};
}

#endif