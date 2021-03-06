#pragma once

#include "GameObject.h"

class TPSCamera;
class Effect2D;

class Player : public GameObject
{
public:

	Player();
	~Player();

	void Init() override;
	void Update() override;
	void Draw() override;
	void DrawTranslucent() override;
	void Draw2D() override;
	void Change3D() override;
	void Change2D() override;

	// 場所の取得
	const Math::Vector3 GetPos() const override { return m_worldPos; }

	// 「プレイヤー」としてクラスIDを返す
	const GameObject::ClassID GetClassID() const override
	{
		return GameObject::ePlayer;
	}

	const std::shared_ptr<TPSCamera> GetCamera() const { return m_spCamera; }
	std::shared_ptr<TPSCamera> WorkCamera() { return m_spCamera; }

	void SetPos(const Math::Vector3& pos) override
	{
		m_worldPos = pos;
		m_prevPos = pos;
	}

private:
	std::shared_ptr<TPSCamera> m_spCamera = nullptr;
	std::weak_ptr<GameObject>  m_wpRideObj;
	Math::Matrix               m_mLocalFromRide;      // 乗物からの相対行列

	// 歩いて超えられる限界の高さ
	static const float s_limitOfStepHeight;

	void Release();

	void UpdateMove(Math::Vector3& dstMove);
	void UpdateRotate(const Math::Vector3& srcMove);

	void UpdatePosFromRideObj();
	void UpdateLocalFromRide();

	// 当たり判定統合
	void UpdateCollision();

	void UpdateWorldMatrix();

	void Update2D();
	void Update3D();

	bool m_isAlive = true;
	bool m_canJump = true;
	bool m_enemyHit = false;
	bool m_objectHit = false;

	Math::Vector3 m_worldPos;
	Math::Vector3 m_worldRot;
	Math::Vector3 m_prevPos;

	float m_verticalMovement = 0.0f;

	const float MAX_TIME = 1200;  // 3Dモード時の時間制限最大値

	const float MIN_TIME = 0;     // 3Dモード時の時間制限最小値

	float m_changeTime = MAX_TIME; // 3Dモード時の制限時間

	int m_life = 3; // プレイヤーのHP
	int m_invincibleTime = 120; // ダメージを喰らった時の無敵時間

	std::shared_ptr<KdTexture> m_spLifeTex[3];     // HP用テクスチャ
	std::shared_ptr<KdTexture> m_spLimitTex = nullptr;    // 3Dゲージ用テクスチャ

	bool m_isLanding = false; // 着地している状態

	float m_cameraRotY = -90.0f;
	bool m_start = true;
};
