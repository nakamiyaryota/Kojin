#pragma once
#include "GameObject.h"

class StageObject : public GameObject
{
public:
	StageObject() {}
	~StageObject() {}

	void Init() override;
	void Update() override;
	void Draw() override;
	void DrawTranslucent() override;

	// èÍèäÇÃê›íË
	void SetWorldMatrix(float x, float z, float y = 0.0f) { m_mWorld = Math::Matrix::CreateTranslation(x, y, z); }
	void SetModel(const std::shared_ptr<KdModelData> spModel);

	const GameObject::ClassID GetClassID() const override
	{
		return GameObject::eStageObject;
	}
	
	bool CheckCollisionBump(const SphereInfo& info, BumpResult& result, float dis = 0) override;

private:

	void Release();
};