#include "ClearObject.h"

void ClearObject::Init()
{
	// テクスチャの読み込み
	m_spClearTex = GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Clear/Clear.png");

	std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
	spSoundInstance = GameSystem::GetInstance().WorkAudioManager().Play("Data/Sounds/NES-RPG-A11-3(Ending-Loop105).wav", true);
	spSoundInstance->SetVolume(0.5f);
}

void ClearObject::Draw2D()
{
	if (!m_spClearTex) { return; }

	// ビューポートを利用して解像度を得る
	Math::Viewport vp;
	D3D.GetViewport(vp);

	int posX = int(-(vp.width * 0.5f) + (m_spClearTex.get()->GetWidth() * 0.5f));
	int posY = int((vp.height * 0.5f) - (m_spClearTex.get()->GetHeight() * 0.5f));

	SHADER->m_spriteShader.SetMatrix(Math::Matrix::Identity);
	SHADER->m_spriteShader.DrawTex(m_spClearTex.get(), posX, posY);
}

void ClearObject::Update()
{
	
}

void ClearObject::Release()
{
}
