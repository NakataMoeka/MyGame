#pragma once
#include "Sprite.h"
#include <Windows.h>
#include <string>
class DebugText
{
public: // 定数の宣言    
	static const int maxCharCount = 256;    // 最大文字数
	static const int fontWidth = 9;         // フォント画像内1文字分の横幅
	static const int fontHeight = 18;       // フォント画像内1文字分の縦幅
	static const int fontLineCount = 14;    // フォント画像内1行分の文字数
	static const int bufferSize = 256;	// 書式付き文字列展開用バッファサイズ

public: // メンバ関数

	static DebugText* GetInstance();

	void Initialize(UINT texnumber);
	void Print(int len, const std::string& text, float x, float y, float scale);
	//void NPrint(int len, const char* text);
	void Printf(float x, float y, float scale, const char* fmt, ...);
	void DrawAll(ID3D12GraphicsCommandList* cmdList);

private: // メンバ変数     
	DebugText() = default;
	DebugText(const DebugText&) = delete;
	~DebugText() = default;
	DebugText& operator=(const DebugText&) = delete;
	// スプライトデータの配列
	Sprite* sprite[maxCharCount] = {};
	// スプライトデータ配列の添え字番号
	int spriteIndex = 0;

	char buffer[bufferSize];


	float posX = 0.0f;
	float posY = 0.0f;
	float size = 1.0f;
};