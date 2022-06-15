#include "FirstEnemy.h"

const float FirstEnemy::s_limitOfStepHeight = 0.5f;

void FirstEnemy::Init()
{
	m_modelWork.SetModel(GameSystem::GetInstance().WorkResourceFactory().GetModelData("Data/Models/Enemy/Enemy2.gltf"));

	m_bumpSphereInfo.m_radius = 0.3f;
	m_bumpSphereInfo.m_pos.y = 0.7f;
}

void FirstEnemy::Update()
{
	if (GameSystem::GetInstance().GetSceneName() != "Game") { return; }

	// �d�͂����Z
	m_verticalMovement += GameSystem::s_worldGravity;

	m_prevPos = GetPos();

	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
	{
		if (spObject->GetClassID() == GameObject::ePlayer)
		{
			SetTarget(spObject);
		}
	}

	m_stayTime--;
	if (m_stayTime <= 0)
	{
		m_stayTime = 0;
		m_canChase = true;
	}

	// �v���C���[��ǂ������鏈��
	UpdateMove();
	UpdateRotate();

	m_worldPos.y -= m_verticalMovement;

	UpdateCollision();

	// ���[���h�s��̍쐬
	DirectX::SimpleMath::Matrix trans;
	trans = trans.CreateTranslation(m_worldPos);

	DirectX::SimpleMath::Matrix rotation;
	rotation = rotation.CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(m_worldRot.y),
		DirectX::XMConvertToRadians(m_worldRot.x),
		DirectX::XMConvertToRadians(m_worldRot.z));

	m_mWorld = rotation * trans;
}

void FirstEnemy::Change3D()
{
	SetPos(Math::Vector3(m_keepPos.x, m_mWorld._42, m_mWorld._43));
	m_startPos.x = m_keepStartPos.x;

	m_change = true;
}

void FirstEnemy::Change2D()
{
	m_keepPos = m_mWorld.Translation();

	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
	{
		if (spObject->GetClassID() != GameObject::ePlayer) {
			continue;
		}

		SetPos(Math::Vector3(spObject->GetPos().x, m_mWorld._42, m_mWorld._43));
		m_startPos.x = spObject->GetPos().x;
	}

	m_change = false;
}

void FirstEnemy::UpdateMove()
{
	if (!m_canChase) { return; }

	if (m_wpTarget.expired()) { return; }

	std::shared_ptr<const GameObject> spTarget = m_wpTarget.lock();

	Math::Vector3 targetPos = spTarget->GetPos();
	Math::Vector3 targetDir;

	if (targetPos.z > m_actionRangeMin && targetPos.z < m_actionRangeMax)
	{
		m_stayTime = 120;
		m_isChase = true;
		// �^�[�Q�b�g�̕���
		targetDir = targetPos - m_worldPos;
	}
	else
	{
		m_stayTime--;
		if (m_stayTime <= 0)
		{
			m_stayTime = 0;
			targetDir = m_startPos - m_worldPos;
			m_isChase = false;
		}
	}

	float targetDistSqr = targetDir.LengthSquared();

	if (m_isChase)
	{
		m_stopDist = 0.6f;
	}
	if (!m_stopDist)
	{
		m_stopDist = 0.01f;
	}

	//if (m_isChase)
	{
		if (targetDistSqr > m_stopDist * m_stopDist)
		{
			Math::Vector3 vMove = m_mWorld.Backward();
			vMove.Normalize();

			vMove *= 0.03f;

			m_worldPos += vMove;
		}
	}
	/*if (!m_isChase)
	{
		Math::Vector3 vMove = m_mWorld.Backward();
		vMove.Normalize();

		vMove *= 0.03f;
		if (m_worldPos == m_startPos)
		{
			vMove *= 0.0f;
		}

		m_worldPos += vMove;
	}*/
}

