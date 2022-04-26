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
	m_spResultTex = GameSystem::GetInstance().WorkResourceFactory().GetTexture("Data/Textures/Result/Result.png");
}

void ResultObject::Draw2D()
{
	if (!m_spResultTex) { return; }

	// ビューポートを利用して解像度を得る
	Math::Viewport vp;
	D3D.GetViewport(vp);

	int posX = int(-(vp.width * 0.5f) + (m_spResultTex.get()->GetWidth() * 0.5f));
	int posY = int((vp.height * 0.5f) - (m_spResultTex.get()->GetHeight() * 0.5f));

	SHADER->m_spriteShader.SetMatrix(Math::Matrix::Identity);
	SHADER->m_spriteShader.DrawTex(m_spResultTex.get(), posX, posY);
}

void ResultObject::Update()
{
	
}

void ResultObject::Release()
{
}
