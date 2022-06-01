#pragma once
#include "../../GameObject.h"

class Effect2D;

class TitleStageMap : public GameObject
{
public:
	TitleStageMap() {}
	~TitleStageMap() {}

	void Init() override;

	// 「ステージ」としてクラスIDを返す
	const GameObject::ClassID GetClassID() const override
	{
		return GameObject::eStage;
	}

	// StageMapでは球対球の判定ではなく、球対メッシュの判定にする
	bool CheckCollisionBump(const SphereInfo& info, BumpResult& result, float dis = 0) override;

private:
	void Release();

	const int OBJECT_SIZE = 1;
	const int LIFT_SIZE = 0;
};
