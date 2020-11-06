#pragma once

class Scene
{
public:
	enum class State
	{
		kActive,
		kPaused
	};

	Scene(class Gfw* gfw);
	virtual ~Scene() = default;

	virtual void Enter() { }
	virtual void Exit() { }
	virtual void ProcessInput(unsigned char key) { }
	virtual void Update() { }
	virtual void Draw() { }
	virtual void Pause() { mState = State::kPaused; }
	virtual void Resume() { mState = State::kActive; }

	virtual void AddSceneActor(class SceneActor* actor) { };
	virtual void RemoveSceneActor(class SceneActor* actor) { };

	// Getters
	State GetState() const { return mState; }
	class Gfw* GetGfw() { return mGfw; }

	// Setters
	void SetState(State state) { mState = state; }

protected:
	class Gfw* mGfw;
	const float dt{ 16.0f / 1000.f };

private:
	State mState;
};