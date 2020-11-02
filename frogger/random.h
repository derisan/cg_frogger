#pragma once

#include <random>

class Random
{
public:
	static void Init();

	// Returns 0.0f ~ 1.0f
	static float GetFloat();
	static float GetFloatRange(float min, float max);
	static int GetIntRange(int min, int max);

private:
	static std::mt19937 mEngine;
};

