#include "GameSystem.h"
#include "../main.h"
#include "GameObject/StageMap.h"
#include "GameObject/Player.h"
#include "Camera/TPSCamera.h"
#include "GameObject/Effect2D.h"
#include "GameObject/Enemy.h"

#include "GameObject/Scene/TitleObject/TitleObject.h"
#include "GameObject/Scene/ResultObject/ResultObject.h"
#include "GameObject/Scene/ClearObject/ClearObject.h"

// プログラム開始時に一度だけ確保＆初期化
const float GameSystem::s_worldGravity = 0.01f;

void GameSystem::TitleInit()
{
	Release();

	// モデル読み込み
	m_sky.SetModel(m_resourceFactory.GetModelData("Data/Models/SkySphere/SkySphere.gltf"));

	// 空を拡大させる
	m_skyMat = m_skyMat.CreateScale(200.0f);

	// タイトルオブジェクトのインスタンス化
	std::shared_ptr<TitleObject> spTitle = std::make_shared<TitleObject>();
	spTitle->Init();
	AddObject(spTitle);

	// プレイヤーオブジェクトのインスタンス化
	std::shared_ptr<Player> spPlayer = std::make_shared<Player>();
	spPlayer->Init();
	spPlayer->Change2D();
	AddObject(spPlayer);

	// ポリゴンを作成
	std::shared_ptr<Effect2D> spEffect = std::make_shared<Effect2D>();
	spEffect->Init();
	spEffect->SetAnimation(5, 5);
	AddObject(spEffect);
}

void GameSystem::GameInit()
{
	Release();

	// モデル読み込み
	m_sky.SetModel(m_resourceFactory.GetModelData("Data/Models/SkySphere/SkySphere.gltf"));

	// 空を拡大させる
	m_skyMat = m_skyMat.CreateScale(200.0f);

	// ステージオブジェクトのインスタンス化
	std::shared_ptr<StageMap> spStage = std::make_shared<StageMap>();
	spStage->Init();
	spStage->Change2D();
	AddObject(spStage);

	// プレイヤーオブジェクトのインスタンス化
	std::shared_ptr<Player> spPlayer = std::make_shared<Player>();
	spPlayer->Init();
	spPlayer->Change2D();
	AddObject(spPlayer);

	// ポリゴンを作成
	std::shared_ptr<Effect2D> spEffect = std::make_shared<Effect2D>();
	spEffect->Init();
	spEffect->SetAnimation(5, 5);
	AddObject(spEffect);
}

void GameSystem::ResultInit()
{
	m_audioManager.StopAllSound();

	// リザルトオブジェクトのインスタンス化
	std::shared_ptr<ResultObject> spResult = std::make_shared<ResultObject>();
	spResult->Init();
	AddObject(spResult);

	m_change = false;
}

void GameSystem::ClearInit()
{
	m_audioManager.StopAllSound();

	// リザルトオブジェクトのインスタンス化
	std::shared_ptr<ClearObject> spClear = std::make_shared<ClearObject>();
	spClear->Init();
	AddObject(spClear);

	m_change = false;
}

void GameSystem::Init()
{
	m_audioManager.Init();

	TitleInit();
}

