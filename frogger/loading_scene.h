#pragma once

#include "scene.h"

#include <vector>
#include <string>

class LoadingScene :
    public Scene
{
public:
	LoadingScene(class Gfw* gfw);

	void Enter() override;
	void Exit() override;
	void ProcessInput(unsigned char key) override;
	void Update() override;
	void Draw() override;

	void AddSceneActor(class SceneActor* actor) override;
	void RemoveSceneActor(class SceneActor* actor) override;

	// Scene specific
	bool ReadFromJson(const std::string& file);

private:
	std::vector<class SceneActor*> mActors;
	std::vector<std::string> mMeshFiles;

	class Renderer* mRenderer;
	class Shader* mSpriteShader;

	unsigned int mCurIdx;
	float mElapsed;
};

