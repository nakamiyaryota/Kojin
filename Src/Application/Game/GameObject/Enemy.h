#pragma once
#include "GameObject.h"

class Enemy : public GameObject
{
public:
	Enemy() {}
	~Enemy() { Release(); }

	void Init() override;
	void Update() override;
	void Change3D() override;
	void Change2D() override;

	// 場所の設定
	void SetWorldMatrix(float x, float z, float y = 0.0f) {
		m_mWorld = Math::Matrix::CreateTranslation(x, y, z); 
	}

	void SetPos(const Math::Vector3& pos) override
	{
		m_worldPos = pos;
	}

	const bool GetHit() const override { return m_isHit; }

	const Math::Vector3 GetPos() const override { return m_worldPos; }

	const GameObject::ClassID GetClassID() const override
	{
		return GameObject::eEnemy;
	}

private:
	void Release();

	void UpdateRotate(const Math::Vector3& srcMove);
	void UpdateWorldMatrix();

	// 当たり判定統合
	//void UpdateCollision();

	void ShotArrow();

	bool m_isHit = false;
	bool m_canShot = true;

	bool m_zMove = false;
	bool m_xMove = false;
	bool m_moveVec = false;

	const int TIME_MAX = 180;
	int moveReturn = TIME_MAX;

	int     m_lifeSpan = 60;

	Math::Vector3 m_worldPos;
	Math::Vector3 m_worldRot;

};