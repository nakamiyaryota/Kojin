#pragma once

#include "GameObject.h"

class LiftFix : public GameObject
{
public:
	LiftFix() {}
	~LiftFix() {}

	void Init() override;
	void Update() override;
	void Draw() override;
	void Change3D() override {};
	void Change2D() override {};
	void DrawTranslucent() override;

	const GameObject::ClassID GetClassID() const override
	{
		return GameObject::eLift;
	}

	bool CheckCollisionBump(const SphereInfo& info, BumpResult& result, float dis = 0) override;

	void SetRoute(const Math::Vector3& start, const Math::Vector3& end);

	void SetSpeed(float speed) { m_speed = speed; }
	void SetSpeedByTime(float time);

private:

	void Release();

	Math::Vector3 m_startPos;   // 始点
	Math::Vector3 m_endPos;     // 終点

	float m_speed = 0.0f;       // 速度
	float m_progress = 0.0f;    // 始点と終点の進行度
	bool m_isReverse = false;   // 帰路かどうか
};


