#include "movecomponent.h"

#include <glm/vec3.hpp>

#include "actor.h"

MoveComponent::MoveComponent(Actor* owner)
	: Component{ owner },
	mForwardSpeed{ 0.0f }
{

}

void MoveComponent::Update()
{
	auto curPos = mOwner->GetPosition();
	curPos += mOwner->GetForward() * mForwardSpeed;
	mOwner->SetPosition(curPos);
}