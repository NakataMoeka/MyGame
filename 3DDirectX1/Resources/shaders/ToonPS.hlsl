#include "Toon.hlsli"

Texture2D<float4> tex : register(t0);	//0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	//float4 texcolor = tex.Sample(smp, input.uv);
	//float4 shadercolor;
	//光沢度
	//const float shininess = 4.0f;
	//float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	//float3 dotlightnormal = dot(lightv, input.normal);
	//float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
	//float3 ambient = m_ambient;
	//float3 diffuse = dotlightnormal * m_diffuse;
	//float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	//shadercolor.rgb = (ambient + diffuse + specular) * lightcolor;
	//shadercolor.a = m_alpha;

	//return shadercolor * texcolor;
	float4 texcolor = tex.Sample(smp, input.uv);
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	float3 halfVec = normalize(lightv+ eyedir);
	float intensity = saturate(dot(normalize(input.normal), halfVec));
	float4 color = float4(m_ambient, 1);
	float4 bright = step(0.1, intensity) * texcolor;
	float4 dark = (1 - step(0.1, intensity)) * color;

	float4 toon = bright + dark;
	return float4(texcolor.rgb * toon.rgb, texcolor.a * m_alpha + toon.a);
}
