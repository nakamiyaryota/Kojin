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

	// 移動する前の位置を覚える
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

		// レイを使って当たり判定
		// 発射位置(動く前の位置)
		Math::Vector3 rayPos = m_prevPos;
		// 発射方向(前の地点から今の地点に向かって)
		Math::Vector3 rayDir = m_mWorld.Translation() - m_prevPos;
		// レイの長さ(1回の移動量)
		float length = rayDir.Length();

		// レイ情報を作る
		RayInfo rayInfo(rayPos, rayDir, length);
		BumpResult result;

		// 自分と当たる対象に呼び出してもらう
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

		// レイを使って当たり判定
		// 発射位置(動く前の位置)
		Math::Vector3 rayPos = m_prevPos;
		// 発射方向(前の地点から今の地点に向かって)
		Math::Vector3 rayDir = m_mWorld.Translation() - m_prevPos;
		// レイの長さ(1回の移動量)
		float length = rayDir.Length();

		// レイ情報を作る
		RayInfo rayInfo(rayPos, rayDir, length);
		BumpResult result;

		// 自分と当たる対象に呼び出してもらう
		if (spObject->CheckCollisionBump(rayInfo, result))
		{
			if (GameSystem::GetInstance().GetSceneName() != "Game") { return; }

			Math::Vector3 pos = GetPos();
			pos.z += 0.2;

			m_isHit = true;
		}
	}
}
