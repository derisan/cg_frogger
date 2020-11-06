#pragma once

#include "scene.h"

#include <vector>
#include <string>

class StartScene :
    public Scene
{
public:
	StartScene(class Gfw* gfw);

	void Enter() override;
	void Exit() override;
	void ProcessInput(unsigned char key) override;
	void Update() override;
	void Draw() override;

	void AddSceneActor(class SceneActor* actor) override;
	void RemoveSceneActor(class SceneActor* actor) override;

private:
	std::vector<class SceneActor*> mActors;

	class Renderer* mRenderer;
	class Shader* mSpriteShader;
};

