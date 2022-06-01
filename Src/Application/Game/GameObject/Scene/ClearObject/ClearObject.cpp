#include "ClearObject.h"

void ClearObject::Init()
{
	// テクスチャの読み込み
	m_spTex[0] = GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Clear/ClearMain.png");
	m_spTex[1] = GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Clear/Clear.png");

	std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
	spSoundInstance = GameSystem::GetInstance().WorkAudioManager().Play("Data/Sounds/NES-RPG-A11-3(Ending-Loop105).wav", true);
	spSoundInstance->SetVolume(0.5f);
}

void ClearObject::Release()
{
}
