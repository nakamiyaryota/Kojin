#pragma once
#include "GameObject.h"

class Effect2D;

class StageMap : public GameObject
{
public:
	StageMap() {}
	~StageMap() {}

	void Init() override;
	void FirstStageInit();
	void SecondStageInit();

	// 「ステージ」としてクラスIDを返す
	const GameObject::ClassID GetClassID() const override
	{
		return GameObject::eStage;
	}

	// StageMapでは球対球の判定ではなく、球対メッシュの判定にする
	bool CheckCollisionBump(const SphereInfo& info, BumpResult& result, float dis = 0) override;

private:
	void Release();

	json11::Json m_jsonObjFileName = nullptr;
	json11::Json m_jsonObjPosition = nullptr;

	const int ENEMY_SIZE = 3;
	const int OBJECT_SIZE = 20;
	const int OBJECTFIX_SIZE = 1;
	const int LIFT_SIZE = 4;
};
