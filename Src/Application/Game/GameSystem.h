#pragma once

class GameObject;

class DebugCamera;

class GameSystem
{
public:

	void TitleInit();
	void GameInit();
	void ResultInit();
	void ClearInit();
	void Init();
	void Update();
	void Draw();

	void RequestChangeScene(const std::string& name);

	void RequestChange(bool flg) { m_limit = flg; }

	// オブジェクトの追加
	void AddObject(std::shared_ptr<GameObject> spObject)
	{
		if (spObject) { m_spObjects.push_back(spObject); }
	}

	// ゲームオブジェクトのリストを毎取得する
	const std::list<std::shared_ptr<GameObject>>& GetObjects() 
	{ 
		return m_spObjects; 
	}

	void SetCamera(std::shared_ptr<KdCamera> spCamera) { m_spCamera = spCamera; }

	const std::shared_ptr<KdCamera> GetCamera() const;

	// リソースファクトリーの実体を返す
	ResourceFactory& WorkResourceFactory() { return m_resourceFactory; }

	KdAudioManager& WorkAudioManager() { return m_audioManager; }

	const InputController& GetInputController() { return m_input; }

	// キャラクターにかかる重力
	static const float s_worldGravity;
	// static：インスタンスが複製されてもメモリを共有する変数
	// const：初期化以外の代入を禁止された変数

private:
	void Release();

	void ChangeScene();
	
	KdModelWork m_sky;  // 空のモデル
	DirectX::SimpleMath::Matrix m_skyMat; // 空のワールド行列

	std::shared_ptr<KdCamera>   m_spCamera = nullptr; // カメラ

	std::shared_ptr<DebugCamera> m_spDebugCamera = nullptr;

	// ゲームオブジェクトリスト
	std::list<std::shared_ptr<GameObject>>  m_spObjects;

	// リソース管理クラス
	ResourceFactory m_resourceFactory;

	// オーディオ管理クラス
	KdAudioManager m_audioManager;

	// 入力管理
	InputController m_input;
	
	std::string m_nextSceneName = "Title";
	bool m_isRequestChangeScene = false;

	bool m_change = false;
	bool m_limit = false;

// シングルトン
private:
	// コンストラクター
	GameSystem() {}

	// デストラクター
	~GameSystem() { Release(); }

public:
	// GameSystemインスタンスの取得
	static GameSystem& GetInstance()
	{
		static GameSystem instance;

		return instance;
	}
};

// GameSystemを呼び出すマクロ
//#define GameInstance GameSystem::GetInstance()

// マクロで呼び出す
//#define GameResourceFactory GameSystem::GetInstance().WorkResourceFactory()

//#define GameAudioManager GameSystem::GetInstance().WorkAudioManager()

//#define GameInput GameInstance.GetInputController()