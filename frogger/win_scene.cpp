#include "win_scene.h"

#include <algorithm>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "gfw.h"
#include "renderer.h"
#include "shader.h"
#include "start_actor.h"
#include "sound_engine.h"

WinScene::WinScene(Gfw* gfw)
	: Scene{ gfw },
	mRenderer{ nullptr },
	mSpriteShader{ nullptr },
	mMeshShader{ nullptr }
{

}

void WinScene::Enter()
{
	mRenderer = Renderer::Get();
	mSpriteShader = mRenderer->GetShader("sprite");

	SoundEngine::Get()->Play("victory.mp3");
}

void WinScene::Exit()
{
	while (!mActors.empty())
		delete mActors.back();

	SoundEngine::Get()->Stop("victory.mp3");
}

void WinScene::ProcessInput(unsigned char key)
{
	if (key == 27)
		mGfw->PopScene();
	else if (key == 13) // enter
		mGfw->ChangeScene("main");
}

void WinScene::Update()
{
	for (auto actor : mActors)
		actor->Update();
}

void WinScene::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	mSpriteShader->SetActive();
	for (auto actor : mActors)
		actor->Draw(mSpriteShader);

	glutSwapBuffers();
}

void WinScene::AddSceneActor(SceneActor* actor)
{
	mActors.emplace_back(actor);
}

void WinScene::RemoveSceneActor(SceneActor* actor)
{
	auto iter = std::find(std::begin(mActors), std::end(mActors), actor);
	if (iter != std::end(mActors))
		mActors.erase(iter);
}