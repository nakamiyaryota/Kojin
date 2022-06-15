#include "../inc_KdCommon.hlsli"
#include "inc_TranslucentShader.hlsli"

// �e�N�X�`��
Texture2D g_baseTex : register(t0);		// �x�[�X�J���[�e�N�X�`��
Texture2D g_emissiveTex : register(t1);	// �G�~�b�V�u�e�N�X�`��
Texture2D g_mrTex : register(t2);		// ���^���b�N/���t�l�X�e�N�X�`��

// �T���v��
SamplerState g_ss : register(s0);

// BlinnPhong NDF
// �ElightDir    �c ���C�g�̕���
// �EvCam        �c �s�N�Z������J�����ւ̕���
// �Enormal      �c �@��
// �EspecPower   �c ���˂̉s��
float BlinnPhong(float3 lightDir, float3 vCam, float3 normal, float specPower)
{
	float3 H = normalize(-lightDir + vCam);
	// �J�����̊p�x��(0�`1)
	float NdotH = saturate(dot(normal, H));
	float spec = pow(NdotH, specPower);

	// ���K��Blinn-Phong
	return spec * ((specPower + 2) / (2 * 3.1415926535));
}

//================================
// �s�N�Z���V�F�[�_
//================================
float4 main(VSOutput In) : SV_Target0
{
	// �J�����ւ̕���
	float3 vCam = g_CamPos - In.wPos;
	float camDist = length(vCam);       // �J���� - �s�N�Z������
	vCam = normalize(vCam);

	// �@�����K��
	float3 wN = normalize(In.wN);

	//------------------------------------------
	// �ގ��F
	//------------------------------------------

	float4 mr = g_mrTex.Sample(g_ss, In.UV);
	// ������
	float metallic = mr.b * g_Material.Metallic;
	// �e��
	float roughness = mr.g * g_Material.Roughness;

	// �ގ��̐F
	float4 baseColor = g_baseTex.Sample(g_ss, In.UV) * g_Material.BaseColor * In.Color;

	//------------------------------------------
	//
	// ���C�e�B���O
	//
	//------------------------------------------
	// �ŏI�I�ȐF
	float3 color = 0;

	// ���C�g�L����
	if (g_LightEnable)
	{
		// �ގ��̊g�U�F�@������قǍގ��̐F�ɂȂ�A�����قǊg�U�F�͖����Ȃ�
		const float3 baseDiffuse = lerp(baseColor.rgb, float3(0, 0, 0), metallic);
		// �ގ��̔��ːF�@������قǌ��̐F�����̂܂ܔ��˂��A�����قǍގ��̐F�����
		const float3 baseSpecular = lerp(0.04, baseColor.rgb, metallic);

		//------------------
		// ���s��
		//------------------

		// Diffuse(�g�U��) ���K��Lambert���g�p
		{
			// ���̕����Ɩ@���̕����Ƃ̊p�x�������̋����ɂȂ�
			float lightDiffuse = dot(-g_DL_Dir, wN);
			lightDiffuse = saturate(lightDiffuse);  // �}�C�i�X�l��0�ɂ���@0(��)�`1(��)�ɂȂ�

			// ���K��Lambert
			lightDiffuse /= 3.1415926535;

			// ���̐F * �ގ��̊g�U�F * ������
			color += (g_DL_Color * lightDiffuse) * baseDiffuse * baseColor.a;
		}

		//// Specular(���ːF) ���K��Blinn-Phong NDF���g�p
		//{
		//	// ���˂������̋��������߂�

		//	// ���t�l�X����ABlinn-Phong�p��SpecularPower�����߂�
		//	float smoothness = 1.0 - roughness;         // ���t�l�X���t�]�����u���炩�v���ɂ���
		//	float specPower = pow(2, 13 * smoothness);  // 1�`8192

		//	// Blinn-Phong NDF
		//	float spec = BlinnPhong(g_DL_Dir, vCam, wN, specPower);

		//	// ���̐F * ���ˌ��̋��� * �ގ��̔��ːF * ���K���W�� * ������
		//	color += (g_DL_Color * spec) * baseSpecular * baseColor.a;
		//}

		//------------------
		// ����
		//------------------
		color += g_AmbientLight * baseColor.rgb * baseColor.a;

		//------------------
		// �G�~�b�V�u
		//------------------
		color += g_emissiveTex.Sample(g_ss, In.UV).rgb * g_Material.Emissive;
	}
	// ���C�g������
	else
	{
		// �ގ��̐F�����̂܂܎g�p
		color = baseColor.rgb;
	}

	//------------------------------------------
	// �����t�H�O
	//------------------------------------------
	if (g_DistanceFogEnable && g_FogEnable)
	{
		// �t�H�O 1(�߂�)�`0(����)
		float f = saturate(1.0 / exp(camDist * g_DistanceFogDensity));
		// �K�p
		color.rgb = lerp(g_DistanceFogColor, color.rgb, f);
	}

	//------------------------------------------
	// �o��
	//------------------------------------------
	return float4(color, 0.2f);
}