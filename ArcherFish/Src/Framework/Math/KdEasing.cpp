#include "KdEasing.h"

inline float KdEase::InSine(float _progress)
{
	return 1 - cos((_progress * DirectX::XM_PI) * 0.5f);
}

inline float KdEase::OutSine(float _progress)
{
	return sin((_progress * DirectX::XM_PI) * 0.5f);
}

inline float KdEase::InOutSine(float _progress)
{
	return -(cos(DirectX::XM_PI * _progress) - 1.0f) * 0.5f;
}

inline float KdEase::OutBounce(float _progress)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (_progress < 1.0f / d1) { return n1 * _progress * _progress; }
	else if (_progress < 2.0f / d1) { return n1 * (_progress -= 1.5f / d1) * _progress + 0.75f; }
	else if (_progress < 2.5f / d1) { return n1 * (_progress -= 2.25f / d1) * _progress + 0.9375f; }
	else { return n1 * (_progress -= 2.625f / d1) * _progress + 0.984375f; }
}