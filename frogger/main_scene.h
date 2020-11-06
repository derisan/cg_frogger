#pragma once

#include "scene.h"

#include <vector>

class MainScene :
    public Scene
{
public:
	MainScene(class Gfw* gfw);

	void Enter() override;
	void Exit() override;
	void ProcessInput(unsigned char key) override;
	void Update() override;
	void Draw() override;
	void Pause() override;
	void Resume() override;

	void AddSceneActor(class SceneActor* actor) override;
	void RemoveSceneActor(class SceneActor* actor) override;

private:
	std::vector<class SceneActor*> mActors;

	class Game* mGame;
};