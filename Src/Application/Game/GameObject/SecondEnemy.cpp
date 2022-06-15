#include "SecondEnemy.h"
#include "Arrow.h"

void SecondEnemy::Init()
{
	m_modelWork.SetModel(GameSystem::GetInstance().WorkResourceFactory().GetModelData("Data/Models/Enemy/Enemy1.gltf"));

	m_bumpSphereInfo.m_radius = 0.3f;
	m_bumpSphereInfo.m_pos.y = 0.7f;
}

void SecondEnemy::Update()
{
	Math::Vector3 vMove;

	UpdateRotate(vMove);

	UpdateWorldMatrix();

	// ��̔���
	ShotArrow();

}

void SecondEnemy::Change3D()
{
	SetPos(Math::Vector3(m_keepPos.x, m_mWorld._42, m_mWorld._43));

	m_change = true;
}

void SecondEnemy::Change2D()
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

void SecondEnemy::Release()
{
}

void SecondEnemy::UpdateRotate(const Math::Vector3& srcMove)
{
	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
	{
		if (spObject->GetClassID() != GameObject::ePlayer) { continue; }

		if (m_worldPos.z < spObject->GetPos().z)
		{
			m_worldRot.y = 0.0f;
		}
		if (m_worldPos.z > spObject->GetPos().z)
		{
			m_worldRot.y = 180.0f;
		}
	}
}

void SecondEnemy::UpdateWorldMatrix()
{
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

void SecondEnemy::ShotArrow()
{
	m_lifeSpan--;
	if (m_lifeSpan <= 0)
	{
		m_canShot = true;
	}

	// �e�I�u�W�F�N�g���쐬���A�����ł��o����悤�ɂ���
	if (m_canShot)
	{
		// ����C���X�^���X��
		std::shared_ptr<Arrow> spArrow = std::make_shared<Arrow>();
		spArrow->Init();

		// �G�̈ʒu�A�����Ă�������ɖ��ݒ�
		spArrow->SetWorldMatrix(m_mWorld);

		// ���f������m�[�h��T��
		const KdModelWork::Node* pNode = m_modelWork.FindNode("bow");

		// �T�����m�[�h������΁A���̏ꏊ���甭�˂���
		if (pNode)
		{
			spArrow->SetWorldMatrix(pNode->m_worldTransform * m_mWorld);
		}

		spArrow->SetPosX(m_keepPos.x);
		spArrow->SetChange(m_change);

		if (!m_change)
		{
			spArrow->Change2D();
		}
		// ��̃��X�g�ɒǉ�
		GameSystem::GetInstance().AddObject(spArrow);

		// ����������̂ň�U���ĂȂ�����
		m_canShot = false;
		m_lifeSpan = 180;
	}
}