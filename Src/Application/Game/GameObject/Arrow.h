#pragma once
#include "GameObject.h"

class Arrow : public GameObject
{
public:
	Arrow() {}
	~Arrow() { Release(); }

	void Init() override;
	void Update() override;
	void DrawEffect() override;
	void Change3D() override;
	void Change2D() override;

	bool GetHit() override { return m_isHit; }

	// 向きと場所の設定
	void SetWorldMatrix(const Math::Matrix& world) { m_mWorld = world; }

	// X座標のみ設定
	void SetPosX(const float& x) { SetPos(Math::Vector3(x, m_mWorld._42, m_mWorld._43)); }

	void SetChange(bool flg = false) { m_change = flg; }

	const GameObject::ClassID GetClassID() const override
	{
		return GameObject::eArrow;
	}

private:
	void Release();

	// 当たり判定統合
	void UpdateCollision();

	// weak_ptr(弱参照):メモリの解放に関与しないポインタ
	std::weak_ptr<const GameObject> m_wpTarget; // null代入は出来ない

	int     m_lifeSpan = 60;

	bool m_isHit = false;

	Math::Vector3 m_prevPos = {}; // 動く前の位置
};
