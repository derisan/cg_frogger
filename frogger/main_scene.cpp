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
#include "plane.h"
#include "background.h"

MainScene::MainScene(Gfw* gfw)
	: Scene{ gfw },
	mGame{ nullptr },
	mSpriteShader{ nullptr },
	mBackground{ nullptr }
{

}

void MainScene::Enter()
{
	SetState(State::kActive);

	mGame = new Game{ this };
	mSpriteShader = Renderer::Get()->GetShader("sprite");
	mBackground = new Background{"Assets/space.jpg"};

	if (!mGame->Init())
	{
		std::cout << "Failed to initialize game" << std::endl;
		return;
	}

	auto lives = mGame->GetPlayerLives();
	for (int i = 0; i < lives; ++i)
	{
		auto heart = new LifeGauge{ this };
		heart->SetPosition(glm::vec3{ -0.8f + 0.2f * i, 0.8f, 0.0f });
		mLives.emplace_back(heart);
	}
}

void MainScene::Exit()
{
	// Shutdown game
	mGame->Shutdown();
	delete mGame;
	mGame = nullptr;

	delete mBackground;

	while (!mActors.empty())
		delete mActors.back();
	mLives.clear();
}

void MainScene::ProcessInput(unsigned char key)
{
	if (key == 27)
		mGfw->ChangeScene("start");
	else if (key == 'p' || key == 'P')
		Pause();
	else if (key == 'r' || key == 'R')
		Resume();
	//else if (key == 't')
	//	mGfw->ChangeScene("win");
	//else if (key == 'y')
	//	mGfw->ChangeScene("dead");

	if (GetState() == State::kPaused || !mGame)
		return;
	mGame->ProcessInput(key);
}

void MainScene::Update()
{
	if (GetState() == State::kPaused || !mGame)
		return;

	mGame->Update();

	std::vector<SceneActor*> deads;
	for (auto actor : mActors)
	{
		actor->Update();
		if (actor->GetState() == SceneActor::State::kDead)
			deads.emplace_back(actor);
	}

	if (mGame->GetPlayerLives() < mLives.size())
	{
		mLives[mLives.size() - 1]->SetState(SceneActor::State::kDead);
		mLives.pop_back();
	}
	
	for (auto actor : deads)
		delete actor;

	if (mGame->GetShouldCloseGame())
		mGfw->ChangeScene("dead");

	if (mGame && mGame->IsWin())
		mGfw->ChangeScene("win");
}

void MainScene::Draw()
{
	if (!mGame)
		return;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	mBackground->Draw();
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