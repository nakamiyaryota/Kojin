#include "Player.h"
#include "../Camera/TPSCamera.h"
#include "Effect2D.h"

const float Player::s_limitOfStepHeight = 0.5f;

Player::Player()
{
}

Player::~Player()
{
	Release();
}

void Player::Init()
{
	m_modelWork.SetModel(GameSystem::GetInstance().WorkResourceFactory().GetModelData("Data/Models/Player/Player.gltf"));

	// テクスチャの読み込み
	m_spLifeTex = GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Game/Life.png");
	m_spLifeBackTex = GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Game/LifeBack.png");
	m_spLimitTex = GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Game/Limit.png");
	
	m_spCamera = std::make_shared<TPSCamera>();

	GameSystem::GetInstance().SetCamera(m_spCamera);

	m_spCamera->Init();
	
	m_spCamera->SetProjectionMatrixOrth();
	m_spCamera->SetLocalPos(Math::Vector3(1.0f, 2.0f, -1000.0f));
	m_spCamera->SetLocalRotX(0.0f);
	m_spCamera->SetLocalRotY(-90.0f);
	m_spCamera->SetLocalGazePosition(Math::Vector3(0.0f, 0.0f, 0.0f));

	// HIT半径を設定
	m_bumpSphereInfo.m_radius = 0.3f;
	m_bumpSphereInfo.m_pos.y = 0.7f;
}

void Player::Update()
{
	if (!m_isAlive) { return; }

	if (GameSystem::GetInstance().GetSceneName() == "Clear") { return; }

	// 重力を加算
	m_verticalMovement += GameSystem::s_worldGravity;

	// 乗ってるオブジェクトを取得(実際に乗ってたら)
	UpdatePosFromRideObj();

	m_prevPos = GetPos();

	Math::Vector3 vMove;

	// 座標の更新
	UpdateMove(vMove);

	// 回転角度の更新
	UpdateRotate(vMove);

	// ジャンプ
	if (GameSystem::GetInstance().GetInputController().GetKeyState("Jump") & eKeyPress)
	{ 
		if (m_canJump)
		{
			std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
			spSoundInstance = GameSystem::GetInstance().WorkAudioManager().Play("Data/Sounds/se_jump.wav");
			spSoundInstance->SetVolume(0.14f);

			m_verticalMovement = -0.15f;
			m_canJump = false;
		}
	}

	if (m_change)
	{
		Update3D();
	}
	else
	{
		Update2D();
	}
	
	m_worldPos.y -= m_verticalMovement;

	// 当たり判定
	UpdateCollision();

	// 座標が正しい位置に確定した後に行う
	UpdateWorldMatrix();

	// 落下判定
	if (m_worldPos.y < -10)
	{
		m_life = 0;
	}

	// 死亡判定
	if (m_life < 1)
	{
		m_isAlive = false;
	}

	// 死亡した時の処理
	if (!m_isAlive)
	{
		if (GameSystem::GetInstance().GetSceneName() == "Title")
		{
			SetPos(Math::Vector3::Zero);
			m_life = 3;
			m_isAlive = true;
		}
		// ゲームオーバー画面へ移行
		if (GameSystem::GetInstance().GetSceneName() == "Game")
		{
			Player::Change2D();
			GameSystem::GetInstance().RequestChangeScene("Result");
		}
	}

	// カメラ切り替え、追従
	if (m_spCamera)
	{

		m_spCamera->Update();

		bool a = false;

		if (m_change)
		{
			cameraRotY += 3.0f;
			if (cameraRotY >= 0.0f)
			{
				cameraRotY = 0.0f;
			}

			m_spCamera->SetLocalRotY(cameraRotY);
		}
		if (!m_change)
		{
			cameraRotY -= 90.0f;
			if (cameraRotY <= -90.0f)
			{
				cameraRotY = -90.0f;
				a = true;
			}
			
			GameSystem::GetInstance().RequestStartFlg(a);
			m_spCamera->SetLocalRotY(cameraRotY);
		}

		Math::Matrix trans;
		
		// 追従の範囲
		if (!m_change)
		{
			if (m_worldPos.y >= 1.5f)
			{
				trans = Math::Matrix::CreateTranslation(m_worldPos.x * 0.5f, m_worldPos.y - 1.5f, m_worldPos.z);
			}
			else
			{
				trans = Math::Matrix::CreateTranslation(m_worldPos.x * 0.5f, 0.0f, m_worldPos.z);
			}
		}
		if (m_change)
		{
			if (m_worldPos.y >= -0.1f)
			{
				trans = Math::Matrix::CreateTranslation(m_worldPos.x * 0.5f, m_worldPos.y, m_worldPos.z);
			}
			else
			{
				trans = Math::Matrix::CreateTranslation(m_worldPos.x * 0.5f, 0.0f, m_worldPos.z);
			}
		}
		
		// プレイヤー追従カメラの位置
		// キャラクターから見たカメラの位置にプレイヤーの位置を合成したもの
		m_spCamera->SetCameraMatrix(trans);
	}

	// プレイヤーから乗ってるオブジェクトへの相対的な行列を作成する
	UpdateLocalFromRide();

	
	// 無敵時間
	if (m_enemyHit)
	{
		m_invincibleTime--;
		if (m_invincibleTime <= 0)
		{
			m_invincibleTime = 120;
			m_enemyHit = false;
		}
	}
}

