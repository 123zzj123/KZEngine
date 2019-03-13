#ifndef KZTERRIAN_H
#define KZTERRIAN_H

#include "object.h"
#include "image.h"
#include <queue>

namespace KZEngine {
	class KZPipeLine;

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
		uint32_t h_img_height_;
		uint32_t h_img_width_;
		KZImage height_map_;
	};

	class KZQuadTerrian : public KZTerrian
	{
	private:
		struct KZQuadTerrianNode
		{
			//�ڵ����Ķ�������
			uint32_t vertex_idx_;
			//�ڵ㼶��
			uint32_t level_;
			//�ڵ�߳�
			uint32_t side_len_;
			//�ڵ�ֲڳ̶�
			uint32_t rough_;
			//�ӽڵ�
			KZQuadTerrianNode* child_node_[4];
		};
		KZQuadTerrianNode root_node_;
		queue<KZQuadTerrianNode*> process_queue_[2];
		bool* node_state_table_;
		uint32_t max_level_;
	public:
		KZQuadTerrian(float width, float height, float vscale, const char* height_map_file_name, const char* texture_map_file_name,
			const KZEngine::Color& ini_color, const KZMath::KZVector4D<float>& world_pos, const KZMath::KZQuat& quat);
		//����mesh
		void UpdateMesh() override;
	protected:
		bool InSideView(KZQuadTerrianNode* node);
		bool EvalTerrianNode(KZQuadTerrianNode* node);
		void BuildQuadTree(KZQuadTerrianNode* root, uint32_t level, uint32_t diff_num);
		void IniQuadTreeRough(KZQuadTerrianNode* root, uint32_t level, uint32_t diff_num);
		void GetMaxLevel(uint32_t img_len);
	};
}

#endif