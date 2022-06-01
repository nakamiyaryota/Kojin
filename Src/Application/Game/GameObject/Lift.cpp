#include "Lift.h"

#include "../../main.h"

void Lift::Init()
{
	SetSpeedByTime(10.0f);

	m_modelWork.SetModel(GameSystem::GetInstance().WorkResourceFactory().GetModelData("Data/Models/Lift/Lift.gltf"));
}

void Lift::Update()
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

	// 進歩の制御
	m_progress += m_speed;
}

void Lift::Draw()
{
	if (!m_hit)
	{
		SHADER->m_standardShader.DrawModel(m_modelWork, m_mWorld);
	}
}

void Lift::DrawTranslucent()
{
	if (m_hit)
	{
		SHADER->m_translucentShader.DrawModel(m_modelWork, m_mWorld);
	}
}

void Lift::Change3D()
{
	if (!m_change)
	{
		Math::Matrix scale = Math::Matrix::CreateScale(0.01f, 1.0f, 1.0f);
		m_mWorld *= scale;
	}

	m_startPos = Math::Vector3(m_startKeepPos.x, m_startPos.y, m_startPos.z);
	m_endPos = Math::Vector3(m_endKeepPos.x, m_endPos.y, m_endPos.z);

	m_change = true;
}

void Lift::Change2D()
{
	m_startKeepPos = m_startPos;
	m_endKeepPos = m_endPos;

	Math::Matrix scale = Math::Matrix::CreateScale(100.0f, 1.0f, 1.0f);
	m_mWorld *= scale;

	m_startPos = Math::Vector3(m_startKeepPos.x, m_startPos.y, m_startPos.z);
	m_endPos = Math::Vector3(m_endKeepPos.x, m_endPos.y, m_endPos.z);

	m_change = false;
}

bool Lift::CheckCollisionBump(const SphereInfo& info, BumpResult& result, float dis)
{
	// StageObjectのメッシュ達と当たり判定
	for (UINT i = 0; i < m_modelWork.GetDataNodes().size(); ++i)
	{
		// 該当のノードを取得
		const KdModelData::Node dataNode = m_modelWork.GetDataNodes()[i];

		// メッシュではなかったら判定しない
		if (!dataNode.m_spMesh) { continue; }

		// ノードの現在の位置や回転情報
		const KdModelWork::Node workNode = m_modelWork.GetNodes()[i];

		// 最終的にすべての当たった面から差し戻されるベクトル
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

void Lift::SetSpeedByTime(float time)
{
	m_speed = float((2.0f * M_PI) / (time * APP.m_maxFps));
}

void Lift::SetRoute(const Math::Vector3& start, const Math::Vector3& end)
{
	m_startPos = start;
	m_endPos = end;
}

void Lift::Release()
{
}

