#include "scene_actor.h"

#include <glm/gtc/matrix_transform.hpp>

#include "scene.h"

SceneActor::SceneActor(class Scene* scene)
	: mScene{ scene },
	mState{ State::kActive },
	mWorldTransform{ 1.0f },
	mPosition{ 0.0f },
	mScale{ 1.0f },
	mRotation{ 0.0f },
	mAxis{ 0.0f, 1.0f, 0.0f },
	mRecomputeWorldTransform{ true }
{
	scene->AddSceneActor(this);
}

SceneActor::~SceneActor()
{
	mScene->RemoveSceneActor(this);
}

void SceneActor::Update()
{
	if (mState == State::kActive)
	{
		ComputeWorldTransform();

		UpdateSceneActor();

		ComputeWorldTransform();
	}
}

void SceneActor::ProcessInput(unsigned char key)
{
	if (mState == State::kActive)
	{
		SceneActorInput(key);
	}
}

void SceneActor::ComputeWorldTransform()
{
	if (mRecomputeWorldTransform)
	{
		mRecomputeWorldTransform = false;

		mWorldTransform = glm::mat4{ 1.0f };
		mWorldTransform = glm::translate(mWorldTransform, mPosition);
		mWorldTransform = glm::rotate(mWorldTransform, glm::radians(mRotation), mAxis);
		mWorldTransform = glm::scale(mWorldTransform, glm::vec3{ mScale });
	}
}