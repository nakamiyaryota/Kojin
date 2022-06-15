#include "Effect2D.h"

void Effect2D::Init()
{
}

void Effect2D::Update()
{
	// アニメーションが無い or ループアニメーション
	if (m_poly.GetAnimationSize() <= 1 || m_isLoop)
	{
		if (--m_lifeSpan == 0)
		{
			m_isAlive = false;
		}
	}

	// アニメーションしないのであれば更新せずに返る
	if (m_poly.GetAnimationSize() <= 1) { return; }

	m_poly.Animation(m_animSpd, m_isLoop);

	if (m_poly.IsAnimationEnd())
	{
		m_isAlive = false;
	}
}

void Effect2D::DrawEffect()
{
	Math::Matrix mDraw;

	// 拡縮
	Math::Vector3 scale;
	scale.x = m_mWorld.Right().Length();
	scale.y = m_mWorld.Up().Length();
	scale.z = m_mWorld.Forward().Length();

	// 回転
	if (m_changeDir)
	{
		float dir;
		if (m_change)
		{
			dir = 0.0f;
		}
		if (!m_change)
		{
			dir = 90.0f;
		}
		mDraw = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(dir));
	}
	if (!m_changeDir)
	{
		mDraw = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(90.0f));
	}

	// 移動
	mDraw.Translation(m_mWorld.Translation());

	SHADER->m_effectShader.DrawSquarePolygon(m_poly, mDraw);
}

void Effect2D::SetTexture(const std::shared_ptr<KdTexture> spTex, float angleZ, float w, float h, Math::Color col)
{
	if (!spTex) { return; }
	m_poly.SetTexture(spTex);

	// ポリゴンの設定
	m_poly.Init(w, h, col);

	m_angleZ = angleZ;
}

void Effect2D::SetAnimation(int splitX, int splitY, float speed, bool isLoop)
{
	m_poly.SetAnimationInfo(splitX, splitY);

	// アニメーションの設定
	m_animSpd = speed;
	m_isLoop = isLoop;
}
