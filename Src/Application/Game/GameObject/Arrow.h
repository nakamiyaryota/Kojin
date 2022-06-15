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

	// �����Əꏊ�̐ݒ�
	void SetWorldMatrix(const Math::Matrix& world) { m_mWorld = world; }

	// X���W�̂ݐݒ�
	void SetPosX(const float& x) { SetPos(Math::Vector3(x, m_mWorld._42, m_mWorld._43)); }

	void SetChange(bool flg = false) { m_change = flg; }

	const GameObject::ClassID GetClassID() const override
	{
		return GameObject::eArrow;
	}

private:
	void Release();

	// �����蔻�蓝��
	void UpdateCollision();

	// weak_ptr(��Q��):�������̉���Ɋ֗^���Ȃ��|�C���^
	std::weak_ptr<const GameObject> m_wpTarget; // null����͏o���Ȃ�

	int     m_lifeSpan = 60;

	bool m_isHit = false;

	Math::Vector3 m_prevPos = {}; // �����O�̈ʒu
};
