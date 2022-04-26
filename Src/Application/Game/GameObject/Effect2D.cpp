#include "Effect2D.h"

void Effect2D::Init()
{
	// ポリゴンを作成
	//m_poly.SetTexture(GameResourceFactory.GetTexture("Data/Textures/Explosion.png"));
	//m_poly.Init(1.0f, 1.0f, kWhiteColor);
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

	//mDraw = Math::Matrix::CreateScale(scale);

	// 回転
	std::shared_ptr<KdCamera> gameCam = GameSystem::GetInstance().GetCamera();
	mDraw = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(90.0f));

	if (gameCam)
	{
		// カメラの逆行列の合成
		Math::Matrix mCamInv = gameCam->GetCameraMatrix();
		mCamInv.Invert();

		// カメラの目の前に持ってくる
		//mDraw *= mCamInv;
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
