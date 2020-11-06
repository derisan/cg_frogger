#include "game.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

#include "player.h"
#include "plane.h"
#include "vehicle.h"
#include "tree.h"

#include "renderer.h"
#include "sound_engine.h"
#include "box_component.h"
#include "random.h"
#include "shader.h"
#include "scene.h"
#include "gfw.h"

Game::Game(Scene* scene)
	: mScene{ scene },
	mRenderer{ nullptr },
	mMeshShader{ nullptr },
	mPlayer{ nullptr },
	mView{ 1.0f },
	mIsUpdating{ false },
	mCurStage{ 0 }
{

}

bool Game::Init()
{
	mRenderer = Renderer::Get();
	mMeshShader = mRenderer->GetShader("basicMesh");
	mMeshShader->SetActive();
	glm::mat4 proj{ 1.0f };
	proj = glm::perspective(45.0f, static_cast<float>(mScene->GetGfw()->GetScrWidth()) / mScene->GetGfw()->GetScrHeight(),
		0.1f, 100.0f);
	mMeshShader->SetMatrix4Uniform("uProj", proj);
	mPlayer = new Player{ this };

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

	return true;
}

void Game::Shutdown()
{
	while (!mActors.empty())
		delete mActors.back();
}

void Game::ProcessInput(unsigned char key)
{
	for (auto actor : mActors)
		actor->ProcessInput(key);
}

void Game::Update()
{
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

	CollisionCheck();
	CreateMap();
}

void Game::Draw()
{
	mMeshShader->SetActive();
	mMeshShader->SetMatrix4Uniform("uView", mView);
	for (auto actor : mActors)
		actor->Draw(mMeshShader);
}

void Game::CreateMap()
{
	// Generate plane along player's z position value
	auto zPos = mPlayer->GetPosition().z;
	for (; mCurStage < -zPos + 10; ++mCurStage)
	{
		auto plane = new Plane{ this, static_cast<Plane::PlaneType>(mStage[mCurStage]) };
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

		const auto& vehicleBox = vehicle->GetBox()->GetWorldBox();
		if (Intersects(playerBox, vehicleBox))
		{
			if (vehicle->GetType() == Vehicle::VehicleType::kLog)
			{
				mPlayer->CollideWithLog(vehicle);
			}
			else
			{
				mPlayer->Die();
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
				mPlayer->NotToFall();
			}
		}
	}
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