void Player::Draw()
{
	if (!m_isAlive) { return; }

	if (!m_enemyHit)
	{
		SHADER->m_standardShader.DrawModel(m_modelWork, m_mWorld);
	}
}

void Player::DrawTranslucent()
{
	if (!m_isAlive) { return; }

	if (m_enemyHit)
	{
		SHADER->m_translucentShader.DrawModel(m_modelWork, m_mWorld);
	}
}

void Player::Draw2D()
{
	if (!m_spLimitTex) { return; }

	if (GameSystem::GetInstance().GetSceneName() != "Game") { return; }

	// ビューポートを利用して解像度を得る
	Math::Viewport vp;
	D3D.GetViewport(vp);

	int posX = static_cast<int>(-(vp.width * 0.5f) + (m_spLifeBackTex.get()->GetWidth() * 0.5f));
	int posY = static_cast<int>((vp.height * 0.5f) - (m_spLifeBackTex.get()->GetHeight() * 0.5f));

	SHADER->m_spriteShader.SetMatrix(Math::Matrix::Identity);
	SHADER->m_spriteShader.DrawTex(m_spLifeBackTex.get(), posX + 10, posY - 10);
	SHADER->m_spriteShader.DrawTex(m_spLifeBackTex.get(), posX + 120, posY - 10);
	SHADER->m_spriteShader.DrawTex(m_spLifeBackTex.get(), posX + 230, posY - 10);

	if (m_life > 0)
	{
		SHADER->m_spriteShader.DrawTex(m_spLifeTex.get(), posX + 10, posY - 10);
	}
	if (m_life > 1)
	{
		SHADER->m_spriteShader.DrawTex(m_spLifeTex.get(), posX + 120, posY - 10);
	}
	if (m_life > 2)
	{
		SHADER->m_spriteShader.DrawTex(m_spLifeTex.get(), posX + 230, posY - 10);
	}

	posX = static_cast<int>(-(vp.width * 0.5f) + (m_spLimitTex.get()->GetWidth() * 0.5f));
	posY = static_cast<int>((vp.height * 0.5f) - (m_spLimitTex.get()->GetHeight() * 0.5f));

	SHADER->m_spriteShader.DrawTex(m_spLimitTex.get(), posX + 10, posY - 120);

	Math::Color color = { (1.0f - (m_changeTime * 0.001f)), 0.8f, (1.0f - (m_changeTime * 0.001f)), 1.0f };

	for (int i = 1; i < 45; i++)
	{
		SHADER->m_spriteShader.DrawLine(-622, 232 - i, int(-622 + (m_changeTime * 0.236)), 232 - i, &color);
	}

}

void Player::Change3D()
{
	m_worldPos.x = m_Cpos.x;
	
	if (!m_change)
	{
		m_spCamera->SetProjectionMatrix(60.0f);
		m_spCamera->SetLocalPos(Math::Vector3(0.0f, 0.0f, -5.0f));
		m_spCamera->SetLocalRotX(15.0f);
		m_spCamera->SetLocalRotY(0.0f);
		m_spCamera->SetLocalGazePosition(Math::Vector3(0.0f, 1.0f, 0.0f));
	}

	m_change = true;
}

void Player::Change2D()
{
	m_Cpos = m_mWorld.Translation();

	if (m_change)
	{
		m_spCamera->SetProjectionMatrixOrth();
		m_spCamera->SetLocalPos(Math::Vector3(1.0f, 2.0f, -1000.0f));
		m_spCamera->SetLocalRotX(0.0f);
		m_spCamera->SetLocalRotY(-90.0f);
		m_spCamera->SetLocalGazePosition(Math::Vector3(0.0f, 0.0f, 0.0f));
	}

	m_change = false;
}

void Player::Release()
{
	m_spCamera.reset(); // 参照カウンタ減らす
}

