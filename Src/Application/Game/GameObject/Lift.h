#pragma once

#include "GameObject.h"

class Lift : public GameObject
{
public:
	Lift() {}
	~Lift() {}

	void Init() override;
	void Update() override;
	void Draw() override;
	void Change2D() override;
	void Change3D() override;
	void DrawTranslucent() override;

	const GameObject::ClassID GetClassID() const override
	{
		return GameObject::eLift;
	}

	bool CheckCollisionBump(const SphereInfo& info, BumpResult& result, float dis = 0) override;

	// 始点、終点の設定
	void SetRoute(const Math::Vector3& start, const Math::Vector3& end);

	void SetSpeed(float speed) { m_speed = speed; }
	void SetSpeedByTime(float time = 10.0f);

private:

	void Release();

	Math::Vector3 m_startPos;   // 始点
	Math::Vector3 m_endPos;     // 終点

	Math::Vector3 m_startKeepPos;   // 始点保存
	Math::Vector3 m_endKeepPos;     // 終点保存

	float m_speed = 0.0f;       // 速度
	float m_progress = 0.0f;    // 始点と終点の進行度
	bool m_isReverse = false;   // 帰路かどうか
};

