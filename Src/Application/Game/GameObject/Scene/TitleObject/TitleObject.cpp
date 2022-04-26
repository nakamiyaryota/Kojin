#include "TitleObject.h"

TitleObject::TitleObject()
{
}

TitleObject::~TitleObject()
{
}

void TitleObject::Init()
{
	// テクスチャの読み込み
	m_spTitleTex = GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Title/Title.png");

	std::shared_ptr<KdSoundInstance> spBgmInstance = nullptr;
	spBgmInstance = GameSystem::GetInstance().WorkAudioManager().Play("Data/Sounds/NES-RPG-A01-2(Title-Loop135).wav", true);
	spBgmInstance->SetVolume(0.5f);
}

void TitleObject::Draw2D()
{
	if (!m_spTitleTex) { return; }

	// ビューポートを利用して解像度を得る
	Math::Viewport vp;
	D3D.GetViewport(vp);

	int posX = int(-(vp.width * 0.5f) + (m_spTitleTex.get()->GetWidth() * 0.5f));
	int posY = int((vp.height * 0.5f) - (m_spTitleTex.get()->GetHeight() * 0.5f));

	SHADER->m_spriteShader.SetMatrix(Math::Matrix::Identity);
	SHADER->m_spriteShader.DrawTex(m_spTitleTex.get(), posX, posY);
}

void TitleObject::Update()
{
	
}

void TitleObject::Release()
{
}
