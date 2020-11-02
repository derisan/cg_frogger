#include "game.h"

#include <iostream>
#include <algorithm>

#include "random.h"
#include "actor.h"
#include "renderer.h"

Game::Game(int w, int h)
	: mShouldCloseWindow{ false },
	mShouldPause{ false },
	mRenderer{ nullptr }
{

}

bool Game::Init(int* argc, char** argv)
{
	mRenderer = new Renderer{ this };
	if (!mRenderer->Init(argc, argv, 1024, 768))
	{
		std::cout << "Failed to initialize renderer" << std::endl;
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}

	Random::Init();

	LoadData();

	return true;
}

void Game::LoadData()
{
	
}

void Game::Shutdown()
{
	while (!mActors.empty())
		delete mActors.back();

	if (mRenderer)
	{
		mRenderer->Shutdown();
		delete mRenderer;
	}
}

void Game::ProcessKeyboardInput(unsigned char key)
{
	switch (key)
	{		
		case 27:
			mShouldCloseWindow = true;
			break;
		case 'p': case 'P':
			mShouldPause = !mShouldPause;
			break;
	}

	for (auto actor : mActors)
		actor->ProcessInput(key);
}


void Game::Update()
{
	if (mShouldPause)
		return;

	std::vector<Actor*> deads;
	for (auto actor: mActors)
	{
		actor->Update();
		if (actor->GetState() == Actor::State::kDead)
			deads.emplace_back(actor);
	}

	for (auto actor : deads)
		delete actor;
	deads.clear();
}

void Game::Draw()
{
	mRenderer->Draw();
}

void Game::AddActor(Actor* actor)
{
	mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(std::begin(mActors), std::end(mActors), actor);
	if (iter != std::end(mActors))
		mActors.erase(iter);
}