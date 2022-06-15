#pragma once


//============================================================
//
// ��{�V�F�[�_
//
//============================================================
class TranslucentShader
{
	struct cbObject;
	struct cbMaterial;
	struct cbBone;
public:


	//================================================
	// �ݒ�E�擾
	//================================================

	// ���[���h�s��Z�b�g
	void SetWorldMatrix(const Math::Matrix& m)
	{
		m_cb0.Work().mW = m;
	}

	// UV�^�C�����O�ݒ�
	void SetUVTiling(const Math::Vector2& tiling)
	{
		m_cb0.Work().UVTiling = tiling;
	}
	// UV�I�t�Z�b�g�ݒ�
	void SetUVOffset(const Math::Vector2& offset)
	{
		m_cb0.Work().UVOffset = offset;
	}

	// ���C�g�L��/����
	void SetLightEnable(bool enable)
	{
		m_cb0.Work().LightEnable = enable;
	}

	// �t�H�O�L��/����
	void SetFogEnable(bool enable)
	{
		m_cb0.Work().FogEnable = enable;
	}

	// Set�n�ŃZ�b�g�����f�[�^���A���ۂɒ萔�o�b�t�@�֏�������
	void WriteToCB()
	{
		m_cb0.Write();
		m_cb1_Material.Write();
	}

	// Object�p�萔�o�b�t�@�����̂܂ܕԂ�
	KdConstantBuffer<cbObject>& ObjectCB()
	{
		return m_cb0;
	}

	// material�p�萔�o�b�t�@�����̂܂ܕԂ�
	KdConstantBuffer<cbMaterial>& MaterialCB()
	{
		return m_cb1_Material;
	}

	// bone�p�萔�o�b�t�@�����̂܂ܕԂ�
	KdConstantBuffer<cbBone>& BoneCB()
	{
		return m_cb2_Bones;
	}

	//================================================
	// �`��
	//================================================

	// ���̃V�F�[�_���f�o�C�X�փZ�b�g
	void SetToDevice();

	// ���b�V���`��
	// �Emesh			�c �`�悷�郁�b�V��
	// �Ematerials		�c �g�p����ގ��z��
	void DrawMesh(const KdMesh* mesh, const std::vector<KdMaterial>& materials);

	// ���f���`��
	void DrawModel(const KdModelWork& rModel, const Math::Matrix& mWorld = Math::Matrix::Identity);

	//================================================
	// �������E���
	//================================================

	// ������
	bool Init();
	// ���
	void Release();
	// 
	~TranslucentShader()
	{
		Release();
	}

private:

	// 3D���f���p�V�F�[�_
	ID3D11VertexShader* m_VS = nullptr;				// ���_�V�F�[�_�[
	ID3D11VertexShader* m_VS_Skin = nullptr;		// ���_�V�F�[�_�[(�X�L�����b�V��

	ID3D11InputLayout* m_inputLayout = nullptr;	// ���_���̓��C�A�E�g
	ID3D11InputLayout* m_inputLayout_Skin = nullptr;// ���_���̓��C�A�E�g(�X�L�����b�V��

	ID3D11PixelShader* m_PS = nullptr;				// �s�N�Z���V�F�[�_�[

	// �萔�o�b�t�@
	//  ���萔�o�b�t�@�́A�p�b�L���O�K���Ƃ������̂����炵�Ȃ���΂Ȃ�Ȃ�
	//  <�p�b�L���O�K��> �Q�l�Fhttps://docs.microsoft.com/ja-jp/windows/win32/direct3dhlsl/dx-graphics-hlsl-packing-rules
	//  �E�\���̂̃T�C�Y��16�o�C�g�̔{���ɂ���B
	//  �E�e����(�ϐ�)�́A16�o�C�g���E���܂����悤�ȏꍇ�A���̃x�N�g���ɔz�u�����B

	// �萔�o�b�t�@(�I�u�W�F�N�g�P�ʍX�V)
	struct cbObject
	{
		Math::Matrix		mW;		// ���[���h�s��@�s���16�o�C�gx4�o�C�g��64�o�C�g�Ȃ̂Ńs�b�^���B

		// UV����
		Math::Vector2		UVOffset = { 0,0 };
		Math::Vector2		UVTiling = { 1,1 };

		// ���C�g�L��
		int					LightEnable = 1;
		// �t�H�O�L��
		int					FogEnable = 1;

		float tmp[2];
	};
	KdConstantBuffer<cbObject>	m_cb0;

	// �萔�o�b�t�@(�}�e���A���P�ʍX�V)
	struct cbMaterial {
		Math::Vector4	BaseColor;
		Math::Vector3	Emissive;
		float			Metallic;
		float			Roughness;

		float			tmp[3];
	};
	KdConstantBuffer<cbMaterial>	m_cb1_Material;

public:
	static const int maxBoneBufferSize = 300;
private:
	// �萔�o�b�t�@(�{�[���P�ʍX�V)
	struct cbBone {
		Math::Matrix	mBones[300];
	};
	KdConstantBuffer<cbBone>	m_cb2_Bones;

};



