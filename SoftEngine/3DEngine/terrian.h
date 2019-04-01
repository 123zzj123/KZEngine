#ifndef KZTERRIAN_H
#define KZTERRIAN_H

#include "object.h"
#include "image.h"
#include "camera.h"
#include <queue>

namespace KZEngine {
	const float kFactor = 50.0f;

	class KZTerrian : public KZObject
	{
	public:
		KZTerrian(float width, float height, float vscale, const char* height_map_file_name, const char* texture_map_file_name,
			const KZEngine::Color& ini_color, const KZMath::KZVector4D<float>& world_pos, const KZMath::KZQuat& quat);
		KZTerrian(const char* height_map_file_name):height_map_(height_map_file_name){};
	public:
		float width_;
		float height_;
		float vscale_;
		float col_step_;
		float row_step_;
		uint32_t h_img_height_;
		uint32_t h_img_width_;
		KZImage height_map_;
	};

	class KZQuadTerrian : public KZTerrian
	{
	private:
		struct KZQuadTerrianNode
		{
			//节点中心顶点索引
			uint32_t vertex_idx_;
			//节点级别
			uint32_t level_;
			//节点边长
			uint32_t side_len_;
			//节点粗糙程度
			uint32_t rough_;
			//与左上角右下角节点序号差值
			uint32_t diff_num_;
			//子节点
			KZQuadTerrianNode* child_node_[4];
		};
		KZQuadTerrianNode* root_node_;
		queue<KZQuadTerrianNode*> process_queue_[2];
		bool* node_state_table_;
		uint32_t max_level_;
		KZMath::KZQuat quat_;
	public:
		KZQuadTerrian(float width, float height, float vscale, const char* height_map_file_name, const char* texture_map_file_name,
			const KZEngine::Color& ini_color, const KZMath::KZVector4D<float>& world_pos, const KZMath::KZQuat& quat);
		//更新mesh
		void UpdateMesh() override;
	protected:
		bool EvalNeighbor(KZQuadTerrianNode* node);
		bool EvalTerrianNode(KZQuadTerrianNode* node, bool& inside);
		void BuildQuadTree(KZQuadTerrianNode* root, uint32_t level, uint32_t diff_num);
		void IniQuadTreeRough(KZQuadTerrianNode* root, uint32_t level, uint32_t diff_num);
		void GetMaxLevel(uint32_t img_len);
		void AddNodeToMesh(KZQuadTerrianNode* node);
		bool CheckHeightMap();
	};
}

#endif