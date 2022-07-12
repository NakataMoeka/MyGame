#include "Ease.h"
using namespace DirectX;

//tÇÕéûä‘Ç≈ÇÕÇ»Ç≠éûä‘ÇÃäÑçá

const XMFLOAT2 Eas::lerp(const XMFLOAT2& start, const XMFLOAT2& end, const float t)
{
	XMFLOAT2 num;

	num.x = start.x * (1.0f - t) + end.x * t;
	num.y = start.y * (1.0f - t) + end.y * t;


	return num;
}


const  XMFLOAT3 Eas::lerp(const XMFLOAT3& start, const XMFLOAT3& end, const float t)
{
	XMFLOAT3 num;

	num.x = start.x * (1.0f - t) + end.x * t;
	num.y = start.y * (1.0f - t) + end.y * t;
	num.z = start.z * (1.0f - t) + end.z * t;

	return num;
}
const XMFLOAT2 Eas::easeIn(const XMFLOAT2& start, const XMFLOAT2& end, const float t)
{
	XMFLOAT2 num;

	//t*tÇ

	num.x = start.x * (1.0f - t * t) + end.x * (t * t);
	num.y = start.y * (1.0f - t * t) + end.y * (t * t);
	return num;
}
const XMFLOAT3 Eas::easeIn(const XMFLOAT3& start, const XMFLOAT3& end, const float t)
{
	XMFLOAT3 num;

	//t*tÇ

	num.x = start.x * (1.0f - t * t) + end.x * (t * t);
	num.y = start.y * (1.0f - t * t) + end.y * (t * t);
	num.z = start.z * (1.0f - t * t) + end.z * (t * t);

	return num;
}

const XMFLOAT2 Eas::easeOut(const XMFLOAT2& start, const XMFLOAT2& end, const float t)
{
	XMFLOAT2 num;
	num.x = start.x * (1.0f - (t * (2 - t))) + end.x * (t * (2 - t));
	num.y = start.y * (1.0f - (t * (2 - t))) + end.y * (t * (2 - t));
	return num;
}

const XMFLOAT3 Eas::easeOut(const XMFLOAT3& start, const XMFLOAT3& end, const float t)
{
	XMFLOAT3 num;

	num.x = start.x * (1.0f - (t * (2 - t))) + end.x * (t * (2 - t));
	num.y = start.y * (1.0f - (t * (2 - t))) + end.y * (t * (2 - t));
	num.z = start.z * (1.0f - (t * (2 - t))) + end.z * (t * (2 - t));
	return num;
}

const XMFLOAT2 Eas::easeInOut(const XMFLOAT2& start, const XMFLOAT2& end, const float t)
{
	XMFLOAT2 num;
	num.x = start.x * (1.0f - (t * t * (3 - 2 * t))) + end.x * (t * t * (3 - 2 * t));
	num.y = start.y * (1.0f - (t * t * (3 - 2 * t))) + end.y * (t * t * (3 - 2 * t));
	return num;
}

const XMFLOAT3 Eas::easeInOut(const XMFLOAT3& start, const XMFLOAT3& end, const float t)
{
	XMFLOAT3 num;

	num.x = start.x * (1.0f - (t * t * (3 - 2 * t))) + end.x * (t * t * (3 - 2 * t));
	num.y = start.y * (1.0f - (t * t * (3 - 2 * t))) + end.y * (t * t * (3 - 2 * t));
	num.z = start.z * (1.0f - (t * t * (3 - 2 * t))) + end.z * (t * t * (3 - 2 * t));
	return num;
}

const XMFLOAT2 Eas::easeInCubic(const XMFLOAT2& start, const XMFLOAT2& end, float t, float d)
{
	XMFLOAT2 num;
	float x = t / d;
	float v = x * x * x;
	num.x = (end.x - start.x) * v + start.x;
	num.y = (end.y - start.y) * v + start.y;


	return num;
}

const XMFLOAT3 Eas::easeInCubic(const XMFLOAT3& start, const XMFLOAT3& end, float t, float d)
{
	XMFLOAT3 num;
	float x = t / d;
	float v = x * x * x;
	num.x = (end.x - start.x) * v + start.x;
	num.y = (end.y - start.y) * v + start.y;
	num.z = (end.z - start.z) * v + start.z;

	return num;
}

const XMFLOAT2 Eas::easeOutCubic(const XMFLOAT2& start, const XMFLOAT2& end, float t, float d)
{
	XMFLOAT2 num;
	float x = t / d;
	float v = 1 - pow(1 - x, 3);
	num.x = (end.x - start.x) * v + start.x;
	num.y = (end.y - start.y) * v + start.y;


	return num;
}

const XMFLOAT3 Eas::easeOutCubic(const XMFLOAT3& start, const XMFLOAT3& end, float t, float d)
{
	XMFLOAT3 num;
	float x = t / d;
	float v = 1 - pow(1 - x, 3);
	num.x = (end.x - start.x) * v + start.x;
	num.y = (end.y - start.y) * v + start.y;
	num.z = (end.z - start.z) * v + start.z;

	return num;
}

const XMFLOAT2 Eas::easeInQuad(const XMFLOAT2& start, const XMFLOAT2& end, float t, float d)
{
	XMFLOAT2 num;
	float x = t / d;
	float v = x * x;
	num.x = (end.x - start.x) * v + start.x;
	num.y = (end.y - start.y) * v + start.y;


	return num;
}

const XMFLOAT3 Eas::easeInQuad(const XMFLOAT3& start, const XMFLOAT3& end, float t, float d)
{
	XMFLOAT3 num;
	float x = t / d;
	float v = x * x;
	num.x = (end.x - start.x) * v + start.x;
	num.y = (end.y - start.y) * v + start.y;
	num.z = (end.z - start.z) * v + start.z;

	return num;
}

const XMFLOAT2 Eas::easeOutQuad(const XMFLOAT2& start, const XMFLOAT2& end, float t, float d)
{
	XMFLOAT2 num;
	float x = t / d;
	float v = 1 - (1 - x) * (1 - x);
	num.x = (end.x - start.x) * v + start.x;
	num.y = (end.y - start.y) * v + start.y;


	return num;
}

const XMFLOAT3 Eas::easeOutQuad(const XMFLOAT3& start, const XMFLOAT3& end, float t, float d)
{
	XMFLOAT3 num;
	float x = t / d;
	float v = 1 - (1 - x) * (1 - x);
	num.x = (end.x - start.x) * v + start.x;
	num.y = (end.y - start.y) * v + start.y;
	num.z = (end.z - start.z) * v + start.z;

	return num;
}
