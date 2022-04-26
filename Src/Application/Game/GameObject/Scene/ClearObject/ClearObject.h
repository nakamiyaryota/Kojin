#pragma once
#include "../../GameObject.h"

class ClearObject : public GameObject
{
public:
	ClearObject() {}
	~ClearObject() {}

	void Init() override;
	void Draw2D() override;
	void Update() override;



private:
	void Release();

	std::shared_ptr<KdTexture> m_spClearTex;

};