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

	// �n�_�A�I�_�̐ݒ�
	void SetRoute(const Math::Vector3& start, const Math::Vector3& end);

	void SetSpeed(float speed) { m_speed = speed; }
	void SetSpeedByTime(float time = 10.0f);

private:

	void Release();

	Math::Vector3 m_startPos;   // �n�_
	Math::Vector3 m_endPos;     // �I�_

	Math::Vector3 m_startKeepPos;   // �n�_�ۑ�
	Math::Vector3 m_endKeepPos;     // �I�_�ۑ�

	float m_speed = 0.0f;       // ���x
	float m_progress = 0.0f;    // �n�_�ƏI�_�̐i�s�x
	bool m_isReverse = false;   // �A�H���ǂ���
};