void FirstEnemy::UpdateRotate()
{
	if (!m_canChase) { return; }

	// �^�[�Q�b�g�����Ȃ�������Ԃ�
	if (m_wpTarget.expired()) { return; }

	std::shared_ptr<const GameObject> spTarget = m_wpTarget.lock();

	Math::Vector3 targetPos = spTarget->GetPos();
	// �������Ă������
	Math::Vector3 nowDir = m_mWorld.Backward();

	// �^�[�Q�b�g�̕���
	Math::Vector3 targetDir = targetPos - m_worldPos;

	if (targetPos.z > m_actionRangeMin && targetPos.z < m_actionRangeMax)
	{
		targetDir = targetPos - m_worldPos;
	}
	else
	{
		if (m_stayTime <= 0)
		{
			targetDir = m_startPos - m_worldPos;
		}
	}

	// ���K��
	nowDir.Normalize();
	targetDir.Normalize();

	// �^�[�Q�b�g���X�ɉ�]�����鏈��

	// ���݂̐�Ίp�x
	float nowAng = atan2(nowDir.x, nowDir.z);
	nowAng = DirectX::XMConvertToDegrees(nowAng);

	// �ڕW�Ƃ����Ίp�x
	float targetAng = atan2(targetDir.x, targetDir.z);
	targetAng = DirectX::XMConvertToDegrees(targetAng);

	// ���݂ƖڕW�̊Ԃ̊p�x�����߂�
	float betweenAng = targetAng - nowAng;

	// 180�x�̈ʒu�Ŋp�x�̐��l�̐؂��
	if (betweenAng > 180)
	{
		betweenAng -= 360;
	}
	else if (betweenAng < -180)
	{
		betweenAng += 360;
	}

	// 1�t���[���ōő剽�x�X���̂��̐���
	float rotateAng = std::clamp(betweenAng, -90.0f, 90.0f);

	// Y���̊p�x��ύX����
	m_worldRot.y += rotateAng;
}

void FirstEnemy::UpdateCollision()
{
	// �X�e�[�W�֘A�I�u�W�F�N�g�Ƃ̓����蔻��
	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
	{
		if (spObject->GetClassID() != GameObject::eStage &&
			spObject->GetClassID() != GameObject::eStageObject &&
			spObject->GetClassID() != GameObject::eStageObjectFix &&
			spObject->GetClassID() != GameObject::eLift) {
			continue;
		}

		Math::Vector3 rayPos = m_prevPos;
		rayPos.y += s_limitOfStepHeight;

		RayInfo rayInfo(rayPos, Math::Vector3(0.0f, -1.0f, 0.0f), m_verticalMovement + s_limitOfStepHeight);
		SphereInfo sphereInfo(GetPos() + m_bumpSphereInfo.m_pos, m_bumpSphereInfo.m_radius);
		BumpResult bumpResult;

		// �����Ɠ�����ΏۂɌĂяo���Ă��炤
		bool result = spObject->CheckCollisionBump(sphereInfo, bumpResult);
		if (result)
		{
			// �����߂�����
			m_worldPos += bumpResult.m_pushVec;
		}
	}

	// �v���C���[�Ƃ̓����蔻��
	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
	{
		if (spObject->GetClassID() != GameObject::ePlayer)
		{
			continue;
		}

		SphereInfo sphereInfo(GetPos() + m_bumpSphereInfo.m_pos, m_bumpSphereInfo.m_radius);
		BumpResult bumpResult;

		// �����Ɠ�����ΏۂɌĂяo���Ă��炤
		bool result = spObject->CheckCollisionBump(sphereInfo, bumpResult);
		if (result)
		{
			m_canChase = false;
			m_stayTime = 120;
		}
	}

	// �n�ʂƂ̐ڒn����
	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
	{
		if (spObject->GetClassID() != GameObject::eStage &&
			spObject->GetClassID() != GameObject::eStageObject &&
			spObject->GetClassID() != GameObject::eStageObjectFix &&
			spObject->GetClassID() != GameObject::eLift) {
			continue;
		}

		Math::Vector3 rayPos = m_prevPos;
		rayPos.y += s_limitOfStepHeight;

		RayInfo rayInfo(rayPos, Math::Vector3(0.0f, -1.0f, 0.0f), m_verticalMovement + s_limitOfStepHeight);

		BumpResult bumpResult;

		// ����ɓ����蔻����Ă΂���
		spObject->CheckCollisionBump(rayInfo, bumpResult);

		if (bumpResult.m_isHit)
		{
			// �����߂�����
			m_worldPos += bumpResult.m_pushVec;

			m_verticalMovement = 0.0f;
		}
	}
}

void FirstEnemy::Release()
{
	m_wpTarget.reset();
}