#pragma once
#include "component.h"

class MoveComponent :
    public Component
{
public:
    MoveComponent(class Actor* owner);

    void Update() override;

    // Getters
    float GetForwardSpeed() const { return mForwardSpeed; }

    // Setters
    void SetForwardSpeed(float speed) { mForwardSpeed = speed; }

private:
    float mForwardSpeed;
};

