#include "System/KdSystem.h"

#include "TranslucentShader.h"

void TranslucentShader::SetToDevice()
{
	D3D.WorkDevContext()->PSSetShader(m_PS, 0, 0);

	// �萔�o�b�t�@���Z�b�g
	D3D.WorkDevContext()->VSSetConstantBuffers(0, 1, m_cb0.GetAddress());
	D3D.WorkDevContext()->PSSetConstantBuffers(0, 1, m_cb0.GetAddress());

	D3D.WorkDevContext()->VSSetConstantBuffers(1, 1, m_cb1_Material.GetAddress());
	D3D.WorkDevContext()->PSSetConstantBuffers(1, 1, m_cb1_Material.GetAddress());

	D3D.WorkDevContext()->VSSetConstantBuffers(2, 1, m_cb2_Bones.GetAddress());
	D3D.WorkDevContext()->PSSetConstantBuffers(2, 1, m_cb2_Bones.GetAddress());
}

void TranslucentShader::DrawMesh(const KdMesh* mesh, const std::vector<KdMaterial>& materials)
{
	if (mesh == nullptr)return;

	// StaticMesh��SkinMesh�ɂ���Ē��_�V�F�[�_�[�Ɠ��̓��C�A�E�g��؂�ւ���
	if (mesh->IsSkinMesh())
	{
		// ���_�V�F�[�_���Z�b�g
		D3D.WorkDevContext()->VSSetShader(m_VS_Skin, 0, 0);
		// ���_���C�A�E�g���Z�b�g
		D3D.WorkDevContext()->IASetInputLayout(m_inputLayout_Skin);

		m_cb2_Bones.Write();
	}
	else
	{
		// ���_�V�F�[�_���Z�b�g
		D3D.WorkDevContext()->VSSetShader(m_VS, 0, 0);
		// ���_���C�A�E�g���Z�b�g
		D3D.WorkDevContext()->IASetInputLayout(m_inputLayout);
	}

	// �萔�o�b�t�@��������
	m_cb0.Write();

	// ���b�V�������Z�b�g
	mesh->SetToDevice();

	// �S�T�u�Z�b�g
	for (UINT subi = 0; subi < mesh->GetSubsets().size(); subi++)
	{
		// �ʂ��P���������ꍇ�̓X�L�b�v
		if (mesh->GetSubsets()[subi].FaceCount == 0)continue;

		// �}�e���A���Z�b�g
		const KdMaterial& material = materials[mesh->GetSubsets()[subi].MaterialNo];

		//-----------------------
		// �}�e���A������萔�o�b�t�@�֏�������
		//-----------------------
		m_cb1_Material.Work().BaseColor = material.BaseColor;
		m_cb1_Material.Work().Emissive = material.Emissive;
		m_cb1_Material.Work().Metallic = material.Metallic;
		m_cb1_Material.Work().Roughness = material.Roughness;
		m_cb1_Material.Write();

		//-----------------------
		// �e�N�X�`���Z�b�g
		//-----------------------
		ID3D11ShaderResourceView* srvs[3] = {};

		// BaseColor
		srvs[0] = material.BaseColorTex->WorkSRView();
		// Emissive
		srvs[1] = material.EmissiveTex->WorkSRView();
		// Metallic Roughness
		srvs[2] = material.MetallicRoughnessTex->WorkSRView();

		// �Z�b�g
		D3D.WorkDevContext()->PSSetShaderResources(0, _countof(srvs), srvs);

		//-----------------------
		// �T�u�Z�b�g�`��
		//-----------------------
		mesh->DrawSubset(subi);
	}
}

