#include "circle_component.h"

#include <glm/gtx/norm.hpp>

#include "actor.h"

CircleComponent::CircleComponent(Actor* owner)
	: Component{ owner },
	mRadius{ 0.0f }
{

}

float CircleComponent::GetRadius() const
{
	return mOwner->GetScale() * mRadius;
}

const glm::vec3& CircleComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

bool Intersect(const CircleComponent& a, const CircleComponent& b)
{	
	float dist = glm::length2(a.GetCenter() - b.GetCenter());

	float radii = glm::length2(a.GetRadius() + b.GetRadius());
	
	return dist <= radii;
}