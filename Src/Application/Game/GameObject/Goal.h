#pragma once
#include "GameObject.h"

class Goal : public GameObject
{
public:
	Goal() {}
	~Goal() {}

	void Init() override;
	void Update() override;
	
	// 場所の設定
	void SetWorldMatrix(float x, float z, float y = 0.0f) { m_mWorld = Math::Matrix::CreateTranslation(x, y, z); }
	
	const GameObject::ClassID GetClassID() const override
	{
		return GameObject::eGoal;
	}

	bool CheckCollisionBump(const SphereInfo& info, BumpResult& result, float dis = 0) override;

private:
	
	void Release();
};
