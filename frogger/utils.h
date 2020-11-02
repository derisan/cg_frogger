#pragma once

#include <glm/glm.hpp>

glm::vec2 ScreenToNDC(int x, int y, float w, float h);

bool IsIntersect(const glm::vec2& a1, const glm::vec2& a2,
	const glm::vec2& b1, const glm::vec2& b2, glm::vec2& out);