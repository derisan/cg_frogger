#include "game.h"

#include <iostream>
#include <algorithm>

#include "renderer.h"
#include "player.h"
#include "plane.h"

Game::Game()
	: mShouldCloseWindow{ false },
	mShouldPause{ false },
	mRenderer{ nullptr }
{

}

bool Game::Init(int* argc, char** argv)
{
	mRenderer = new Renderer{ this };
	if (!mRenderer->Init(argc, argv))
		return false;

	LoadData();

	return true;
}

void Game::LoadData()
{
	auto player = new Player{ this };
		
	for (int i = 0; i < 10; ++i)
	{
		auto road = new Plane{ this, Plane::Type::kGrass };
		road->SetRotation(90.0f);
		road->SetPosition(glm::vec3{ -10.0f + i * 2.0f, 0.0f, 0.0f });
	}

	for (int i = 0; i < 10; ++i)
	{
		auto road = new Plane{ this, Plane::Type::kRoad};
		road->SetRotation(90.0f);
		road->SetPosition(glm::vec3{ -10.0f + i * 2.0f, 0.0f, -2.0f });
	}

	for (int i = 0; i < 10; ++i)
	{
		auto road = new Plane{ this, Plane::Type::kRoad };
		road->SetRotation(90.0f);
		road->SetPosition(glm::vec3{ -10.0f + i * 2.0f, 0.0f, -4.0f });
	}

	for (int i = 0; i < 10; ++i)
	{
		auto road = new Plane{ this, Plane::Type::kRoad };
		road->SetRotation(90.0f);
		road->SetPosition(glm::vec3{ -10.0f + i * 2.0f, 0.0f, -6.0f });
	}

	for (int i = 0; i < 10; ++i)
	{
		auto road = new Plane{ this, Plane::Type::kRoad };
		road->SetRotation(90.0f);
		road->SetPosition(glm::vec3{ -10.0f + i * 2.0f, 0.0f, -8.0f });
	}

	for (int i = 0; i < 10; ++i)
	{
		auto road = new Plane{ this, Plane::Type::kGrass };
		road->SetRotation(90.0f);
		road->SetPosition(glm::vec3{ -10.0f + i * 2.0f, 0.0f, -10.0f });
	}
}

void Game::Shutdown()
{
	while (!mActors.empty())
		delete mActors.back();

	if (mRenderer)
	{
		mRenderer->Shutdown();
		//delete mRenderer;
	}
}

void Game::ProcessKeyboardInput(unsigned char key)
{
	if (key == 27)
		mShouldCloseWindow = true;

	for (auto actor : mActors)
		actor->ProcessInput(key);
}

void Game::Update()
{
	for (auto actor : mActors)
		actor->Update();
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