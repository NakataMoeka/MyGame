#pragma once
#include"Sprite.h"
/// <summary>
/// スプライトアニメーションで初期化が汚くなるのを避けるために作ったクラス
/// </summary>
class SpriteAnimation
{
public:
	void Animation();
	void Draw();
	Sprite sprite[12];
};

