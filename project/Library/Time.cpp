#include "time.h"
#include <Windows.h>
#include <DxLib.h>

// 平均値を計測するか
#define USE_TIME_BUF 1

namespace 
{
	LARGE_INTEGER freq;
	LARGE_INTEGER current;
	float deltaTime;
	float refreshRate;
	float timeScale;

#ifdef USE_TIME_BUF
	// 直近の処理時間を保存
	const int BUF_SIZE = 30;
	float timeBuf[BUF_SIZE];
	int readP;
	int writeP;
#endif
};

void Time::Init()
{
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&current);

	HDC hdc = GetDC(GetMainWindowHandle());	// デバイスコンテキストの取得
	refreshRate = (float)GetDeviceCaps(hdc, VREFRESH);	// リフレッシュレートの取得
	ReleaseDC(GetMainWindowHandle(), hdc);	// デバイスコンテキストの解放

#ifdef USE_TIME_BUF
	readP = 0;
	writeP = 0;
	for (int i = 0; i < BUF_SIZE; i++) {
		timeBuf[i] = 1.0f / refreshRate;
	}
#endif
	timeScale = 1.0f;
}

void Time::Refresh()
{
	LARGE_INTEGER last = current;
	QueryPerformanceCounter(&current);

#ifdef USE_TIME_BUF
	float dt = static_cast<float >(current.QuadPart - last.QuadPart) / freq.QuadPart;
	
	// 処理時間を保存
	timeBuf[writeP] = dt;
	writeP = (writeP+1) % BUF_SIZE;
	if (writeP == readP) {
		readP = (readP + 1) % BUF_SIZE;
	}

	// 直近の処理時間の平均を計算
	float sum = 0;
	int num = 0;
	for (int i = readP; i != writeP; i = (i + 1) % BUF_SIZE) {
		sum += timeBuf[i];
		num++;
	}

	// 平均値の2倍以上の場合、平均値の2倍に制限
	if (num > BUF_SIZE / 2) {
		float ave = sum / num;
		if (dt >= ave*2) {
			deltaTime = ave * 2;
		}
		else {
			deltaTime = dt;
		}
	}
	else {
		deltaTime = dt;
	}
#else
	deltaTime = static_cast<float>(current.QuadPart - last.QuadPart) / freq.QuadPart;
#endif
}

float Time::DeltaTime()
{
	return deltaTime * timeScale;
}

float Time::DeltaTimePlane()
{
	return deltaTime;
}

void Time::SetTimeScale(float scale)
{
	timeScale = scale;
}

float Time::GetFPS()
{
#ifdef USE_TIME_BUF
	return 1.0f / timeBuf[0];
#else
	return 1.0f / deltaTime;
#endif
}

void Time::Release()
{
}