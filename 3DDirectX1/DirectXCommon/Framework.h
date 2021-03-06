#pragma once
#include"WinApp.h"
#include"Audio.h"
#include"DXCommon.h"
#include "input.h"
#include "Object3d.h"
#include "FbxLoader.h"
#include "PostEffect.h"
#include"LightGroup.h"
class Framework
{
public:
	//初期化
	void Run();
	virtual void Initialize();
	//終了
	virtual void Finalize();
	// 毎フレーム処理
	virtual void Update();
	// 描画
	virtual void Draw() = 0;
	bool eR() { return endRequst; }
protected:
	bool endRequst = false;
	WinApp* winapp = nullptr;
	DXCommon* dxcommon = nullptr;
	Audio* audio = nullptr;
	Input* input = nullptr;

	PostEffect* postEffect = nullptr;
};