void Player::UpdateMove(Math::Vector3& dstMove)
{
	float moveSpd = 0.06f;

	DirectX::SimpleMath::Vector3 moveVec;

	const Math::Vector2& inputAxis = GameSystem::GetInstance().GetInputController().GetAxis("Move");

	
	
	if (m_change)
	{
		moveVec.x = inputAxis.x;
		moveVec.z = inputAxis.y;
	}
	if (!m_change)
	{
		moveVec.x = inputAxis.x;
	}

	// 正規化:方向を保った状態で長さを１にする
	moveVec.Normalize();

	moveVec *= moveSpd;

	// カメラの回転を加味してベクトルを変換する
	if (m_spCamera)
	{
		moveVec = moveVec.TransformNormal(moveVec, m_spCamera->GetRotationYMatrix());
	}

	if (!m_change)
	{
		moveVec.x = 0.0f;
	}

	m_worldPos += moveVec;

	dstMove = moveVec;

	
}

void Player::UpdateRotate(const Math::Vector3& srcMove)
{
	if (srcMove.LengthSquared() == 0.0f) { return; }

	// 今向いている方向(出発地点)
	Math::Vector3 nowDir = m_mWorld.Backward();

	// 向きたい目的の方向：キャラの移動方向(目標地点)
	Math::Vector3 targetDir = srcMove;

	// 正規化
	nowDir.Normalize();
	targetDir.Normalize();

	// 現在の絶対角度
	float nowAng = atan2(nowDir.x, nowDir.z);
	nowAng = DirectX::XMConvertToDegrees(nowAng);

	// 目標とする絶対角度
	float targetAng = atan2(targetDir.x, targetDir.z);
	targetAng = DirectX::XMConvertToDegrees(targetAng);

	// 現在と目標の間の角度を求める
	float betweenAng = targetAng - nowAng;

	if (betweenAng > 180)
	{
		betweenAng += -360;
	}
	else if (betweenAng < -180)
	{
		betweenAng += 360;
	}

	float rotateAng = std::clamp(betweenAng, -20.0f, 20.0f);

	// Y軸の角度を変更する
	m_worldRot.y += rotateAng;
}

void Player::UpdatePosFromRideObj()
{
	for (std::shared_ptr<GameObject> spObject : GameSystem::GetInstance().GetObjects())
	{
		spObject = m_wpRideObj.lock();

		if (!spObject) { return; }

		if (spObject->GetClassID() != GameObject::eLift) {
			continue;
		}

		m_worldPos.y = (m_mLocalFromRide * spObject->GetMatrix()).Translation().y;
		m_worldPos.z = (m_mLocalFromRide * spObject->GetMatrix()).Translation().z;
	}
}

void Player::UpdateLocalFromRide()
{
	std::shared_ptr<GameObject> m_rideObj = m_wpRideObj.lock();

	if (!m_rideObj) { return; }

	Math::Matrix liftInv = m_rideObj->GetMatrix().Invert();
	m_mLocalFromRide = m_mWorld * liftInv;
}

