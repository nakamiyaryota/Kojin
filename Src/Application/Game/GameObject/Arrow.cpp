#include "Arrow.h"

#include "Effect2D.h"

void Arrow::Init()
{
	m_modelWork.SetModel(GameSystem::GetInstance().WorkResourceFactory().GetModelData("Data/Models/Enemy/Arrow/Arrow.gltf"));

	m_lifeSpan = 180;

	m_bumpSphereInfo.m_radius = 0.25f;
}

void Arrow::Update()
{
	if (--m_lifeSpan <= 0)
	{
		m_isAlive = false;
	}

	// �ړ�����O�̈ʒu���o����
	m_prevPos = m_mWorld.Translation();


	Math::Vector3 move = m_mWorld.Backward();
	move.Normalize();

	move *= 0.05f;

	Math::Matrix trans = Math::Matrix::CreateTranslation(move);

	m_mWorld *= trans;

	UpdateCollision();
}

void Arrow::DrawEffect()
{
}

void Arrow::Change3D()
{
	SetPos(Math::Vector3(m_keepPos.x, m_mWorld._42, m_mWorld._43));

	m_change = true;
}

void Arrow::Change2D()
{
	m_keepPos = m_mWorld.Translation();

	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
	{
		if (spObject->GetClassID() != GameObject::ePlayer) {
			continue;
		}

		SetPos(Math::Vector3(spObject->GetPos().x, m_mWorld._42, m_mWorld._43));
	}

	m_change = false;
}

void Arrow::Release()
{
	
}

void Arrow::UpdateCollision()
{
	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
	{
		if (spObject->GetClassID() != GameObject::eStage &&
			spObject->GetClassID() != GameObject::eStageObject &&
			spObject->GetClassID() != GameObject::eStageObjectFix &&
			spObject->GetClassID() != GameObject::eLift) {
			continue;
		}

		// ���C���g���ē����蔻��
		// ���ˈʒu(�����O�̈ʒu)
		Math::Vector3 rayPos = m_prevPos;
		// ���˕���(�O�̒n�_���獡�̒n�_�Ɍ�������)
		Math::Vector3 rayDir = m_mWorld.Translation() - m_prevPos;
		// ���C�̒���(1��̈ړ���)
		float length = rayDir.Length();

		// ���C�������
		RayInfo rayInfo(rayPos, rayDir, length);
		BumpResult result;

		// �����Ɠ�����ΏۂɌĂяo���Ă��炤
		if (spObject->CheckCollisionBump(rayInfo, result))
		{
			m_isAlive = false;
		}
	}

	m_isHit = false;
	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
	{
		if (spObject->GetClassID() != GameObject::ePlayer) {
			continue;
		}

		// ���C���g���ē����蔻��
		// ���ˈʒu(�����O�̈ʒu)
		Math::Vector3 rayPos = m_prevPos;
		// ���˕���(�O�̒n�_���獡�̒n�_�Ɍ�������)
		Math::Vector3 rayDir = m_mWorld.Translation() - m_prevPos;
		// ���C�̒���(1��̈ړ���)
		float length = rayDir.Length();

		// ���C�������
		RayInfo rayInfo(rayPos, rayDir, length);
		BumpResult result;

		// �����Ɠ�����ΏۂɌĂяo���Ă��炤
		if (spObject->CheckCollisionBump(rayInfo, result))
		{
			if (GameSystem::GetInstance().GetSceneName() != "Game") { return; }

			Math::Vector3 pos = GetPos();
			pos.z += 0.2;

			m_isHit = true;
		}
	}
}
