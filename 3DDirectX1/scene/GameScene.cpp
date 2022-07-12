#include "GameScene.h"
#include <cassert>
#include<sstream>
#include<iomanip>
#include "FbxLoader.h"
#include "FbxObject.h"
#include"input.h"
#include"DebugText.h"
GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	safe_delete(object3d);
	safe_delete(object3d2);
	safe_delete(object3d3);
	safe_delete(object3d4);
	safe_delete(model);
	safe_delete(model2);
	safe_delete(model3);
	safe_delete(model4);
	safe_delete(sprite);
	safe_delete(particleMan);
	safe_delete(lightGroup);
}

void GameScene::Initialize(DXCommon* dxCommon, Audio* audio)
{
	//u
	assert(dxCommon);
	assert(audio);

	this->dxCommon = dxCommon;
	this->audio = audio;

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);
	FbxObject3d::SetDev(dxCommon->Getdev());
	//ライト生成
	lightGroup = LightGroup::Create();

	Object3d::SetLight(lightGroup);

	// 3Dオブエクトにライトをセット
	// 
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,0,1,0 });
	lightGroup->SetDirLightDir(1, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetDirLightDir(2, XMVECTOR{ 1,0,0,0 });
	//lightGroup->SetPointLightActive(0, true);
	//lightGroup->SetSpotLightActive(0, true);
	lightGroup->SetCircleShadowActive(0, true);

	FbxObject3d::SetCamera(camera);
	FbxObject3d::CreateGraphicsPipeline(L"Resources/shaders/FBXPS.hlsl", L"Resources/shaders/FBXVS.hlsl");

	// パーティクルマネージャ生成
	particleMan = ParticleManager::Create(dxCommon->Getdev(), camera);

	model = model->Create("bullet", false);
	model2 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");
	object3d = Object3d::Create(model);
	object3d2 = new FbxObject3d();
	object3d2->Initialize();
	object3d2->SetModel(model2);
	model3 = model3->Create("skydome", true);
	model4 = model4->Create("ground", false);
	object3d3 = Object3d::Create(model3);
	object3d4 = Object3d::Create(model4);

	//Createの後に書かないとclient.hのInternalRelease()でエラーが起こる
	object3d->CreateGraphicsPipeline(L"Resources/shaders/OBJPS.hlsl", L"Resources/shaders/OBJVS.hlsl");
	object3d3->CreateGraphicsPipeline(L"Resources/shaders/OBJPS.hlsl", L"Resources/shaders/OBJVS.hlsl");
	object3d4->CreateGraphicsPipeline(L"Resources/shaders/OBJPS.hlsl", L"Resources/shaders/OBJVS.hlsl");

	object3d2->SetRotation({ 0,45,0 });

	object3d->Update();

	//object3d2->Update();


	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
		return;
	}
	// デバッグテキスト初期化
	DebugText::GetInstance()->Initialize(debugTextTexNumber);

	Sprite::LoadTexture(1, L"Resources/background.png");

	sprite = Sprite::CreateSprite(1, { 0,0 });


	sound1 = Audio::SoundLoadWave("Resources/ショット.wav");
	sound2 = Audio::SoundLoadWave("Resources/World_Heritage.wav");
	//audio->SoundPlayWave(sound1);
	//audio->SoundPlayWave(sound2);
	//audio->SetBGMVolume(0.5f);
	// カメラ注視点をセット
	camera->SetTarget({ 0, 0.0f, 0 });
	camera->SetEye({ 0, 0, -10 });
	player = new Player;//newすればエラー吐かない
	player->Initialize();
	gameObject = new GameObject;//newすればエラー吐かない
	gameObject->Initialize();
}

void GameScene::Init()
{
	player->Init();
	gameObject->Init();
}

void GameScene::Update()
{
	HP += HPRecovery;

	//光線方向初期値                  上奥
	//static XMVECTOR lightDir = { 0, 4, 0, 0 };


	if (Input::GetInstance()->TriggerKey(DIK_SPACE) || Input::GetInstance()->IsButtonDown(ButtonA)) {
		object3d2->PlayAnimation();
	}

	lightGroup->SetCircleShadowDir(0, XMVECTOR({ 0,-1,0,0 }));
	lightGroup->SetCircleShadowCasterPos(0, player->GetPlayerPos());
	lightGroup->SetCircleShadowAtten(0, XMFLOAT3(0.5, 0.6, 0));
	lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(0, 0.5));


	object3d->SetScale({ 0.2, 0.2, 0.2 });
	object3d4->SetPosition({ 0,-1,0 });
	object3d->SetRotation({ a,0,b });
	player->Update();
	camera->FollowCamera(player->GetPlayerPos(), XMFLOAT3{ 0,2,-10 }, 0, player->GetPlayerAngle().y);
	camera->Update();

	gameObject->Update();


		if (Collision::CheckSphere2Box(player->GetSphere(), gameObject->GetCBox())) {
			IsHit = true;
		}
		else {
			IsHit = false;
		}

		if (IsHit == true) {
			gameObject->GetObject()->SetParent(player->GetObject());
			//gameObject->SetSize({0.5,0.5,0.5});
		}
	
	

	
	particleMan->Update();
	object3d2->SetPosition(playerPosition);
	object3d2->SetRotation({ 0,90,0 });
	object3d->Update();
	object3d2->Update();
	object3d3->Update();
	object3d4->Update();
	lightGroup->Update();
}

void GameScene::DrawBG()
{
	//背景
	sprite->PreDraw(dxCommon->GetCmdList());
	sprite->Draw();
	sprite->PostDraw();
	dxCommon->ClearDepthBuffer();
}

void GameScene::Draw()
{
	Object3d::PreDraw(dxCommon->GetCmdList());
	FbxObject3d::PreDraw(dxCommon->GetCmdList());

	object3d3->Draw();
	object3d4->Draw();

	//object3d->Draw();
	//object3d2->Draw();
	player->Draw();
	gameObject->Draw();

	Object3d::PostDraw();
	FbxObject3d::PostDraw();

}
void GameScene::DrawFront()
{
	//前景
	sprite->PreDraw(dxCommon->GetCmdList());
	//sprite->Draw();
	DebugText::GetInstance()->Printf(100, 20, 3.0f, "%f", gameObject->GetSize().x);

		
		DebugText::GetInstance()->DrawAll(dxCommon->GetCmdList());
	sprite->PostDraw();
}
void GameScene::CreateParticles()
{
	for (int i = 0; i < 10; i++) {
		// X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		// 追加
		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
	}
}