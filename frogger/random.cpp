#include "random.h"

void Random::Init()
{
	std::random_device rd;
	mEngine.seed(rd());
}

float Random::GetFloat()
{
	return GetFloatRange(0.0f, 1.0f);
}

float Random::GetFloatRange(float min, float max)
{
	std::uniform_real_distribution<float> urd{ min, max };
	return urd(mEngine);
}

int Random::GetIntRange(int min, int max)
{
	std::uniform_int_distribution<int> uid{ min, max };
	return uid(mEngine);
}

std::mt19937 Random::mEngine;


