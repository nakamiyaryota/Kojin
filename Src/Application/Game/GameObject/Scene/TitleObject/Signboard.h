#pragma once
#include "../../GameObject.h"

class Signboard : public GameObject
{
public:
	Signboard() {}
	~Signboard() {}

	void Init() override;
	void Update() override;
	void Draw() override;
	void DrawTranslucent() override;

	// êŠ‚Ìİ’è
	void SetWorldMatrix(float x, float z, float y = 0.0f) { m_mWorld = Math::Matrix::CreateTranslation(x, y, z); }

	void SetModel(const std::shared_ptr<KdModelData> spModel);
	void SetSignboardNumber(int number) { m_SignboardNumber = number; }

	const GameObject::ClassID GetClassID() const override
	{
		return GameObject::eSignboard;
	}

	bool CheckCollisionBump(const SphereInfo& info, BumpResult& result, float dis = 0) override;

	const int GetNumber() const override { return m_SignboardNumber; }

private:

	void Release();

	int m_SignboardNumber;

};