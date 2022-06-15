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

	// 重力を加算
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

	// プレイヤーを追いかける処理
	UpdateMove();
	UpdateRotate();

	m_worldPos.y -= m_verticalMovement;

	UpdateCollision();

	// ワールド行列の作成
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
		// ターゲットの方向
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

	// ターゲットがいなかったら返る
	if (m_wpTarget.expired()) { return; }

	std::shared_ptr<const GameObject> spTarget = m_wpTarget.lock();

	Math::Vector3 targetPos = spTarget->GetPos();
	// 今向いている方向
	Math::Vector3 nowDir = m_mWorld.Backward();

	// ターゲットの方向
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

	// 正規化
	nowDir.Normalize();
	targetDir.Normalize();

	// ターゲット徐々に回転させる処理

	// 現在の絶対角度
	float nowAng = atan2(nowDir.x, nowDir.z);
	nowAng = DirectX::XMConvertToDegrees(nowAng);

	// 目標とする絶対角度
	float targetAng = atan2(targetDir.x, targetDir.z);
	targetAng = DirectX::XMConvertToDegrees(targetAng);

	// 現在と目標の間の角度を求める
	float betweenAng = targetAng - nowAng;

	// 180度の位置で角度の数値の切れ目
	if (betweenAng > 180)
	{
		betweenAng -= 360;
	}
	else if (betweenAng < -180)
	{
		betweenAng += 360;
	}

	// 1フレームで最大何度傾くのかの制御
	float rotateAng = std::clamp(betweenAng, -90.0f, 90.0f);

	// Y軸の角度を変更する
	m_worldRot.y += rotateAng;
}

void FirstEnemy::UpdateCollision()
{
	// ステージ関連オブジェクトとの当たり判定
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

		// 自分と当たる対象に呼び出してもらう
		bool result = spObject->CheckCollisionBump(sphereInfo, bumpResult);
		if (result)
		{
			// 押し戻す処理
			m_worldPos += bumpResult.m_pushVec;
		}
	}

	// プレイヤーとの当たり判定
	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
	{
		if (spObject->GetClassID() != GameObject::ePlayer)
		{
			continue;
		}

		SphereInfo sphereInfo(GetPos() + m_bumpSphereInfo.m_pos, m_bumpSphereInfo.m_radius);
		BumpResult bumpResult;

		// 自分と当たる対象に呼び出してもらう
		bool result = spObject->CheckCollisionBump(sphereInfo, bumpResult);
		if (result)
		{
			m_canChase = false;
			m_stayTime = 120;
		}
	}

	// 地面との接地判定
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

		// 相手に当たり判定を呼ばせる
		spObject->CheckCollisionBump(rayInfo, bumpResult);

		if (bumpResult.m_isHit)
		{
			// 押し戻す処理
			m_worldPos += bumpResult.m_pushVec;

			m_verticalMovement = 0.0f;
		}
	}
}

void FirstEnemy::Release()
{
	m_wpTarget.reset();
}