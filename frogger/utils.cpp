#include "utils.h"

glm::vec2 ScreenToNDC(int x, int y, float w, float h)
{
	float retX = (static_cast<float>(x) - w / 2) * (2 / w);
	float retY = (static_cast<float>(-y) + h / 2) * (2 / h);

	return glm::vec2{ retX, retY };
}

bool IsIntersect(const glm::vec2& a1, const glm::vec2& a2,
	const glm::vec2& b1, const glm::vec2& b2, glm::vec2& out)
{
	double t;
	double s;
	double under = static_cast<double>((b2.y - b1.y) * (a2.x - a1.x) - (b2.x - b1.x) * (a2.y - a1.y));
	if (under == 0) return false;

	double _t = static_cast<double>((b2.x - b1.x) * (a1.y - b1.y) - (b2.y - b1.y) * (a1.x - b1.x));
	double _s = static_cast<double>((a2.x - a1.x) * (a1.y - b1.y) - (a2.y - a1.y) * (a1.x - b1.x));

	t = _t / under;
	s = _s / under;

	if (t < 0.0 || t>1.0 || s < 0.0 || s>1.0) return false;
	if (_t == 0 && _s == 0) return false;

	out.x = a1.x + t * (a2.x - a1.x);
	out.y = a1.y + t * (a2.y - a1.y);

	return true;
}