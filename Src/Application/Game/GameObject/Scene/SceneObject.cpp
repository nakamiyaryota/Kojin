#include "SceneObject.h"

void SceneObject::Draw2D()
{
	// ビューポートを利用して解像度を得る
	Math::Viewport vp;
	D3D.GetViewport(vp);

	int posX = int(-(vp.width * 0.5f) + (m_spTex[0].get()->GetWidth() * 0.5f));
	int posY = int((vp.height * 0.5f) - (m_spTex[0].get()->GetHeight() * 0.5f));

	SHADER->m_spriteShader.SetMatrix(Math::Matrix::Identity);
	SHADER->m_spriteShader.DrawTex(m_spTex[0].get(), posX, posY);
	SHADER->m_spriteShader.DrawTex(m_spTex[1].get(), posX, posY, nullptr, &Math::Color(1.0f, 1.0f, 1.0f, m_alpha));
}

void SceneObject::Update()
{
	if (m_alpha < 0.0f)
	{
		m_alpha = 0.0f;
		a *= -1.0f;
	}
	if (m_alpha > 1.0f)
	{
		m_alpha = 1.0f;
		a *= -1.0f;
	}

	m_alpha += a;
}
