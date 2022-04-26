#pragma once

#include "GameObject.h"

class StageObjectFix : public GameObject
{
public:
	StageObjectFix() {}
	~StageObjectFix() {}

	void Init() override;
	void Update() override;
	void Draw() override;
	void Change2D() override {}
	void Change3D() override {}
	void DrawTranslucent() override;

	// èÍèäÇÃê›íË
	void SetWorldMatrix(float x, float z, float y = 0.0f) { m_mWorld = Math::Matrix::CreateTranslation(x, y, z); }
	void SetModel(const std::shared_ptr<KdModelData> spModel);

	const GameObject::ClassID GetClassID() const override
	{
		return GameObject::eStageObjectFix;
	}

	bool CheckCollisionBump(const SphereInfo& info, BumpResult& result, float dis = 0) override;

private:

	void Release();

};
