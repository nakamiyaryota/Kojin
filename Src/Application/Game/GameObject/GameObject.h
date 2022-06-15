#pragma once

struct SphereInfo
{
	SphereInfo() = default; // デフォルトコンストラクタ

	SphereInfo(const Math::Vector3& pos, float radius) :
		m_pos(pos),
		m_radius(radius) {}

	Math::Vector3 m_pos;
	float m_radius = 1.0f;
};

// レイの判定用情報(レイの発射側)
struct RayInfo
{
	RayInfo() {} // デフォルトコンストラクタ

	RayInfo(const Math::Vector3& pos, const Math::Vector3& dir, float range) :
		m_pos(pos),
		m_dir(dir),
		m_range(range) 
	{ m_dir.Normalize(); }

	Math::Vector3 m_pos;
	Math::Vector3 m_dir;
	float m_range = 0.0f;
};

// 物体同士の当たり判定結果
struct BumpResult
{
	bool m_isHit = false;
	Math::Vector3 m_pushVec;
	Math::Vector3 m_HitPos;  // レイがHITした位置
	float m_Distance = 0.0f; // レイがHITした際の本体との距離
	Math::Vector3 m_pos;
};

class GameObject
{
public:
	enum ClassID
	{
		eBase,
		eStage,
		ePlayer,
		eEnemy,
		eArrow,
		eStageObject,
		eStageObjectFix,
		eLift,
		eGoal,
		eSignboard
	};

	GameObject() {}
	virtual ~GameObject() { Release(); }

	virtual void ImGuiProcess() {}

	virtual void Init() {}
	virtual void Update() {}
	virtual void DrawShadowMap();
	virtual void Draw();
	virtual void DrawTranslucent() {} // 半透明描画
	virtual void DrawEffect() {}
	virtual void Draw2D() {}
	virtual void Change3D(); // 3Dモードのチェンジした時に行う処理
	virtual void Change2D(); // 2Dモードのチェンジした時に行う処理

	void Destroy() { m_isAlive = false; }

	virtual const Math::Vector3 GetPos() const { return m_mWorld.Translation(); }
	const Math::Matrix GetMatrix() const { return m_mWorld; }

	// キャラクターのi位置を設定する
	virtual void SetPos(const Math::Vector3& pos)
	{
		m_mWorld.Translation(pos);
	}


	virtual const GameObject::ClassID GetClassID() const
	{
		return GameObject::eBase;
	}

	bool IsAlive() { return m_isAlive; }
	void SetHit(bool hit) { m_hit = hit; }
	virtual bool GetHit() { return m_hit; }

	// 物体同士の当たり判定

	// 球対なにかの当たり判定
	// info：相手の球情報
	// result：当たったかどうか及び、押し返すベクトル
	// 戻り値：当たったかどうか
	virtual bool CheckCollisionBump(const SphereInfo& info, BumpResult& result, float dis = 0);

	// レイ対なにかの当たり判定
	// info：相手の球情報
	// result：当たったかどうか及び、押し返すベクトル
	// 戻り値：当たったかどうか
	bool CheckCollisionBump(const RayInfo& info, BumpResult& result);

	virtual const bool GetHit() const { return nullptr; }
	
	virtual const int GetNumber() const { return NULL; }

protected:
	KdModelWork m_modelWork;
	Math::Matrix m_mWorld;
	Math::Vector3 m_keepPos = m_mWorld.Translation();
	bool m_change = false;

	bool m_isAlive = true;
	bool m_hit = false;

	// このキャラクターの球情報
	SphereInfo m_bumpSphereInfo;
private:
	void Release() {}

};
