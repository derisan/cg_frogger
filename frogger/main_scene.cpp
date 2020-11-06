#include "main_scene.h"

#include <iostream>
#include <algorithm>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "gfw.h"
#include "game.h"
#include "sound_engine.h"

MainScene::MainScene(Gfw* gfw)
	: Scene{ gfw },
	mGame{ nullptr }
{

}

void MainScene::Enter()
{
	mGame = new Game{ this };

	if (!mGame->Init())
	{
		std::cout << "Failed to initialize game" << std::endl;
		return;
	}

	SoundEngine::Get()->Play("bgm");
}

void MainScene::Exit()
{
	// Shutdown game
	mGame->Shutdown();
	delete mGame;

	SoundEngine::Get()->Stop("bgm");
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
}

void MainScene::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	mGame->Draw();

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