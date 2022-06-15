#pragma once
#include "GameObject.h"

class StageObject : public GameObject
{
public:
	StageObject() {}
	~StageObject() {}

	void ImGuiProcess() override;

	void Init() override;
	void Update() override;
	void Draw() override;
	void DrawTranslucent() override;
	void Change3D() override; // 3Dモードのチェンジした時に行う処理
	void Change2D() override; // 2Dモードのチェンジした時に行う処理

	// 場所の設定
	//void SetWorldMatrix(float x, float z, float y = 0.0f) { m_mWorld = Math::Matrix::CreateTranslation(x, y, z); }
	void SetWorldMatrix(float x, float z, float y = 0.0f) { m_worldPos.x = x; m_worldPos.y = y; m_worldPos.z = z; }
	void SetModel(const std::shared_ptr<KdModelData> spModel);

	const GameObject::ClassID GetClassID() const override
	{
		return GameObject::eStageObject;
	}
	
	bool CheckCollisionBump(const SphereInfo& info, BumpResult& result, float dis = 0) override;

	void SetObjectNumber(int num) { m_ObjectNumber = num + 1; }

private:

	void UpdateWorldMatrix();

	int m_ObjectNumber;

	Math::Vector3 m_worldPos;
	Math::Vector3 m_worldScale;

	void Release();
};