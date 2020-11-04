#include "game.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "renderer.h"
#include "player.h"
#include "plane.h"
#include "vehicle.h"
#include "circle_component.h"
#include "box_component.h"
#include "random.h"

Game::Game()
	: mShouldCloseWindow{ false },
	mShouldPause{ false },
	mIsUpdating{ false },
	mRenderer{ nullptr },
	mPlayer{ nullptr }
{

}

bool Game::Init(int* argc, char** argv)
{
	mRenderer = new Renderer{ this };
	if (!mRenderer->Init(argc, argv))
		return false;

	Random::Init();

	if (!LoadData())
		return false;

	return true;
}

bool Game::LoadData()
{
	std::ifstream file{ "Assets/stage.txt" };
	if (!file.is_open())
	{
		std::cout << "Failed to read stage file" << std::endl;
		return false;
	}

	std::stringstream s;
	s << file.rdbuf();
	std::string contents = s.str();

	for (auto ch : contents)
		if (ch != '\n' && ch != ' ')
			mStage.emplace_back(ch - 65);

	mPlayer = new Player{ this };
		
	CreateMap();

	return true;
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
	else if (key == 'p' || key == 'P')
		mShouldPause = !mShouldPause;

	for (auto actor : mActors)
		actor->ProcessInput(key);
}

void Game::Update()
{
	if (mShouldPause)
		return;

	std::vector<Actor*> deads;
	mIsUpdating = true;
	for (auto actor : mActors)
	{
		actor->Update();
		if (actor->GetState() == Actor::State::kDead)
			deads.emplace_back(actor);
	}
	mIsUpdating = false;

	for (auto pending : mPendingActors)
	{
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	CollisionCheck();

	for (auto actor : deads)
		delete actor;
}

void Game::Draw()
{
	mRenderer->Draw();
}

void Game::AddActor(Actor* actor)
{
	if (mIsUpdating)
		mPendingActors.emplace_back(actor);
	else
		mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(std::begin(mPendingActors), std::end(mPendingActors), actor);
	if (iter != std::end(mPendingActors))
		mPendingActors.erase(iter);

	iter = std::find(std::begin(mActors), std::end(mActors), actor);
	if (iter != std::end(mActors))
		mActors.erase(iter);
}

void Game::CreateMap()
{
	int idx{ 0 };
	for (auto st : mStage)
	{
		auto plane = new Plane{ this, static_cast<Plane::PlaneType>(st) };
		plane->SetPosition(glm::vec3{ 0.0f, -0.2f, -2.0f * idx++ });
	}
}

void Game::CollisionCheck()
{
	auto playerBox = mPlayer->GetBox()->GetWorldBox();

	static int times = 0;
	for (auto vehicle : mVehicles)
	{
		if (vehicle->GetState() != Actor::State::kActive)
			continue;

		if (Intersects(playerBox, vehicle->GetBox()->GetWorldBox()))
		{
			std::cout << "Player collides with vehicle" << times++ << std::endl;
			//mShouldPause = true;
		}
	}

	for (auto plane : mPlanes)
	{
		if (plane->GetState() != Actor::State::kActive)
			continue;

		if (Intersects(playerBox, plane))
		{
			std::cout << "Player collides with plane" << times++ << std::endl;
		}
	}
}