#pragma once


#include"SafeDelete.h"
#include <DirectXMath.h>
#include"input.h"
#include"Sprite.h"
#include"Audio.h"
#include"DXCommon.h"
#include"DebugText.h"
#include"Object3d.h"
#include "ParticleManager.h"
#include"Model.h"
#include "Camera.h"
#include "FbxObject.h"
#include"LightGroup.h"
#include"player.h"
#include"GameObject.h"

class GameScene
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public: // メンバ関数


	GameScene();


	~GameScene();

	void Initialize(DXCommon* dxCommon, Audio* audio);

	void Init();
	void Update();
	void DrawBG();
	void Draw();
	void DrawFront();
	void CreateParticles();
private: // メンバ変数
	DXCommon* dxCommon = nullptr;


	Audio* audio = nullptr;

	SoundData sound1;
	SoundData sound2;

	Camera* camera = nullptr;
	ParticleManager* particleMan = nullptr;
	Sprite* sprite = nullptr;

	Object3d* object3d = nullptr;
	Model* model = nullptr;

	FbxObject3d* object3d2 = nullptr;
	FbxModel* model2 = nullptr;

	Object3d* object3d3 = nullptr;
	Model* model3 = nullptr;

	Object3d* object3d4 = nullptr;
	Model* model4 = nullptr;

	LightGroup* lightGroup = nullptr;

	Player* player;

	GameObject* gameObject;

	const int debugTextTexNumber = 0;

	XMFLOAT3 playerPosition = { 0.0f,0.0f,0.0f };

	int HP = 50;
	const int HPRecovery = 10;
	int pg = 0;
	bool pFlag = false;
	float a = 0;
	float b = 0;
	bool IsHit = false;
	bool parentFlag = false;
};