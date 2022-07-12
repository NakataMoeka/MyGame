#pragma once

#include "fbxsdk.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include "FbxModel.h"

class FbxLoader
{
private://エイリアス
	//std::を省略
	using string = std::string;
public:
	static const string defaultTextureFileName;


	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dev">D3D12デバイス</param>
	void Initialize(ID3D12Device* dev);
	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();
	/// <summary>
	/// ファイルからFBXモデル読み込み
	/// </summary>
	/// <param name="modelName">モデル名</param>
	FbxModel* LoadModelFromFile(const string& modelName);

	void ParseNodeRecursive(FbxModel* fbxModel, FbxNode* fbxNode, Node* parent = nullptr);

	void ParseMesh(FbxModel* fbxModel, FbxNode* fbxNode);
	//頂点座標読み取り
	void ParseMeshVertices(FbxModel* fbxModel, FbxMesh* fbxMesh);
	//面情報読み込み
	void ParseMeshFaces(FbxModel* fbxModel, FbxMesh* fbxMesh);
	//マテリアル読み込み
	void ParseMaterial(FbxModel* fbxModel, FbxNode* fbxNode);
	//テクスチャ読み込み
	void LoadTexture(FbxModel* fbxModel, const std::string& fullpath);

	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

	void ParseSkin(FbxModel* fbxModel, FbxMesh* fbxMesh);

public://定数

	static const string baseDirectory;
private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;

	ID3D12Device* dev = nullptr;
	//fbxマネージャ
	FbxManager* fbxManager = nullptr;
	//fbxインポータ
	FbxImporter* fbxImporter = nullptr;
	// ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(const std::string& path);

};