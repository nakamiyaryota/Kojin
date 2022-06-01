#include "ResultObject.h"

ResultObject::ResultObject()
{
}

ResultObject::~ResultObject()
{
}

void ResultObject::Init()
{
	// テクスチャの読み込み
	m_spTex[0] = GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Result/ResultMain.png");
	m_spTex[1] = GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Result/Result.png");

	std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
	spSoundInstance = GameSystem::GetInstance().WorkAudioManager().Play("Data/Sounds/NES-RPG-A03-2(Town2-Loop100).wav", true);
	spSoundInstance->SetVolume(0.5f);
	
}

void ResultObject::Release()
{
}
