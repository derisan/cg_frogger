#include "movecomponent.h"

#include <glm/vec3.hpp>

#include "object.h"

MoveComponent::MoveComponent(Object* owner)
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