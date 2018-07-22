#pragma once
#ifndef KZ_PIPELINE
#define KZ_PIPELINE

#include<vector>;
#include"object.h"
#include"renderlist.h"
#include"camera.h"

namespace KZEngine {
	class KZPipeLine
	{
	public:
		//��ȡ��ɾ����ʵ��
		KZPipeLine* GetInstance() {
			if (instance_ == nullptr) {
				instance_ = new KZPipeLine();
			}
			return instance_;
		}

		//ɾ����ʵ��
		void DeleteInstance() {
			if (instance_) {
				delete instance_;
			}
		}
		//���ļ��м���
		void LoadFromFile();
		//������������Χ�����
		void CullObject();
		//��������
		void RemoveBackface();
		void Initial();
	public:
		uint32_t object_num_;
		//�洢��������
		vector<KZObject> object_vec_;
		//��Ⱦ����
		KZRenderList* render_list_;
		KZCamera main_camera_;
		vector<bool> object_active_;
	private:
		KZPipeLine* instance_ = nullptr;
		KZPipeLine();
	};
}
#endif // !KZ_PIPELINE