void TranslucentShader::DrawModel(const KdModelWork& rModel, const Math::Matrix& mWorld)
{
	// �L������Ȃ��Ƃ��̓X�L�b�v
	if (!rModel.IsEnable()) { return; }

	const std::shared_ptr<KdModelData>& data = rModel.GetData();

	auto& workNodes = rModel.GetNodes();

	// ���f�����Ȃ��Ƃ��̓X�L�b�v
	if (data == nullptr) { return; }

	// �X�L�����b�V�����f���̏ꍇ�F�{�[��������������
	if (data->IsSkinMesh())
	{
		// �m�[�h������{�[�������擾
		auto& dataNodes = data->GetOriginalNodes();

		for (auto&& nodeIdx : data->GetBoneNodeIndices())
		{
			if (nodeIdx >= KdStandardShader::maxBoneBufferSize) { assert(0 && "�]���ł���{�[���̏�����𒴂��܂���"); return; }

			auto& dataNode = dataNodes[nodeIdx];
			auto& workNode = workNodes[nodeIdx];

			// �{�[����񂩂�GPU�ɓn���s��̌v�Z
			SHADER->m_standardShader.BoneCB().Work().mBones[dataNode.m_boneIndex] = dataNode.m_boneInverseWorldMatrix * workNode.m_worldTransform;
		}
	}

	// �S���b�V���m�[�h��`��
	for (auto& nodeIdx : data->GetMeshNodeIndices())
	{
		auto& rWorkNode = workNodes[nodeIdx];

		const std::shared_ptr<KdMesh>& spMesh = rModel.GetMesh(nodeIdx);

		// �s��Z�b�g
		SetWorldMatrix(rWorkNode.m_worldTransform * mWorld);

		// �`��
		DrawMesh(spMesh.get(), data->GetMaterials());
	}
}

bool TranslucentShader::Init()
{
	//-------------------------------------
	// ���_�V�F�[�_
	//-------------------------------------
	{
		// �R���p�C���ς݂̃V�F�[�_�[�w�b�_�[�t�@�C�����C���N���[�h
		#include "TranslucentShader_VS.inc"

		// ���_�V�F�[�_�[�쐬
		if (FAILED(D3D.WorkDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_VS))) {
			assert(0 && "���_�V�F�[�_�[�쐬���s");
			Release();
			return false;
		}

		// �P���_�̏ڍׂȏ��
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// ���_���̓��C�A�E�g�쐬
		if (FAILED(D3D.WorkDev()->CreateInputLayout(
			&layout[0],				// ���̓G�������g�擪�A�h���X
			layout.size(),			// ���̓G�������g��
			&compiledBuffer[0],		// ���_�o�b�t�@�̃o�C�i���f�[�^
			sizeof(compiledBuffer),	// ��L�̃o�b�t�@�T�C�Y
			&m_inputLayout))
			) {
			assert(0 && "CreateInputLayout���s");
			Release();
			return false;
		}
	}
	//-------------------------------------
	// ���_�V�F�[�_
	//-------------------------------------
	{
		// �R���p�C���ς݂̃V�F�[�_�[�w�b�_�[�t�@�C�����C���N���[�h
		#include "TranslucentShader_VS_Skin.inc"

		// ���_�V�F�[�_�[�쐬
		if (FAILED(D3D.WorkDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_VS_Skin))) {
			assert(0 && "���_�V�F�[�_�[�쐬���s");
			Release();
			return false;
		}

		// �P���_�̏ڍׂȏ��
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "SKININDEX",	0, DXGI_FORMAT_R16G16B16A16_SINT,	0, 36,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "SKINWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 44,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// ���_���̓��C�A�E�g�쐬
		if (FAILED(D3D.WorkDev()->CreateInputLayout(
			&layout[0],				// ���̓G�������g�擪�A�h���X
			layout.size(),			// ���̓G�������g��
			&compiledBuffer[0],		// ���_�o�b�t�@�̃o�C�i���f�[�^
			sizeof(compiledBuffer),	// ��L�̃o�b�t�@�T�C�Y
			&m_inputLayout_Skin))
			) {
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
		#include "TranslucentShader_PS.inc"

		if (FAILED(D3D.WorkDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_PS))) {
			assert(0 && "�s�N�Z���V�F�[�_�[�쐬���s");
			Release();
			return false;
		}
	}

	//-------------------------------------
	// �萔�o�b�t�@�쐬
	//-------------------------------------
	m_cb0.Create();
	m_cb1_Material.Create();
	m_cb2_Bones.Create();

	return true;
}

void TranslucentShader::Release()
{
	KdSafeRelease(m_VS);
	KdSafeRelease(m_VS_Skin);
	KdSafeRelease(m_inputLayout);
	KdSafeRelease(m_inputLayout_Skin);
	KdSafeRelease(m_PS);

	m_cb0.Release();
	m_cb1_Material.Release();
	m_cb2_Bones.Release();
}
