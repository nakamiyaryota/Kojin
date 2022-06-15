#include "GenerateShadowMapShader.h"

void GenerateShadowMapShader::Begin()
{
	ID3D11DeviceContext* pDevCon = D3D.WorkDevContext();
	if (!pDevCon) { return; }

	if (m_saveRTV || m_saveDSV)
	{
		assert(0 && "���ɋL�^����Ă��郌���_�[�^�[�Q�b�g������܂�");
	}

	// ���O�̃����_�[�^�[�Q�b�g���̕ۑ�(�������邽��)
	pDevCon->OMGetRenderTargets(1, &m_saveRTV, &m_saveDSV);
	UINT num = 1;
	pDevCon->RSGetViewports(&num, &m_saveVP);

	// �V���h�E�}�b�v�p�e�N�X�`���ɐ؂�ւ�
	pDevCon->OMSetRenderTargets(1, m_dirLightShadowMap->WorkRTViewAddress(), m_dirLightZBuffer->WorkDSView());

	// ViewPort�̐؂�ւ�
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

	// �e�N�X�`���̃N���A
	pDevCon->ClearRenderTargetView(m_dirLightShadowMap->WorkRTView(), kWhiteColor);
	pDevCon->ClearDepthStencilView(m_dirLightZBuffer->WorkDSView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	// ���C�g�̃r���[�s��E���ˉe�s��
	Math::Vector3 lightDir = SHADER->m_cb8_Light.Get().DL_Dir;
	Math::Vector3 lightPos = SHADER->m_cb7_Camera.Get().CamPos;
	lightPos.y += 20.0f;
	lightPos.z += 20.0f;

	// XMMatrixLookAtLH(����������������t�s������֐�)
	// ����(�P:�J�n���W �Q:�ړI���W �R:�s��̏����)
	Math::Matrix LightView = DirectX::XMMatrixLookAtLH(lightPos, lightPos + lightDir, Math::Vector3::Up);

	// ���ˉe�s��
	Math::Matrix LightProj = DirectX::XMMatrixOrthographicLH(50, 50, 0, 100);

	SHADER->m_cb8_Light.Work().DL_mViewProj = LightView * LightProj;
	SHADER->m_cb8_Light.Write();

	// �f�o�C�X�R���e�L�X�g�Ƀp�C�v���C�����Z�b�g
	SetToDevice();
}

void GenerateShadowMapShader::End()
{
	if (!m_saveRTV || !m_saveDSV)
	{
		assert(0 && "���A�Ώۂ̃����_�[�^�[�Q�b�g�����݂��܂���");

		return;
	}

	// �؂�ւ��O�̃����_�[�^�[�Q�b�g�e�N�X�`���ւ̕��A
	D3D.WorkDevContext()->OMSetRenderTargets(1, &m_saveRTV, m_saveDSV);
	D3D.WorkDevContext()->RSSetViewports(1, &m_saveVP);

	KdSafeRelease(m_saveRTV);
	KdSafeRelease(m_saveDSV);

	D3D.WorkDevContext()->VSSetConstantBuffers(0, 1, m_cb0.GetAddress());
}

void GenerateShadowMapShader::DrawMesh(const KdMesh* mesh, const std::vector<KdMaterial>& materials)
{
	if (mesh == nullptr)return;

	// ���b�V�������Z�b�g
	mesh->SetToDevice();

	// �S�T�u�Z�b�g
	for (UINT subi = 0; subi < mesh->GetSubsets().size(); subi++)
	{
		// �ʂ��P���������ꍇ�̓X�L�b�v
		if (mesh->GetSubsets()[subi].FaceCount == 0)continue;

		//// �����烁�b�V����`�悷��̂ɕK�v�ȃ}�e���A����T���o���Ă���
		//const KdMaterial& material = materials[mesh->GetSubsets()[subi].MaterialNo];

		////-----------------------
		//// �}�e���A������萔�o�b�t�@�֏�������
		////-----------------------
		//m_cb1_Material.Work().BaseColor = material.BaseColor;
		//m_cb1_Material.Write();

		////-----------------------
		//// �e�N�X�`���Z�b�g
		////-----------------------

		//ID3D11ShaderResourceView* srvs[1] = {};
		//// BaseColor
		//srvs[0] = material.BaseColorTex->WorkSRView();
		//// �Z�b�g
		//D3D.WorkDevContext()->PSSetShaderResources(0, _countof(srvs), srvs);

		//-----------------------
		// �T�u�Z�b�g�`��
		//-----------------------
		mesh->DrawSubset(subi);
	}
}

void GenerateShadowMapShader::DrawModel(const KdModelWork& rModel, const Math::Matrix& mWorld)
{
	// �L������Ȃ��Ƃ��̓X�L�b�v
	if (!rModel.IsEnable()) { return; }

	const std::shared_ptr<KdModelData>& data = rModel.GetData();

	// ���f�����Ȃ��Ƃ��̓X�L�b�v
	if (data == nullptr) { return; }

	// �S���b�V���m�[�h��`��
	for (auto& nodeIdx : data->GetMeshNodeIndices())
	{
		auto& rWorkNode = rModel.GetNodes()[nodeIdx];

		const std::shared_ptr<KdMesh>& spMesh = rModel.GetMesh(nodeIdx);

		// �s��Z�b�g
		m_cb0.Work().mWorld = rWorkNode.m_worldTransform * mWorld;
		m_cb0.Write();

		// �`��
		DrawMesh(spMesh.get(), data->GetMaterials());
	}
}

void GenerateShadowMapShader::SetToDevice()
{
	ID3D11DeviceContext* DevCon = D3D.WorkDevContext();
	if (!DevCon) { return; }

	// ���_�V�F�[�_�[���Z�b�g
	DevCon->VSSetShader(m_VS, 0, 0);

	// ���_�f�[�^�̃��C�A�E�g���Z�b�g
	DevCon->IASetInputLayout(m_inputLayout);

	// �s�N�Z���V�F�[�_�[���Z�b�g
	DevCon->PSSetShader(m_PS, 0, 0);

	DevCon->VSSetConstantBuffers(0, 1, m_cb0.GetAddress());
}

bool GenerateShadowMapShader::Init()
{
	//-------------------------------------
	// ���_�V�F�[�_
	//-------------------------------------
	{
		// �R���p�C���ς݂̃V�F�[�_�[�w�b�_�[�t�@�C�����C���N���[�h
#include "GenerateShadowMap_VS.inc"

// ���_�V�F�[�_�[�쐬
		if (FAILED(D3D.WorkDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_VS)))
		{
			assert(0 && "���_�V�F�[�_�[�쐬���s");
			Release();
			return false;
		}

		// �P���_�̏ڍׂȏ��
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		// ���_���̓��C�A�E�g�쐬
		if (FAILED(D3D.WorkDev()->CreateInputLayout(
			&layout[0],			// ���̓G�������g�擪�A�h���X
			layout.size(),		// ���̓G�������g��
			&compiledBuffer[0],				// ���_�o�b�t�@�̃o�C�i���f�[�^
			sizeof(compiledBuffer),			// ��L�̃o�b�t�@�T�C�Y
			&m_inputLayout))					// 
			)
		{
			assert(0 && "CreateInputLayout���s");
			Release();
			return false;
		}
	}

	//-------------------------------------
	// �s�N�Z���V�F�[�_
	//-------------------------------------
	{
		// �R���p�C���ς݂̃V�F�[�_�[�w�b�_�[�t�@�C�����C���N���[�h
#include "GenerateShadowMap_PS.inc"

// �s�N�Z���V�F�[�_�[�쐬
		if (FAILED(D3D.WorkDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_PS))) {
			assert(0 && "�s�N�Z���V�F�[�_�[�쐬���s");
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
