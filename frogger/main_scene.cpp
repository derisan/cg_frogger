#include "main_scene.h"

#include <iostream>
#include <algorithm>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "gfw.h"
#include "game.h"
#include "sound_engine.h"
#include "life_gauge.h"
#include "renderer.h"
#include "shader.h"

MainScene::MainScene(Gfw* gfw)
	: Scene{ gfw },
	mGame{ nullptr },
	mSpriteShader{ nullptr }
{

}

void MainScene::Enter()
{
	mGame = new Game{ this };
	mSpriteShader = Renderer::Get()->GetShader("sprite");

	if (!mGame->Init())
	{
		std::cout << "Failed to initialize game" << std::endl;
		return;
	}

	new LifeGauge{ this };
}

void MainScene::Exit()
{
	// Shutdown game
	mGame->Shutdown();
	delete mGame;
}

void MainScene::ProcessInput(unsigned char key)
{
	if (key == 27)
		mGfw->ChangeScene("start");
	else if (key == 'p' || key == 'P')
		Pause();

	mGame->ProcessInput(key);
}

void MainScene::Update()
{
	if (GetState() == State::kPaused)
		return;

	mGame->Update();
	if (mGame->GetShouldCloseGame())
		mGfw->ChangeScene("dead");

	for (auto actor : mActors)
		actor->Update();
}

void MainScene::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	mGame->Draw();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	mSpriteShader->SetActive();
	for (auto actor : mActors)
		actor->Draw(mSpriteShader);

	glutSwapBuffers();
}

void MainScene::Pause()
{
	Scene::Pause();
}

void MainScene::Resume()
{
	Scene::Resume();
}

void MainScene::AddSceneActor(SceneActor* actor)
{
	mActors.emplace_back(actor);
}

void MainScene::RemoveSceneActor(SceneActor* actor)
{
	auto iter = std::find(std::begin(mActors), std::end(mActors), actor);
	if (iter != std::end(mActors))
		mActors.erase(iter);
}