#pragma once

#include <vector>

#include <glm/glm.hpp>

class Actor
{
public:
	enum class State
	{
		kActive,
		kPaused,
		kDead
	};

	Actor(class Game* game);
	virtual ~Actor();

	void Update();
	void UpdateComponents();
	virtual void UpdateActor() { };

	void ProcessInput(unsigned char key);
	virtual void ActorInput(unsigned char key) { };

	virtual void Draw(class Shader* shader) { };

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	// Getters
	State GetState() const { return mState; }
	const glm::mat4& GetWorldTransform() const { return mWorldTransform; }
	const glm::vec3& GetPosition() const { return mPosition; }
	const glm::vec3& GetScale() const { return mScale; }
	float GetRotation() const { return mRotation; }
	glm::vec3 GetForward() const;

	// Setters
	void SetState(State state) { mState = state; }
	void SetPosition(const glm::vec3& position) { mPosition = position; mRecomputeWorldTransform = true; }
	void SetScale(const glm::vec3& scale) { mScale = scale; mRecomputeWorldTransform = true; }
	void SetRotation(float rotation) { mRotation = rotation; mRecomputeWorldTransform = true; }

protected:
	class Game* mGame;

private:
	void ComputeWorldTransform();

	std::vector<class Component*> mComponents;

	State mState;

	// Transforms
	glm::mat4 mWorldTransform;
	glm::vec3 mPosition;
	glm::vec3 mScale;
	float mRotation;

	bool mRecomputeWorldTransform;
};

