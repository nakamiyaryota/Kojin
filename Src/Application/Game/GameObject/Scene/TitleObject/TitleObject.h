#pragma once

#include "../../GameObject.h"

class TitleObject : public GameObject
{
public:
	TitleObject();
	~TitleObject();

	void Init() override;
	void Draw2D() override;
	void Update() override;



private:
	void Release();

	std::shared_ptr<KdTexture> m_spTitleTex;
};
