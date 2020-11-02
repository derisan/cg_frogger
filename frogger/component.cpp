#include "component.h"

#include "object.h"

Component::Component(Object* owner)
	: mOwner{ owner }
{
	mOwner->AddComponent(this);
}

Component::~Component()
{
	mOwner->RemoveComponent(this);
}

