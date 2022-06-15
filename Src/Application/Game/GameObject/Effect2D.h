#pragma once

#include "GameObject.h"

class Effect2D : public GameObject
{
public:
	Effect2D() {}
	~Effect2D() {}

	void Init() override;
	void Update() override;
	void Draw() override {}
	void DrawEffect() override;
	void Change2D() override { m_change = false; }
	void Change3D() override { m_change = true; }

	// テクスチャ設定
	void SetTexture(const std::shared_ptr<KdTexture> spTex, float angleZ = 0.0f, float w = 1.0f, float h = 1.0f, Math::Color col = kWhiteColor);

	void SetAnimation(int splitX, int splitY, float speed = 1.0f, bool isLoop = false);

	void SetLifeSpan(int lifeSpan) { m_lifeSpan = lifeSpan; }
	const KdSquarePolygon GetPolyData() const { return m_poly; }

	void SetChangeDir(bool flg = false) { m_changeDir = flg; }

	void SetChange(bool flg = false) { m_change = flg; }

private:

	// 四角ポリゴン
	KdSquarePolygon m_poly;

	// アニメーションの速度
	float m_animSpd = 0.0f;

	// ループするかどうか
	bool m_isLoop = false;

	bool m_changeDir = false;

	int m_lifeSpan = -1;

	float m_angleZ = 0.0f;
};