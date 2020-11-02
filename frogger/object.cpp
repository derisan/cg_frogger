#include "object.h"

#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "shader.h"
#include "game.h"
#include "component.h"

Object::Object(Game* game)
	: mGame{ game },
	mState{ State::kActive },
	mWorldTransform{ 1.0f },
	mPosition{ 0.0f },
	mScale{ 1.0f },
	mRotation{ 0.0f },
	mColor{ 1.0f },
	mRecomputeWorldTransform{ true }
{
	mGame->AddObj(this);
}

Object::~Object()
{
	mGame->RemoveObj(this);
}

void Object::Update()
{
	if (mState == State::kActive)
	{
		ComputeWorldTransform();

		UpdateComponents();
		UpdateObject();

		ComputeWorldTransform();
	}
}

void Object::UpdateComponents()
{
	for (auto comp : mComponents)
		comp->Update();
}

void Object::Draw(Shader* shader)
{
	shader->SetActive();
	shader->SetMatrix4Uniform("uWorld", mWorldTransform);
	shader->SetVectorUniform("uObjectColor", mColor);
}

void Object::ComputeWorldTransform()
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

void Object::AddComponent(Component* component)
{
	mComponents.emplace_back(component);
}

void Object::RemoveComponent(Component* component)
{
	auto iter = std::find(std::begin(mComponents), std::end(mComponents), component);
	if (iter != std::end(mComponents))
		mComponents.erase(iter);
}

void Object::ProcessKeyboardInput(unsigned char key)
{
	if (mState == State::kActive)
	{
		for (auto comp : mComponents)
			comp->ProcessKeyboardInput(key);

		ObjectKeyboardInput(key);
	}
}

glm::vec3 Object::GetForward() const
{
	return glm::rotateY(glm::vec3{ 0.0, 0.0f, -1.0f }, glm::radians(mRotation));
}