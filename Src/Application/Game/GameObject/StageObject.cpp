#include "StageObject.h"

void StageObject::ImGuiProcess()
{
	std::string text = "Pos" + std::to_string(m_ObjectNumber);
	ImGui::DragFloat3(text.c_str(), &m_worldPos.x, 0.1f);
}

void StageObject::Init()
{
}

void StageObject::Update()
{
	UpdateWorldMatrix();
}

void StageObject::Draw()
{
	if (!m_hit)
	{
		SHADER->m_standardShader.DrawModel(m_modelWork, m_mWorld);
	}
}

void StageObject::DrawTranslucent()
{
	if (m_hit)
	{
		SHADER->m_translucentShader.DrawModel(m_modelWork, m_mWorld);
	}
}

void StageObject::Change3D()
{
	if (!m_change)
	{
		m_worldScale = Math::Vector3(1.0f, 1.0f, 1.0f);
	}

	SetPos(Math::Vector3(m_keepPos.x, m_mWorld._42, m_mWorld._43));

	m_change = true;
}

void StageObject::Change2D()
{
	m_keepPos = m_mWorld.Translation();

	m_worldScale = Math::Vector3(100.0f, 1.0f, 1.0f);

	SetPos(Math::Vector3(m_keepPos.x, m_mWorld._42, m_mWorld._43));

	m_change = false;
}

void StageObject::SetModel(const std::shared_ptr<KdModelData> spModel)
{
	m_modelWork.SetModel(spModel);
}

bool StageObject::CheckCollisionBump(const SphereInfo& info, BumpResult& result, float dis)
{
	// StageObject�̃��b�V���B�Ɠ����蔻��
	for (UINT i = 0; i < m_modelWork.GetDataNodes().size(); ++i)
	{
		// �Y���̃m�[�h���擾
		const KdModelData::Node dataNode = m_modelWork.GetDataNodes()[i];

		// ���b�V���ł͂Ȃ������画�肵�Ȃ�
		if (!dataNode.m_spMesh) { continue; }

		// �m�[�h�̌��݂̈ʒu���]���
		const KdModelWork::Node workNode = m_modelWork.GetNodes()[i];

		// �ŏI�I�ɂ��ׂĂ̓��������ʂ��獷���߂����x�N�g��
		Math::Vector3 localPushedPos = {};

		if (KdSphereToMesh(info.m_pos, info.m_radius,
			*(dataNode.m_spMesh.get()), workNode.m_worldTransform * m_mWorld, localPushedPos))
		{
			result.m_isHit = true;

			result.m_pushVec = localPushedPos - info.m_pos;
		}
	}

	return result.m_isHit;
}

void StageObject::UpdateWorldMatrix()
{
	// ���[���h�s��̍쐬
	DirectX::SimpleMath::Matrix trans;
	trans = trans.CreateTranslation(m_worldPos);

	DirectX::SimpleMath::Matrix scale;
	scale = scale.CreateScale(m_worldScale);

	m_mWorld = scale * trans;
}

void StageObject::Release()
{
}