void GameSystem::Update()
{
	m_input.Update();
	
	// シーン変更
	if (m_nextSceneName == "Result")
	{
		if (GameSystem::GetInstance().GetInputController().GetKeyState("ChangeScene") & eKeyPress)
		{
			GameSystem::GetInstance().RequestChangeScene("Game");
		}
	}
	if (m_nextSceneName == "Clear")
	{
		if (GameSystem::GetInstance().GetInputController().GetKeyState("ChangeScene") & eKeyPress)
		{
			GameSystem::GetInstance().RequestChangeScene("Title");
		}
	}

	if (GameSystem::GetInstance().GetInputController().GetKeyState("End") & eKeyPress)
	{
		if (MessageBoxA(APP.m_window.GetWndHandle(), "本当にゲームを終了しますか？", "確認", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES)
		{
			APP.End();
		}
	}


	if (m_nextSceneName == "Title" || m_nextSceneName == "Game")
	{
		if (GameSystem::GetInstance().GetInputController().GetKeyState("ChangeWorld") & eKeyPress)
		{
			if (m_canChange)
			{
				std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
				spSoundInstance = m_audioManager.Play("Data/Sounds/beep25.wav");
				spSoundInstance->SetVolume(0.1f);
			}
			if (!m_canChange)
			{
				std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
				spSoundInstance = m_audioManager.Play("Data/Sounds/button43.wav");
				spSoundInstance->SetVolume(0.15f);

				m_change = !m_change;
				if (m_change)
				{
					for (std::shared_ptr<GameObject>& spObject : m_spGameObjects)
					{
						spObject->Change3D();
					}
				}
				if (!m_change)
				{
					std::shared_ptr<Player> spPlayer = std::make_shared<Player>();
					spPlayer->Change2D();

					if (m_startChange)
					{
						for (std::shared_ptr<GameObject>& spObject : m_spGameObjects)
						{
							spObject->Change2D();
						}
					}
				}
			}
		}
	}

	if (m_isRequestChangeScene)
	{
		ChangeScene();
		return;
	}

	// シーンオブジェクトの更新
	for (std::shared_ptr<SceneObject>& spObject : m_spSceneObjects)
	{
		spObject->Update();
	}
	// ゲームオブジェクトの更新
	for (std::shared_ptr<GameObject>& spObject : m_spGameObjects)
	{
		spObject->Update();
	}

	// 死んだオブジェクト除外する
	std::list<std::shared_ptr<GameObject>>::iterator objItr = m_spGameObjects.begin();

	while (objItr != m_spGameObjects.end())
	{
		if (!(*objItr)->IsAlive())
		{
			// スマートポインタは解放は自動で行われる
			objItr->reset();

			objItr = m_spGameObjects.erase(objItr);

			continue;
		}

		++objItr;
	}

	// サウンド関連の更新
	{
		Math::Matrix listenerMat;

		if (m_spCamera)
		{
			listenerMat = m_spCamera->GetCameraMatrix();
		}
		m_audioManager.Update(listenerMat.Translation(), listenerMat.Backward());
	}
}

void GameSystem::Draw()
{
	if (m_spCamera)
	{
		m_spCamera->SetToShader();
	}

	// 不透明物から描画
	SHADER->m_standardShader.SetToDevice();

	// ゲームオブジェクトの描画
	for (std::shared_ptr<GameObject>& spObject : m_spGameObjects)
	{
		spObject->Draw();
	}

	SHADER->m_effectShader.SetToDevice();
	SHADER->m_effectShader.DrawModel(m_sky, m_skyMat);

	{
		D3D.WorkDevContext()->OMSetDepthStencilState(SHADER->m_ds_ZEnable_ZWriteDisable, 0);

		// カリングなし(両面描画)
		D3D.WorkDevContext()->RSSetState(SHADER->m_rs_CullNone);

		// ゲームオブジェクト(透明物)の描画
		for (std::shared_ptr<GameObject>& spObject : m_spGameObjects)
		{
			spObject->DrawEffect();
		}

		D3D.WorkDevContext()->OMSetDepthStencilState(SHADER->m_ds_ZEnable_ZWriteEnable, 0);

		// 裏面カリング(表面のみ描画)
		D3D.WorkDevContext()->RSSetState(SHADER->m_rs_CullBack);
	}

	SHADER->m_translucentShader.SetToDevice();

	// ゲームオブジェクトの描画
	for (std::shared_ptr<GameObject>& spObject : m_spGameObjects)
	{
		spObject->DrawTranslucent();
	}

	// 2D系描画をまとめて行う
	SHADER->m_spriteShader.Begin();
	{
		// 2D系描画はこの範囲で行う
		for (std::shared_ptr<SceneObject>& spObject : m_spSceneObjects)
		{
			spObject->Draw2D();
		}
		for (std::shared_ptr<GameObject>& spObject : m_spGameObjects)
		{
			spObject->Draw2D();
		}
	}
	SHADER->m_spriteShader.End();
}

void GameSystem::RequestChangeScene(const std::string& name)
{
	m_isRequestChangeScene = true;

	m_nextSceneName = name;
}

void GameSystem::ChangeScene()
{
	if (m_nextSceneName == "Title")
	{
		TitleInit();
	}
	else if (m_nextSceneName == "Game")
	{
		GameInit();
	}
	else if (m_nextSceneName == "Result")
	{
		ResultInit();
	}
	else if (m_nextSceneName == "Clear")
	{
		ClearInit();
	}
	
	m_isRequestChangeScene = false;
}


const std::shared_ptr<KdCamera> GameSystem::GetCamera() const
{
	return m_spCamera;
}

void GameSystem::Release()
{
	m_audioManager.StopAllSound();

	m_spSceneObjects.clear();
	m_spGameObjects.clear();

	for (std::shared_ptr<GameObject>& spObject : m_spGameObjects)
	{
		spObject->Change2D();
	}

	m_change = false;
}