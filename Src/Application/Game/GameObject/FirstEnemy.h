#pragma once
#include "GameObject.h"

class FirstEnemy : public GameObject
{
public:
	FirstEnemy() {}
	~FirstEnemy() {}

	void Init() override;
	void Update() override;
	void Change3D() override;
	void Change2D() override;

	// 場所の設定
	void SetWorldMatrix(float x, float z, float y = 0.0f)
	{
		m_mWorld = Math::Matrix::CreateTranslation(x, y, z);
		m_startPos = Math::Vector3(x, y, z);
		m_keepStartPos = Math::Vector3(x, y, z);
	}

	// ターゲットのセット
	void SetTarget(std::shared_ptr<const GameObject> spTarget)
	{
		m_wpTarget = spTarget;
	}

	const Math::Vector3 GetPos() const override { return m_worldPos; }

	// 「エネミー」としてクラスIDを返す
	const GameObject::ClassID GetClassID() const override
	{
		return GameObject::eEnemy;
	}

	void SetPos(const Math::Vector3& pos) override
	{
		m_worldPos = pos;
		m_prevPos = pos;
	}

	void SetActionRange(float min, float max)
	{
		m_actionRangeMin = min;
		m_actionRangeMax = max;
	}

private:

	// 歩いて超えられる限界の高さ
	static const float s_limitOfStepHeight;

	void UpdateMove();
	void UpdateRotate();

	// 当たり判定統合
	void UpdateCollision();

	void Release();

	Math::Vector3 m_worldPos;
	Math::Vector3 m_worldRot;
	Math::Vector3 m_prevPos;

	float m_verticalMovement = 0.0f;
	float m_stopDist = 0.6f;

	Math::Vector3 m_startPos;
	Math::Vector3 m_keepStartPos;

	float m_actionRangeMax;
	float m_actionRangeMin;

	int m_stayTime = 0;
	bool m_isChase = false;
	bool m_canChase = false;

	// 追いかけるターゲット
	std::weak_ptr<const GameObject> m_wpTarget;
};