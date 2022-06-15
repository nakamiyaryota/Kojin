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
	// 座標変換
	Out.Pos = mul(pos, g_mWorld);           // ゲームの3D空間の座標に置換
	Out.Pos = mul(Out.Pos, g_DL_mViewProj);	// 平行光から見た頂点の座標に置換 平行光の正射影座標系(2D正方形の画像)変換

	Out.ProjPos = Out.Pos;

	// 出力
	return Out;
}