#include "Signboard.h"

void Signboard::Init()
{
	
}

void Signboard::Update()
{
	
}

void Signboard::Draw()
{
	if (!m_hit)
	{
		SHADER->m_standardShader.DrawModel(m_modelWork, m_mWorld);
	}
}

void Signboard::DrawTranslucent()
{
	if (m_hit)
	{
		SHADER->m_translucentShader.DrawModel(m_modelWork, m_mWorld);
	}
}

void Signboard::SetModel(const std::shared_ptr<KdModelData> spModel)
{
	m_modelWork.SetModel(spModel);
}

bool Signboard::CheckCollisionBump(const SphereInfo& info, BumpResult& result, float dis)
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

void Signboard::Release()
{
}

