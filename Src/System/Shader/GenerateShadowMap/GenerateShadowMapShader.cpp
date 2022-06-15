#include "GenerateShadowMapShader.h"

void GenerateShadowMapShader::Begin()
{
	ID3D11DeviceContext* pDevCon = D3D.WorkDevContext();
	if (!pDevCon) { return; }

	if (m_saveRTV || m_saveDSV)
	{
		assert(0 && "既に記録されているレンダーターゲットがあります");
	}

	// 直前のレンダーターゲット情報の保存(復元するため)
	pDevCon->OMGetRenderTargets(1, &m_saveRTV, &m_saveDSV);
	UINT num = 1;
	pDevCon->RSGetViewports(&num, &m_saveVP);

	// シャドウマップ用テクスチャに切り替え
	pDevCon->OMSetRenderTargets(1, m_dirLightShadowMap->WorkRTViewAddress(), m_dirLightZBuffer->WorkDSView());

	// ViewPortの切り替え
	D3D11_VIEWPORT vp =
	{
		0.0f,
		0.0f,
		m_dirLightShadowMap->GetWidth(),
		m_dirLightShadowMap->GetHeight(),
		0.0f,
		1.0f
	};
	pDevCon->RSSetViewports(1, &vp);

	// テクスチャのクリア
	pDevCon->ClearRenderTargetView(m_dirLightShadowMap->WorkRTView(), kWhiteColor);
	pDevCon->ClearDepthStencilView(m_dirLightZBuffer->WorkDSView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	// ライトのビュー行列・正射影行列
	Math::Vector3 lightDir = SHADER->m_cb8_Light.Get().DL_Dir;
	Math::Vector3 lightPos = SHADER->m_cb7_Camera.Get().CamPos;
	lightPos.y += 20.0f;
	lightPos.z += 20.0f;

	// XMMatrixLookAtLH(ある方向を向いた逆行列を作る関数)
	// 引数(１:開始座標 ２:目的座標 ３:行列の上方向)
	Math::Matrix LightView = DirectX::XMMatrixLookAtLH(lightPos, lightPos + lightDir, Math::Vector3::Up);

	// 正射影行列
	Math::Matrix LightProj = DirectX::XMMatrixOrthographicLH(50, 50, 0, 100);

	SHADER->m_cb8_Light.Work().DL_mViewProj = LightView * LightProj;
	SHADER->m_cb8_Light.Write();

	// デバイスコンテキストにパイプラインをセット
	SetToDevice();
}

void GenerateShadowMapShader::End()
{
	if (!m_saveRTV || !m_saveDSV)
	{
		assert(0 && "復帰対象のレンダーターゲットが存在しません");

		return;
	}

	// 切り替え前のレンダーターゲットテクスチャへの復帰
	D3D.WorkDevContext()->OMSetRenderTargets(1, &m_saveRTV, m_saveDSV);
	D3D.WorkDevContext()->RSSetViewports(1, &m_saveVP);

	KdSafeRelease(m_saveRTV);
	KdSafeRelease(m_saveDSV);

	D3D.WorkDevContext()->VSSetConstantBuffers(0, 1, m_cb0.GetAddress());
}

void GenerateShadowMapShader::DrawMesh(const KdMesh* mesh, const std::vector<KdMaterial>& materials)
{
	if (mesh == nullptr)return;

	// メッシュ情報をセット
	mesh->SetToDevice();

	// 全サブセット
	for (UINT subi = 0; subi < mesh->GetSubsets().size(); subi++)
	{
		// 面が１枚も無い場合はスキップ
		if (mesh->GetSubsets()[subi].FaceCount == 0)continue;

		//// 今からメッシュを描画するのに必要なマテリアルを探し出している
		//const KdMaterial& material = materials[mesh->GetSubsets()[subi].MaterialNo];

		////-----------------------
		//// マテリアル情報を定数バッファへ書き込む
		////-----------------------
		//m_cb1_Material.Work().BaseColor = material.BaseColor;
		//m_cb1_Material.Write();

		////-----------------------
		//// テクスチャセット
		////-----------------------

		//ID3D11ShaderResourceView* srvs[1] = {};
		//// BaseColor
		//srvs[0] = material.BaseColorTex->WorkSRView();
		//// セット
		//D3D.WorkDevContext()->PSSetShaderResources(0, _countof(srvs), srvs);

		//-----------------------
		// サブセット描画
		//-----------------------
		mesh->DrawSubset(subi);
	}
}

void GenerateShadowMapShader::DrawModel(const KdModelWork& rModel, const Math::Matrix& mWorld)
{
	// 有効じゃないときはスキップ
	if (!rModel.IsEnable()) { return; }

	const std::shared_ptr<KdModelData>& data = rModel.GetData();

	// モデルがないときはスキップ
	if (data == nullptr) { return; }

	// 全メッシュノードを描画
	for (auto& nodeIdx : data->GetMeshNodeIndices())
	{
		auto& rWorkNode = rModel.GetNodes()[nodeIdx];

		const std::shared_ptr<KdMesh>& spMesh = rModel.GetMesh(nodeIdx);

		// 行列セット
		m_cb0.Work().mWorld = rWorkNode.m_worldTransform * mWorld;
		m_cb0.Write();

		// 描画
		DrawMesh(spMesh.get(), data->GetMaterials());
	}
}

void GenerateShadowMapShader::SetToDevice()
{
	ID3D11DeviceContext* DevCon = D3D.WorkDevContext();
	if (!DevCon) { return; }

	// 頂点シェーダーをセット
	DevCon->VSSetShader(m_VS, 0, 0);

	// 頂点データのレイアウトをセット
	DevCon->IASetInputLayout(m_inputLayout);

	// ピクセルシェーダーをセット
	DevCon->PSSetShader(m_PS, 0, 0);

	DevCon->VSSetConstantBuffers(0, 1, m_cb0.GetAddress());
}

bool GenerateShadowMapShader::Init()
{
	//-------------------------------------
	// 頂点シェーダ
	//-------------------------------------
	{
		// コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "GenerateShadowMap_VS.inc"

// 頂点シェーダー作成
		if (FAILED(D3D.WorkDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_VS)))
		{
			assert(0 && "頂点シェーダー作成失敗");
			Release();
			return false;
		}

		// １頂点の詳細な情報
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		// 頂点入力レイアウト作成
		if (FAILED(D3D.WorkDev()->CreateInputLayout(
			&layout[0],			// 入力エレメント先頭アドレス
			layout.size(),		// 入力エレメント数
			&compiledBuffer[0],				// 頂点バッファのバイナリデータ
			sizeof(compiledBuffer),			// 上記のバッファサイズ
			&m_inputLayout))					// 
			)
		{
			assert(0 && "CreateInputLayout失敗");
			Release();
			return false;
		}
	}

	//-------------------------------------
	// ピクセルシェーダ
	//-------------------------------------
	{
		// コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "GenerateShadowMap_PS.inc"

// ピクセルシェーダー作成
		if (FAILED(D3D.WorkDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_PS))) {
			assert(0 && "ピクセルシェーダー作成失敗");
			Release();
			return false;
		}
	}

	m_cb0.Create();

	m_dirLightShadowMap = std::make_shared<KdTexture>();
	m_dirLightZBuffer = std::make_shared<KdTexture>();

	m_dirLightShadowMap->CreateRenderTarget(1024, 1024, DXGI_FORMAT_R32_FLOAT);
	m_dirLightZBuffer->CreateDepthStencil(1024, 1024, DXGI_FORMAT_R32_TYPELESS);

	return true;
}

void GenerateShadowMapShader::Release()
{
	KdSafeRelease(m_VS);
	KdSafeRelease(m_inputLayout);
	KdSafeRelease(m_PS);

	m_cb0.Release();

	m_dirLightShadowMap = nullptr;
	m_dirLightZBuffer = nullptr;
}