void Player::UpdateCollision()
{
	// ステージ関連オブジェクトとの当たり判定
	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
	{
		if (spObject->GetClassID() != GameObject::eStage&&
			spObject->GetClassID() != GameObject::eStageObject&&
			spObject->GetClassID() != GameObject::eStageObjectFix&&
			spObject->GetClassID() != GameObject::eLift){
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

	// 敵との当たり判定
	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
	{
		if (spObject->GetClassID() != GameObject::eEnemy &&
			spObject->GetClassID() != GameObject::eArrow)
		{
			continue;
		}

		SphereInfo sphereInfo(GetPos() + m_bumpSphereInfo.m_pos, m_bumpSphereInfo.m_radius);
		BumpResult bumpResult;

		// 自分と当たる対象に呼び出してもらう
		bool result = spObject->CheckCollisionBump(sphereInfo, bumpResult, 0.5);
		if (result)
		{
			// 押し戻す処理
			m_worldPos += bumpResult.m_pushVec;

			if (!m_enemyHit)
			{
				std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
				spSoundInstance = GameSystem::GetInstance().WorkAudioManager().Play("Data/Sounds/dmg01.wav");
				spSoundInstance->SetVolume(0.2f);

				m_life -= 1;

				m_enemyHit = true;
			}
		}

		// 矢の当たり判定
		if (spObject->GetClassID() != GameObject::eArrow) { continue; }
		if (spObject->GetHit()) 
		{
			if (!m_enemyHit)
			{
				std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
				spSoundInstance = GameSystem::GetInstance().WorkAudioManager().Play("Data/Sounds/dmg01.wav");
				spSoundInstance->SetVolume(0.2f);

				m_life -= 1;

				m_enemyHit = true;
			}
			spObject->Destroy();
		}
	}

	// ゴールとの当たり判定
	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
	{
		if (spObject->GetClassID() != GameObject::eGoal) {
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
			if (GameSystem::GetInstance().GetSceneName() != "Game") { return; }
			Player::Change2D();
			m_worldRot.y = 90.0f;
			GameSystem::GetInstance().RequestChangeScene("Clear");
		}
	}

	// 看板との当たり判定
	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
	{
		if (spObject->GetClassID() != GameObject::eSignboard) {
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
			GameSystem::GetInstance().SetStageNumber(spObject->GetNumber());
			GameSystem::GetInstance().RequestChangeScene("Game");
		}
	}

	m_isLanding = false;

	m_wpRideObj.reset();

	// 地面との接地判定
	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
	{
		if (spObject->GetClassID() != GameObject::eStage &&
			spObject->GetClassID() != GameObject::eStageObject&&
			spObject->GetClassID() != GameObject::eStageObjectFix&&
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
			m_canJump = true;

			m_isLanding = true;
			m_wpRideObj = spObject;
		}
	}


	// オブジェクト透過判定
	m_hit = false;

	if (m_change)
	{
		for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
		{
			if (spObject->GetClassID() != GameObject::eStage &&
				spObject->GetClassID() != GameObject::eStageObject&&
				spObject->GetClassID() != GameObject::eStageObjectFix&&
				spObject->GetClassID() != GameObject::eLift) { continue; }

			Math::Vector3 rayPos = GetPos();
			rayPos.y = GetPos().y + 1.2f;

			RayInfo rayInfo(rayPos, Math::Vector3(0.0f, 0.0f, -1.0f), 6.0f);

			BumpResult bumpResult;

			// 相手に当たり判定を呼ばせる
			spObject->CheckCollisionBump(rayInfo, bumpResult);

			if (bumpResult.m_isHit)
			{
				m_hit = true;
			}
			spObject->SetHit(bumpResult.m_isHit);
		}
	}
	if (!m_change)
	{
		for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
		{
			if (spObject->GetClassID() != GameObject::eStage &&
				spObject->GetClassID() != GameObject::eStageObject &&
				spObject->GetClassID() != GameObject::eStageObjectFix &&
				spObject->GetClassID() != GameObject::eLift) { continue; }
			spObject->SetHit(false);
		}
	}

	// 隣にオブジェクトがあれば2Dにできない
	m_objectHit = false;

	if (m_change)
	{
		for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
		{
			if (spObject->GetClassID() != GameObject::eStage &&
				spObject->GetClassID() != GameObject::eStageObject &&
				spObject->GetClassID() != GameObject::eStageObjectFix &&
				spObject->GetClassID() != GameObject::eLift &&
				spObject->GetClassID() != GameObject::eEnemy) {
				continue;
			}

			Math::Vector3 rayPos = GetPos();
			rayPos.y = GetPos().y + 0.1f;

			{
				RayInfo rayInfo(rayPos, Math::Vector3(1.0f, 0.0f, 0.0f), 13.0f);

				BumpResult bumpResult;

				spObject->CheckCollisionBump(rayInfo, bumpResult);

				if (bumpResult.m_isHit)
				{
					m_objectHit = true;
				}
				GameSystem::GetInstance().RequestChangeFlg(m_objectHit);
			}

			{
				RayInfo rayInfo(rayPos, Math::Vector3(-1.0f, 0.0f, 0.0f), 13.0f);

				BumpResult bumpResult;

				spObject->CheckCollisionBump(rayInfo, bumpResult);

				if (bumpResult.m_isHit)
				{
					m_objectHit = true;
				}
				GameSystem::GetInstance().RequestChangeFlg(m_objectHit);
			}
		}
	}
	if (!m_change)
	{
		for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
		{
			if (spObject->GetClassID() != GameObject::eStage &&
				spObject->GetClassID() != GameObject::eStageObject &&
				spObject->GetClassID() != GameObject::eStageObjectFix &&
				spObject->GetClassID() != GameObject::eLift&&
				spObject->GetClassID() != GameObject::eEnemy) {
				continue;
			}
			GameSystem::GetInstance().RequestChangeFlg(false);
		}
	}
}

void Player::UpdateWorldMatrix()
{
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

void Player::Update2D()
{
	m_worldPos.x = m_Cpos.x;

	m_changeTime++;
	
	if (m_changeTime >= MAX_TIME)
	{
		m_changeTime = MAX_TIME;
	}

}

void Player::Update3D()
{
	if (GameSystem::GetInstance().GetSceneName() != "Game") { return; }

	m_changeTime--;
	
	// ゲージが無くなったらダメージを受ける
	if (m_changeTime <= MIN_TIME)
	{
		m_changeTime = MIN_TIME;
		m_changeTime = MAX_TIME;

		std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
		spSoundInstance = GameSystem::GetInstance().WorkAudioManager().Play("Data/Sounds/dmg01.wav");
		spSoundInstance->SetVolume(0.2f);

		m_life -= 1;
	}
}