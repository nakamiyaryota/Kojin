#pragma once
#include "../../GameObject.h"

class Effect2D;

class TitleStageMap : public GameObject
{
public:
	TitleStageMap() {}
	~TitleStageMap() {}

	void Init() override;

	// �u�X�e�[�W�v�Ƃ��ăN���XID��Ԃ�
	const GameObject::ClassID GetClassID() const override
	{
		return GameObject::eStage;
	}

	// StageMap�ł͋��΋��̔���ł͂Ȃ��A���΃��b�V���̔���ɂ���
	bool CheckCollisionBump(const SphereInfo& info, BumpResult& result, float dis = 0) override;

private:
	void Release();

	const int OBJECT_SIZE = 1;
	const int LIFT_SIZE = 0;
};
