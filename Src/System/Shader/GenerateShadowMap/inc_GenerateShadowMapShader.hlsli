struct VSOutput
{
	float4 Pos     : SV_Position;
	float4 ProjPos : TEXCOORD0; // 変換なしの射影座標
};