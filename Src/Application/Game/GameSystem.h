#pragma once

class SceneObject;
class GameObject;

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

	void RequestChangeFlg(bool flg) { m_canChange = flg; }

	void SetStageNumber(int number) { m_stageNumber = number; }

	// オブジェクトの追加
	void AddObject(std::shared_ptr<GameObject> spObject)
	{
		if (spObject) { m_spGameObjects.push_back(spObject); }
	}
	void AddObject(std::shared_ptr<SceneObject> spObject)
	{
		if (spObject) { m_spSceneObjects.push_back(spObject); }
	}

	// ゲームオブジェクトのリストを毎取得する
	const std::list<std::shared_ptr<GameObject>>& GetObjects() 
	{ 
		return m_spGameObjects; 
	}
	// シーンオブジェクトのリストを毎取得する
	const std::list<std::shared_ptr<SceneObject>>& GetSceneObjects()
	{
		return m_spSceneObjects;
	}

	void SetCamera(std::shared_ptr<KdCamera> spCamera) { m_spCamera = spCamera; }

	const std::shared_ptr<KdCamera> GetCamera() const;

	// リソースファクトリーの実体を返す
	ResourceFactory& WorkResourceFactory() { return m_resourceFactory; }

	KdAudioManager& WorkAudioManager() { return m_audioManager; }

	const InputController& GetInputController() { return m_input; }

	const std::string GetSceneName() const { return m_nextSceneName; }

	const int GetStageNumber() const { return m_stageNumber; }

	// キャラクターにかかる重力
	static const float s_worldGravity;

private:
	void Release();

	void ChangeScene();
	
	KdModelWork m_sky;  // 空のモデル
	DirectX::SimpleMath::Matrix m_skyMat; // 空のワールド行列

	std::shared_ptr<KdCamera>   m_spCamera = nullptr; // カメラ

	// ゲームオブジェクトリスト
	std::list<std::shared_ptr<GameObject>>  m_spGameObjects;
	std::list<std::shared_ptr<SceneObject>>  m_spSceneObjects;

	// リソース管理クラス
	ResourceFactory m_resourceFactory;

	// オーディオ管理クラス
	KdAudioManager m_audioManager;

	// 入力管理
	InputController m_input;
	
	std::string m_nextSceneName = "Title";
	bool m_isRequestChangeScene = false;

	int m_stageNumber;

	bool m_change = false;
	bool m_canChange = false;

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