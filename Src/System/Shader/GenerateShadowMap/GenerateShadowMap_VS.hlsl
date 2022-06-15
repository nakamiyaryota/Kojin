#include "../inc_KdCommon.hlsli"

#include "inc_GenerateShadowMapShader.hlsli"

cbuffer cbObject : register(b0)
{
	row_major float4x4 g_mWorld;
}

VSOutput main(float4 pos : POSITION)
{
	VSOutput Out;

	Out.Pos = pos;
	// ���W�ϊ�
	Out.Pos = mul(pos, g_mWorld);           // �Q�[����3D��Ԃ̍��W�ɒu��
	Out.Pos = mul(Out.Pos, g_DL_mViewProj);	// ���s�����猩�����_�̍��W�ɒu�� ���s���̐��ˉe���W�n(2D�����`�̉摜)�ϊ�

	Out.ProjPos = Out.Pos;

	// �o��
	return Out;
}