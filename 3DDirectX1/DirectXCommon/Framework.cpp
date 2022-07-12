#include "Framework.h"

void Framework::Run()
{
	Initialize();
	while (true)
	{
		if (endRequst == true) { break; }
		Update();
		Draw();

	}
	Finalize();
}

void Framework::Initialize()
{
	//ウィンドウの生成
	winapp = new WinApp();
	winapp->CreateGameWindow();
	//DirectXの初期化
	dxcommon = new DXCommon();
	dxcommon->Initialize(winapp);

	//汎用機能の初期化
	audio = new Audio();
	if (!audio->Initialize()) {
		assert(0);
		return;
	}

	if (!Sprite::StaticInitialize(dxcommon->Getdev(), WinApp::window_width, WinApp::window_height)) {
		assert(0);
		return;
	}

	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxcommon->Getdev());
	//ライト静的初期化
	LightGroup::StaticInitialize(dxcommon->Getdev());
	FbxLoader::GetInstance()->Initialize(dxcommon->Getdev());

	input = Input::GetInstance();
	input->Initialize(winapp);
	//ポストエフェクト用のテクスチャ読み込み
	//Sprite::LoadTexture(100, L"Resources/white1x1.png");
	//ポストエフェクトの初期化
	postEffect = new PostEffect();
	postEffect->Initialize();
	postEffect->CreateGraphicsPipeline(L"Resources/shaders/PostEffectTestPS.hlsl", L"Resources/shaders/PostEffectTestVS.hlsl");
}

void Framework::Finalize()
{
	//ウィンドウクラスを登録解除
	winapp->TerminateGameWindow();


	delete audio;
	delete dxcommon;
	delete winapp;
	delete postEffect;
}

void Framework::Update()
{
	if (winapp->ProcessMessage())
	{
		endRequst = true;
		return;
	}
	//入力関連の毎フレーム処理
	input->update();


	}
