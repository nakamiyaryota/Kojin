#pragma once

#include "../../GameObject.h"

class ResultObject : public GameObject
{
public:
	ResultObject();
	~ResultObject();

	void Init() override;
	void Draw2D() override;
	void Update() override;



private:
	void Release();

	std::shared_ptr<KdTexture> m_spResultTex;
};
