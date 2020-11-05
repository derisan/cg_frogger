#include "game.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "renderer.h"
#include "player.h"
#include "plane.h"
#include "vehicle.h"
#include "tree.h"
#include "circle_component.h"
#include "box_component.h"
#include "random.h"
#include "sound_engine.h"

Game::Game()
	: mShouldCloseWindow{ false },
	mShouldPause{ false },
	mIsUpdating{ false },
	mRenderer{ nullptr },
	mPlayer{ nullptr },
	mSoundEngine{ nullptr },
	mCurStage{ 0 }
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
	// Read stage from file
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
		
	// Generate singleton instance of SoundEngine
	// Load several sounds
	mSoundEngine = SoundEngine::Get();
	mSoundEngine->Create("Sounds/bgm.wav", "bgm", true);
	mSoundEngine->Create("Sounds/jump.wav", "jump", false);
	mSoundEngine->Play("bgm");

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

	delete mSoundEngine;
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
	{
		if (RemoveVehicle(actor))
			;
		else if (RemovePlane(actor))
			;
		else
			RemoveTree(actor);
		delete actor;
	}

	CreateMap();
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
	// Generate plane along player's z position value
	auto zPos = mPlayer->GetPosition().z;
	for (; mCurStage < -zPos + 10; ++mCurStage)
	{
		auto plane = new Plane{ this, static_cast<Plane::PlaneType>( mStage[mCurStage]) };
	}
}

void Game::CollisionCheck()
{
	const auto& playerBox = mPlayer->GetBox()->GetWorldBox();

	static int times = 0;
	for (auto vehicle : mVehicles)
	{
		if (vehicle->GetState() != Actor::State::kActive)
			continue;

		if (Intersects(playerBox, vehicle->GetBox()->GetWorldBox()))
		{
			if (vehicle->GetType() == Vehicle::VehicleType::kLog)
			{
				auto pos = mPlayer->GetPosition();
				pos.x = vehicle->GetPosition().x;
				pos.y = 0.0f;
				mPlayer->SetPosition(pos);
			}

			//std::cout << times++ << std::endl;
		}
	}

	for (auto plane : mPlanes)
	{
		if (plane->GetState() != Actor::State::kActive)
			continue;

		if (Intersects(playerBox, plane))
		{
			if (plane->GetType() != Plane::PlaneType::kWater)
			{
				auto pos = mPlayer->GetPosition();
				pos.y = 0.0f;
				mPlayer->SetPosition(pos);
			}
		}
	}

	for (auto tree : mTrees)
	{
		if (tree->GetState() != Actor::State::kActive)
			continue;

		if (Intersects(playerBox, tree->GetBox()->GetWorldBox()))
		{
			
		}
	}
}

bool Game::RemoveVehicle(Actor* actor)
{
	auto iter = std::find(std::begin(mVehicles), std::end(mVehicles), actor);
	if (iter != std::end(mVehicles))
	{
		mVehicles.erase(iter);
		return true;
	}
	return false;
}

bool Game::RemovePlane(Actor* actor)
{
	auto iter = std::find(std::begin(mPlanes), std::end(mPlanes), actor);
	if (iter != std::end(mPlanes))
	{
		mPlanes.erase(iter);
		return true;
	}
	return false;
}

bool Game::RemoveTree(Actor* actor)
{
	auto iter = std::find(std::begin(mTrees), std::end(mTrees), actor);
	if (iter != std::end(mTrees))
	{
		mTrees.erase(iter);
		return true;
	}
	return false;
}