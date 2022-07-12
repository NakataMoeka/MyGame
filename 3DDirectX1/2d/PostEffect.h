#pragma once
#include "Sprite.h"
#include <DirectXMath.h>
class PostEffect : public Sprite
{
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:


	PostEffect();


	void Initialize();
	void Tex();
	void DescHeap();
	void CreateGraphicsPipeline(const wchar_t* ps,const wchar_t* vs);


	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);


	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

	void Draw(ID3D12GraphicsCommandList* cmdList);
	bool GetBlur() { return Blur; }
	bool SetBlur(bool Blur) { return this->Blur=Blur; }
private:
	//テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff[2];
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//深度バッファ
	ComPtr<ID3D12Resource> depthBuff;
	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;

	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;
	//パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelineState;

	//画面クリアカラー
	static const float clearColor[4];

	bool Blur = false;

};
