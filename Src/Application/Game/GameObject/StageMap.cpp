#include "StageMap.h"
#include "FirstEnemy.h"
#include "SecondEnemy.h"
#include "StageObject.h"
#include "StageObjectFix.h"
#include "Lift.h"
#include "Goal.h"
#include "Effect2D.h"

void StageMap::Init()
{
	if (!GameSystem::GetInstance().GetStageNumber()) { return; }

	// Jsonファイル読み込み
	m_jsonObjFileName = KdLoadJSON("Data/Json/StageObjectFileName.json");
	m_jsonObjPosition = KdLoadJSON("Data/Json/StageObjectPosition.json");

	// ステージセレクト
	if (GameSystem::GetInstance().GetStageNumber() == 1)
	{
		FirstStageInit();
	}

	if (GameSystem::GetInstance().GetStageNumber() == 2)
	{
		SecondStageInit();
	}

	std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
	spSoundInstance = GameSystem::GetInstance().WorkAudioManager().Play("Data/Sounds/NES-RPG-B01-1(Title).wav", true);
	spSoundInstance->SetVolume(0.5f);
}

void StageMap::FirstStageInit()
{
	m_modelWork.SetModel(GameSystem::GetInstance().WorkResourceFactory().GetModelData("Data/Models/StageMap/StageMap2.gltf"));

	std::shared_ptr<Goal> spGoal = std::make_shared<Goal>();
	spGoal->Init();
	spGoal->SetWorldMatrix(0.0f, 40.0f, 0.0f);
	spGoal->Change2D();
	GameSystem::GetInstance().AddObject(spGoal);

	// 敵をインスタンス化
	for (int i = 0; i < 1; i++)
	{
		std::shared_ptr<FirstEnemy> spEnemy = std::make_shared<FirstEnemy>();
		spEnemy->Init();

		if (i == 0)
		{
			spEnemy->SetWorldMatrix(4.0f, 10.0f);
			spEnemy->SetActionRange(6.0f, 13.0f);
		}

		spEnemy->Change3D();
		spEnemy->Change2D();
		GameSystem::GetInstance().AddObject(spEnemy);
	}
	for (int i = 0; i < 1; i++)
	{
		std::shared_ptr<SecondEnemy> spEnemy = std::make_shared<SecondEnemy>();
		spEnemy->Init();

		if (i == 0)
		{
			spEnemy->SetWorldMatrix(-4.0f, 30.0f);
		}

		spEnemy->Change3D();
		spEnemy->Change2D();
		GameSystem::GetInstance().AddObject(spEnemy);
	}

	for (int i = 0; i < m_jsonObjFileName["ObjectSize1"].number_value(); i++)
	{
		std::shared_ptr<StageObject> spStageObject = std::make_shared<StageObject>();
		spStageObject->Init();

		spStageObject->SetModel(GameSystem::GetInstance().WorkResourceFactory().GetModelData(m_jsonObjFileName["ModelFilename" + std::to_string(GameSystem::GetInstance().GetStageNumber()) + "_" + std::to_string(i)].string_value()));
		
		float pos[3];
		pos[0] = (float)m_jsonObjPosition["ObjectPos" + std::to_string(GameSystem::GetInstance().GetStageNumber()) + "_" + std::to_string(i) + "0"].number_value();
		pos[1] = (float)m_jsonObjPosition["ObjectPos" + std::to_string(GameSystem::GetInstance().GetStageNumber()) + "_" + std::to_string(i) + "1"].number_value();
		pos[2] = (float)m_jsonObjPosition["ObjectPos" + std::to_string(GameSystem::GetInstance().GetStageNumber()) + "_" + std::to_string(i) + "2"].number_value();
		spStageObject->SetWorldMatrix(pos[0], pos[2], pos[1]);

		spStageObject->SetObjectNumber(i);
		spStageObject->Change2D();
		GameSystem::GetInstance().AddObject(spStageObject);
	}

	// 2Dオブジェクトのインスタンス化
	for (int i = 0; i < 6; i++)
	{
		// 山描画
		{
			std::shared_ptr<Effect2D> spEffect = std::make_shared<Effect2D>();
			spEffect->Init();
			spEffect->SetPos(Math::Vector3(-6.0f, 0.0f, (10.0f * i) - 1.5f));
			spEffect->SetTexture(GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Game/Mountain.png"), 0.0f, 8.0f, 16.0f);

			GameSystem::GetInstance().AddObject(spEffect);
		}

		// 木描画
		{
			std::shared_ptr<Effect2D> spEffect = std::make_shared<Effect2D>();

			spEffect->Init();
			spEffect->SetPos(Math::Vector3(-6.0f, 2.3f, 10.0f * i));
			spEffect->SetTexture(GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Game/Tree.png"), 0.0f, 4.0f, 5.0f);

			GameSystem::GetInstance().AddObject(spEffect);
		}

		// 草描画
		{
			std::shared_ptr<Effect2D> spEffect = std::make_shared<Effect2D>();
			spEffect->Init();
			spEffect->SetPos(Math::Vector3(-6.0f, 0.5f, (10.0f * i) - 2.0f));
			spEffect->SetTexture(GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Game/Green.png"), 0.0f, 2.0f, 4.0f);

			GameSystem::GetInstance().AddObject(spEffect);
		}

	}

	for (int i = 0; i < 18; i++)
	{
		std::shared_ptr<Effect2D> spEffect = std::make_shared<Effect2D>();
		spEffect->Init();
		spEffect->SetPos(Math::Vector3(float((rand() % 11) - 5), 0.11f, (3.3f * i) - 2.5f));
		spEffect->SetChangeDir(true);
		spEffect->SetTexture(GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Game/Weed.png"), 0.0f, 1.0f, 0.5f);

		GameSystem::GetInstance().AddObject(spEffect);
	}
}

void StageMap::SecondStageInit()
{
	m_modelWork.SetModel(GameSystem::GetInstance().WorkResourceFactory().GetModelData("Data/Models/StageMap/StageMap3.gltf"));

	std::shared_ptr<Goal> spGoal = std::make_shared<Goal>();
	spGoal->Init();
	spGoal->SetWorldMatrix(0.0f, 72.0f, 11.5f);
	spGoal->Change2D();
	GameSystem::GetInstance().AddObject(spGoal);

	// 敵をインスタンス化
	for (int i = 0; i < ENEMY_SIZE; i++)
	{
		std::shared_ptr<SecondEnemy> spEnemy = std::make_shared<SecondEnemy>();
		spEnemy->Init();

		if (i == 0)
		{
			spEnemy->SetWorldMatrix(0.0f, 13.0f);
		}
		if (i == 1)
		{
			spEnemy->SetWorldMatrix(2.0f, 50.0f);
		}
		if (i == 2)
		{
			spEnemy->SetWorldMatrix(-3.0f, 70.0f);
		}


		spEnemy->Change3D();
		spEnemy->Change2D();
		GameSystem::GetInstance().AddObject(spEnemy);
	}

	// ステージオブジェクトをインスタンス化
	for (int i = 0; i < OBJECT_SIZE; i++)
	{
		std::shared_ptr<StageObject> spStageObject = std::make_shared<StageObject>();
		spStageObject->Init();

		spStageObject->SetModel(GameSystem::GetInstance().WorkResourceFactory().GetModelData(m_jsonObjFileName["ModelFilename" + std::to_string(GameSystem::GetInstance().GetStageNumber()) + "_" + std::to_string(i)].string_value()));

		if (i == 0)
		{
			spStageObject->SetWorldMatrix(5.5f, 6.0f);
			
		}
		if (i == 1)
		{
			spStageObject->SetWorldMatrix(-5.0f, 21.0f);
		}
		if (i == 2)
		{
			spStageObject->SetWorldMatrix(5.0f, 23.0f);
		}
		if (i == 3)
		{
			spStageObject->SetWorldMatrix(4.5f, 27.5f, -3.0f);
		}
		if (i == 4)
		{
			spStageObject->SetWorldMatrix(1.5f, 28.5f, -2.0f);
		}
		if (i == 5)
		{
			spStageObject->SetWorldMatrix(-1.5f, 29.5f, -1.0f);
		}
		if (i == 6)
		{
			spStageObject->SetWorldMatrix(-4.5f, 30.5f, 0.0f);
		}
		if (i == 7)
		{
			spStageObject->SetWorldMatrix(0.0f, 33.0f, 0.0f);
		}
		if (i == 8)
		{
			spStageObject->SetWorldMatrix(0.0f, 34.0f, -1.0f);
		}
		if (i == 9)
		{
			spStageObject->SetWorldMatrix(0.0f, 35.0f, -2.0f);
		}
		if (i == 10)
		{
			spStageObject->SetWorldMatrix(0.0f, 36.0f, -3.0f);
		}
		if (i == 11)
		{
			spStageObject->SetWorldMatrix(4.5f, 45.0f, 0.0f);
		}
		if (i == 12)
		{
			spStageObject->SetWorldMatrix(5.5f, 42.0f, -3.0f);
		}
		if (i == 13)
		{
			spStageObject->SetWorldMatrix(5.5f, 43.0f, -2.0f);
		}
		if (i == 14)
		{
			spStageObject->SetWorldMatrix(5.5f, 44.0f, -1.0f);
		}
		if (i == 15)
		{
			spStageObject->SetWorldMatrix(5.5f, 45.0f, 0.0f);
		}
		if (i == 16)
		{
			spStageObject->SetWorldMatrix(5.5f, 58.0f, 0.0f);
		}
		if (i == 17)
		{
			spStageObject->SetWorldMatrix(-2.5f, 69.5f, 11.5f);
		}
		if (i == 18)
		{
			spStageObject->SetWorldMatrix(2.5f, 74.5f, 11.5f);
		}
		if (i == 19)
		{
			spStageObject->SetWorldMatrix(2.5f, 72.0f, 16.0f);
		}

		spStageObject->SetObjectNumber(i);
		spStageObject->Change2D();
		GameSystem::GetInstance().AddObject(spStageObject);
	}

	// ステージオブジェクト(固定)をインスタンス化
	for (int i = 0; i < OBJECTFIX_SIZE; i++)
	{
		std::shared_ptr<StageObjectFix> spStageObjectFix = std::make_shared<StageObjectFix>();
		spStageObjectFix->Init();

		if (i == 0)
		{
			spStageObjectFix->SetModel(GameSystem::GetInstance().WorkResourceFactory().GetModelData("Data/Models/StageObject/StageObject9.gltf"));
			spStageObjectFix->SetWorldMatrix(0.0f, 72.0f, 11.5f);
		}

		GameSystem::GetInstance().AddObject(spStageObjectFix);
	}

	// リフトオブジェクトをインスタンス化
	for (int i = 0; i < LIFT_SIZE; i++)
	{
		std::shared_ptr<Lift> spLift = std::make_shared<Lift>();
		spLift->Init();

		if (i == 0)
		{
			spLift->SetRoute(Math::Vector3(-4.0f, 4.0f, 49.5f), Math::Vector3(-4.0f, 4.0f, 56.5f));
		}
		if (i == 1)
		{
			spLift->SetRoute(Math::Vector3(0.0f, 4.0f, 60.0f), Math::Vector3(0.0f, 7.0f, 60.0f));
			spLift->SetSpeedByTime(4.0f);
		}
		if (i == 2)
		{
			spLift->SetRoute(Math::Vector3(0.0f, 9.5f, 63.0f), Math::Vector3(0.0f, 6.5f, 63.0f));
			spLift->SetSpeedByTime(4.0f);
		}
		if (i == 3)
		{
			spLift->SetRoute(Math::Vector3(0.0f, 9.0f, 66.0f), Math::Vector3(0.0f, 12.0f, 66.0f));
			spLift->SetSpeedByTime(4.0f);
		}

		spLift->Change2D();
		GameSystem::GetInstance().AddObject(spLift);
	}

	// 2Dオブジェクトのインスタンス化
	for (int i = 0; i < 10; i++)
	{
		// 山描画
		{
			std::shared_ptr<Effect2D> spEffect = std::make_shared<Effect2D>();
			spEffect->Init();
			spEffect->SetPos(Math::Vector3(-6.0f, 0.0f, (10.0f * i) - 1.5f));
			spEffect->SetTexture(GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Game/Mountain.png"), 0.0f, 8.0f, 16.0f);

			GameSystem::GetInstance().AddObject(spEffect);
		}

		// 木描画
		{
			std::shared_ptr<Effect2D> spEffect = std::make_shared<Effect2D>();

			spEffect->Init();
			spEffect->SetPos(Math::Vector3(-6.0f, 2.3f, 10.0f * i));
			spEffect->SetTexture(GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Game/Tree.png"), 0.0f, 4.0f, 5.0f);

			GameSystem::GetInstance().AddObject(spEffect);
		}

		// 草描画
		{
			std::shared_ptr<Effect2D> spEffect = std::make_shared<Effect2D>();
			spEffect->Init();
			spEffect->SetPos(Math::Vector3(-6.0f, 0.5f, (10.0f * i) - 2.0f));
			spEffect->SetTexture(GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Game/Green.png"), 0.0f, 2.0f, 4.0f);

			GameSystem::GetInstance().AddObject(spEffect);
		}

	}

	for (int i = 0; i < 30; i++)
	{
		if (i != 7 && i != 8) {
			std::shared_ptr<Effect2D> spEffect = std::make_shared<Effect2D>();
			spEffect->Init();
			spEffect->SetPos(Math::Vector3(float((rand() % 11) - 5), 0.11f, (3.3f * i) - 2.5f));
			spEffect->SetChangeDir(true);
			spEffect->SetTexture(GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Game/Weed.png"), 0.0f, 1.0f, 0.5f);

			GameSystem::GetInstance().AddObject(spEffect);
		}
	}
}

bool StageMap::CheckCollisionBump(const SphereInfo& info, BumpResult& result, float dis)
{
	// StageMapのメッシュ達と当たり判定
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

void StageMap::Release()
{
}
