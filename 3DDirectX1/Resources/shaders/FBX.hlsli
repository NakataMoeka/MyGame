
//ボーンの最大数
static const int MAX_BONES = 32;


cbuffer cbuff0 : register(b0)
{
	//float4 color; // 色(RGBA)
	//matrix mat; // ３Ｄ変換行列
	matrix viewproj;
	matrix world;
	float3 cameraPos;
};

cbuffer skinning: register(b3)
{
	matrix matSkinning[MAX_BONES];
};


//cbuffer cbuff1 : register(b1)
//{
//	float3 m_ambient:packoffset(c0);
//	float3 m_diffuse:packoffset(c1);
//	float3 m_specular:packoffset(c2);
//	float m_alpha : packoffset(c2.w);
//};

struct VSInput
{
	float4 pos : POSITION; // システム用頂点座標
	float3 normal :NORMAL; // 法線ベクトル
	float2 uv  :TEXCOORD; // uv値
	uint4 boneIndices:BONEINDICES;
	float4 boneWeights: BONEWEIGHTS;
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float3 normal :NORMAL; // 法線ベクトル
	float2 uv  :TEXCOORD; // uv値
};
