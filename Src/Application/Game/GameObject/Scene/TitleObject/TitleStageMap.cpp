#include "TitleStageMap.h"
#include "Signboard.h"
#include "../../StageObject.h"
#include "../../Lift.h"
#include "../../Effect2D.h"

void TitleStageMap::Init()
{
	m_modelWork.SetModel(GameSystem::GetInstance().WorkResourceFactory().GetModelData("Data/Models/StageMap/StageMap1.gltf"));

	for (int i = 0; i < OBJECT_SIZE; i++)
	{
		std::shared_ptr<StageObject> spStageObject = std::make_shared<StageObject>();
		spStageObject->Init();

		if (i == 0)
		{
			spStageObject->SetModel(GameSystem::GetInstance().WorkResourceFactory().GetModelData("Data/Models/StageObject/StageObject4.gltf"));
			spStageObject->SetWorldMatrix(5.5f, 4.5f);
		}

		spStageObject->SetObjectNumber(i);
		spStageObject->Change2D();
		GameSystem::GetInstance().AddObject(spStageObject);
	}

	// 看板オブジェクトのインスタンス化
	for (int i = 0; i < 2; i++)
	{
		std::shared_ptr<Signboard> spSignboard = std::make_shared<Signboard>();
		
		spSignboard->Init();

		if (i == 0)
		{
			spSignboard->SetModel(GameSystem::GetInstance().WorkResourceFactory().GetModelData("Data/Models/Signboard/Signboard1.gltf"));
			spSignboard->SetWorldMatrix(0.0f, 2.0f);
			spSignboard->SetSignboardNumber(1);
		}
		if (i == 1)
		{
			spSignboard->SetModel(GameSystem::GetInstance().WorkResourceFactory().GetModelData("Data/Models/Signboard/Signboard2.gltf"));
			spSignboard->SetWorldMatrix(0.0f, 7.0f);
			spSignboard->SetSignboardNumber(2);
		}

		spSignboard->Change2D();
		GameSystem::GetInstance().AddObject(spSignboard);
	}

	// 2Dオブジェクトのインスタンス化
	for (int i = 0; i < 3; i++)
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

	for (int i = 0; i < 9; i++)
	{

		std::shared_ptr<Effect2D> spEffect = std::make_shared<Effect2D>();
		spEffect->Init();
		spEffect->SetPos(Math::Vector3(float((rand() % 11) - 5), 0.11f, (3.3f * i) - 2.5f));
		spEffect->SetChangeDir(true);
		spEffect->SetTexture(GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Game/Weed.png"), 0.0f, 1.0f, 0.5f);

		GameSystem::GetInstance().AddObject(spEffect);
	}
}

bool TitleStageMap::CheckCollisionBump(const SphereInfo& info, BumpResult& result, float dis)
{
	// TitleStageMapのメッシュ達と当たり判定
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

void TitleStageMap::Release()
{
}
