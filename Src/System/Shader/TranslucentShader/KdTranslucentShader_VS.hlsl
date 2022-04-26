#include "../inc_KdCommon.hlsli"
#include "inc_KdTranslucentShader.hlsli"

//================================
// ���_�V�F�[�_
//================================
VSOutput main(
	float4 pos : POSITION,	// ���_���W
	float2 uv : TEXCOORD0,	// �e�N�X�`��UV���W
	float3 normal : NORMAL,	// �@���x�N�g��
	float4 color : COLOR	// ���_�F
) {
	VSOutput Out;

	// �L�����N�^�[�̍��W�ϊ� : ���[�J�����W�n -> ���[���h���W�n�֕ϊ�
	Out.Pos = mul(pos, g_mWorld);
	Out.wPos = Out.Pos.xyz;

	// �J�����̋t�����ɕϊ� : ���[���h���W�n -> �r���[���W�n�֕ϊ�
	Out.Pos = mul(Out.Pos, g_mView);

	// �ˉe��Ԃɕϊ� : �r���[���W�n -> �ˉe���W�n�֕ϊ�
	Out.Pos = mul(Out.Pos, g_mProj);

	// �@��
	Out.wN = normalize(mul(normal, (float3x3)g_mWorld));

	// ���_�F
	Out.Color = color;

	// UV���W
	Out.UV = uv * g_UVTiling + g_UVOffset;

	// �o��
	return Out;
}