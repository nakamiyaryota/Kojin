#include "TitleObject.h"
#include "TitleStageMap.h"

TitleObject::TitleObject()
{
}

TitleObject::~TitleObject()
{
}

void TitleObject::Init()
{
	// テクスチャの読み込み
	m_spTex[0] = GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Title/GameTitle.png");
	m_spTex[1] = GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Title/Title.png");

	std::shared_ptr<TitleStageMap> spTitleStage = std::make_shared<TitleStageMap>();
	spTitleStage->Init();
	spTitleStage->Change2D();
	GameSystem::GetInstance().AddObject(spTitleStage);

	std::shared_ptr<KdSoundInstance> spBgmInstance = nullptr;
	spBgmInstance = GameSystem::GetInstance().WorkAudioManager().Play("Data/Sounds/NES-RPG-A01-2(Title-Loop135).wav", true);
	spBgmInstance->SetVolume(0.5f);
}

void TitleObject::Release()
{
}
