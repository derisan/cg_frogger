#include "start_scene.h"

#include <algorithm>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "gfw.h"
#include "renderer.h"
#include "shader.h"
#include "start_actor.h"
#include "sound_engine.h"

StartScene::StartScene(Gfw* gfw)
	: Scene{ gfw },
	mRenderer{ nullptr },
	mSpriteShader{ nullptr }
{

}

void StartScene::Enter()
{
	mRenderer = Renderer::Get();
	mSpriteShader = mRenderer->GetShader("sprite");

	new StartActor{ this };

	SoundEngine::Get()->Play("bgm");
}

void StartScene::Exit()
{
	while (!mActors.empty())
		delete mActors.back();

	SoundEngine::Get()->Stop("bgm");
}

void StartScene::ProcessInput(unsigned char key)
{
	if (key == 27)
		mGfw->PopScene();
	else if (key == 13) // enter
		mGfw->ChangeScene("main");
}

void StartScene::Update()
{
	for (auto actor : mActors)
		actor->Update();
}

void StartScene::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	mSpriteShader->SetActive();
	for (auto actor : mActors)
		actor->Draw(mSpriteShader);
	
	glutSwapBuffers();
}

void StartScene::AddSceneActor(SceneActor* actor)
{
	mActors.emplace_back(actor);
}

void StartScene::RemoveSceneActor(SceneActor* actor)
{
	auto iter = std::find(std::begin(mActors), std::end(mActors), actor);
	if (iter != std::end(mActors))
		mActors.erase(iter);
}