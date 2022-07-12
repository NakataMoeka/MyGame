#include "Blur.hlsli"

Texture2D<float4> tex : register(t0);	//0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	//ぼかし
	float oU = 2.5 / 1280;
	float oV = 2.5 / 720;
	float4 texcolor = tex.Sample(smp, input.uv);
	
	texcolor += tex.Sample(smp, input.uv + float2(oU, 0.0f));
	texcolor += tex.Sample(smp, input.uv + float2(-oU, 0.0f));
	texcolor += tex.Sample(smp, input.uv + float2(0.0f, oV));
	texcolor += tex.Sample(smp, input.uv + float2(0.0f, -oV));
	texcolor += tex.Sample(smp, input.uv + float2(0.0f, 0.0f));
	texcolor += tex.Sample(smp, input.uv + float2(oU, oV));
	texcolor += tex.Sample(smp, input.uv + float2(oU, -oV));
	texcolor += tex.Sample(smp, input.uv + float2(-oU, oV));
	texcolor += tex.Sample(smp, input.uv + float2(-oU, -oV));
	
	texcolor /= 9.0f;
	//ここまで
	
	return texcolor;
}
