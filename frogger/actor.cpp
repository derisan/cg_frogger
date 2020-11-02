#include "actor.h"

#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "shader.h"
#include "game.h"
#include "component.h"

Actor::Actor(Game* game)
	: mGame{ game },
	mState{ State::kActive },
	mWorldTransform{ 1.0f },
	mPosition{ 0.0f },
	mScale{ 1.0f },
	mRotation{ 0.0f },
	mRecomputeWorldTransform{ true }
{
	mGame->AddActor(this);
}

Actor::~Actor()
{
	mGame->RemoveActor(this);
}

void Actor::Update()
{
	if (mState == State::kActive)
	{
		ComputeWorldTransform();

		UpdateComponents();
		UpdateActor();

		ComputeWorldTransform();
	}
}

void Actor::UpdateComponents()
{
	for (auto comp : mComponents)
		comp->Update();
}

void Actor::ComputeWorldTransform()
{
	if (mRecomputeWorldTransform)
	{
		mRecomputeWorldTransform = false;

		mWorldTransform = glm::mat4{ 1.0f };
		mWorldTransform = glm::translate(mWorldTransform, mPosition);
		mWorldTransform = glm::rotate(mWorldTransform, glm::radians(mRotation), glm::vec3{ 0.0f, 1.0f, 0.0f });
		mWorldTransform = glm::scale(mWorldTransform, mScale);
	}
}

void Actor::AddComponent(Component* component)
{
	mComponents.emplace_back(component);
}

void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(std::begin(mComponents), std::end(mComponents), component);
	if (iter != std::end(mComponents))
		mComponents.erase(iter);
}

void Actor::ProcessInput(unsigned char key)
{
	if (mState == State::kActive)
	{
		for (auto comp : mComponents)
			comp->ProcessKeyboardInput(key);

		ActorInput(key);
	}
}

glm::vec3 Actor::GetForward() const
{
	return glm::rotateY(glm::vec3{ 0.0, 0.0f, -1.0f }, glm::radians(mRotation));
}