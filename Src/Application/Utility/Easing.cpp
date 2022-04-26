#include "Easing.h"

float EaseIn(float nowValue, float endValue)
{
	float progress = nowValue / endValue;

	return static_cast<float>(1 - cos((progress * M_PI) * 0.5));
}

float EaseOut(float nowValue, float endValue)
{
	float progress = nowValue / endValue;

	return static_cast<float>(sin((progress * M_PI) * 0.5f));
}

float EaseInOut(float nowValue, float endValue)
{
	float progress = nowValue / endValue;

	return static_cast<float>(-(cos(M_PI * progress) - 1) * 0.5f);
}
