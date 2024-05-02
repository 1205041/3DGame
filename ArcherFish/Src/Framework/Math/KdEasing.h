#pragma once

struct KdEase
{
	inline float InSine(float _progress);
	inline float OutSine(float _progress);
	inline float InOutSine(float _progress);

	inline float OutBounce(float _progress);
};