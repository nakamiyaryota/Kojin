#include "inc_GenerateShadowMapShader.hlsli"

float4 main(VSOutput In) : SV_TARGET
{
	return float4(In.Pos.z / In.Pos.w, 0.0f, 0.0f, 1.0f);
}