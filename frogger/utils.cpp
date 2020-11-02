#include "utils.h"

glm::vec2 ScreenToNDC(int x, int y, float w, float h)
{
	float retX = (static_cast<float>(x) - w / 2) * (2 / w);
	float retY = (static_cast<float>(-y) + h / 2) * (2 / h);

	return glm::vec2{ retX, retY };
}