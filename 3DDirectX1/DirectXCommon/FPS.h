#pragma once
#pragma once
#include <windows.h>
#include <tchar.h>
#include <sstream>

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
class FPS final
{
public:
	//初回実行
	void Start();
	//FPS処理の実行
	void Ran();
	//インスタンスの取得
	static FPS* GetInstance();

	//コピーコンストラクタ、代入演算子無効化
	FPS(const FPS& obj) = delete;
	FPS& operator=(const FPS& obj) = delete;
private:
	FPS() {};
	~FPS() {};

	const float MIN_FREAM_TIME = 1.0 / 60.0;
	float frameTime = 0;
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timeFreq;
	
	float fps = 0;
};

