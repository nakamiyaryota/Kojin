#include "GameObject.h"

void GameObject::DrawShadowMap()
{
	SHADER->m_genShadowMapShader.DrawModel(m_modelWork, m_mWorld);
}

void GameObject::Draw()
{
	SHADER->m_standardShader.DrawModel(m_modelWork, m_mWorld);
}

void GameObject::Change3D()
{
	if (!m_change)
	{
		Math::Matrix scale = Math::Matrix::CreateScale(0.01f, 1.0f, 1.0f);
		m_mWorld *= scale;
	}

	SetPos(Math::Vector3(m_keepPos.x, m_mWorld._42, m_mWorld._43));
	
	m_change = true;
}

void GameObject::Change2D()
{
	m_keepPos = m_mWorld.Translation();

	Math::Matrix scale = Math::Matrix::CreateScale(100.0f, 1.0f, 1.0f);
	m_mWorld *= scale;

	
	SetPos(Math::Vector3(m_keepPos.x, m_mWorld._42, m_mWorld._43));

	m_change = false;
}

bool GameObject::CheckCollisionBump(const SphereInfo& info, BumpResult& result, float dis)
{
	Math::Vector3 selfPos = GetPos() + m_bumpSphereInfo.m_pos;

	Math::Vector3 betweenVec = info.m_pos - selfPos;

	float distanceSqr = betweenVec.LengthSquared();

	float hitRadius = m_bumpSphereInfo.m_radius + info.m_radius;

	result.m_isHit = (distanceSqr <= (hitRadius * hitRadius));

	if (result.m_isHit)
	{
		// 押し戻すのに都合のいいベクトル
		result.m_pushVec = betweenVec;
		result.m_pushVec.Normalize();

		// どの程度押し戻すか
		float distance = std::sqrtf(distanceSqr) - dis;
		result.m_pushVec *= hitRadius - distance;
	}

	return result.m_isHit;
}

bool GameObject::CheckCollisionBump(const RayInfo& info, BumpResult& result)
{
	float minDist = FLT_MAX;

	// 全てのメッシュと当たり判定をする
	KdRayResult rayResult;

	for (UINT i = 0; i < m_modelWork.GetDataNodes().size(); ++i)
	{
		const KdModelData::Node& dataNode = m_modelWork.GetDataNodes()[i];

		if (!dataNode.m_spMesh) { continue; }

		const KdModelWork::Node& workNode = m_modelWork.GetNodes()[i];

		KdRayToMesh(
			info.m_pos,      // 発射位置
			info.m_dir,      // 発射方向
			info.m_range,    // 判定する長さ
			*(dataNode.m_spMesh.get()), // 相手のメッシュ情報
			workNode.m_worldTransform * m_mWorld,        // 相手の行列
			&rayResult);     // 出力結果

		if (rayResult.m_hit && rayResult.m_distance < minDist)
		{
			result.m_isHit = rayResult.m_hit;

			// 押し戻す分のベクトルを計算する
			result.m_pushVec = info.m_dir * (rayResult.m_distance - info.m_range);

			minDist = rayResult.m_distance;

			// HITした位置を記憶させる
			result.m_HitPos = rayResult.m_HitPos;
			result.m_Distance = rayResult.m_distance;
		}
	}

	return rayResult.m_hit;
}
