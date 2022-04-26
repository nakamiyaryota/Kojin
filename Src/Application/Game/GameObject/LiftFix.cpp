#include "LiftFix.h"

#include "../../main.h"

void LiftFix::Init()
{
	SetSpeedByTime(10.0f);

	m_modelWork.SetModel(GameSystem::GetInstance().WorkResourceFactory().GetModelData("Data/Models/Lift/Lift.gltf"));
}

void LiftFix::Update()
{
	Math::Vector3 moveLine = m_endPos - m_startPos;
	Math::Vector3 nowPos = m_startPos + moveLine * (sinf(m_progress) * 0.5f + 0.5f);

	Math::Vector3 frontDir = moveLine;
	frontDir.Normalize();

	Math::Vector3 upDir = m_mWorld.Up();
	upDir.Normalize();

	Math::Vector3 waveVec = frontDir.Cross(upDir);
	nowPos += waveVec * (sinf(EaseInOut(3.0f, 3.0f)));

	SetPos(nowPos);

	// �i���̐���
	m_progress += m_speed;
}

void LiftFix::Draw()
{
	if (!m_hit)
	{
		SHADER->m_standardShader.DrawModel(m_modelWork, m_mWorld);
	}
}

void LiftFix::Change3D()
{
	//Math::Matrix scale = Math::Matrix::CreateScale(0.1f, 1.0f, 1.0f);
	//m_mWorld *= scale;
}

void LiftFix::Change2D()
{
	//Math::Matrix scale = Math::Matrix::CreateScale(10.0f, 1.0f, 1.0f);
	//m_mWorld *= scale;
}
void LiftFix::DrawTranslucent()
{
	if (m_hit)
	{
		SHADER->m_translucentShader.DrawModel(m_modelWork, m_mWorld);
	}
}

bool LiftFix::CheckCollisionBump(const SphereInfo& info, BumpResult& result, float dis)
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

void LiftFix::SetSpeedByTime(float time)
{
	m_speed = float((2.0f * M_PI) / (time * APP.m_maxFps));
}

void LiftFix::SetRoute(const Math::Vector3& start, const Math::Vector3& end)
{
	m_startPos = start;
	m_endPos = end;
}

void LiftFix::Release()
{
}
