#pragma once
#include"Framework.h"
#include "GameScene.h"
class GameScene;
class NMGame :public Framework
{
public:
	// ‰Šú‰»
	void Initialize() override;
	//I—¹
	void Finalize() override;
	// –ˆƒtƒŒ[ƒ€ˆ—
	void Update() override;
	// •`‰æ
	void Draw() override;
private:
	GameScene* gameScene = nullptr;

};

