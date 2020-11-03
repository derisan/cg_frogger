#pragma once
#include "component.h"

#include <glm/vec3.hpp>

class CircleComponent :
    public Component
{
public:
    CircleComponent(class Actor* owner);

    // Getters
    float GetRadius() const;
    const glm::vec3& GetCenter() const;

    // Setters
    void SetRadius(float radius) { mRadius = radius; }

private:
    float mRadius;
};

bool Intersects(const CircleComponent& a, const CircleComponent& b);

