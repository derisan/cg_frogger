#include "game.h"

#include <iostream>
#include <algorithm>

#include "renderer.h"
#include "player.h"
#include "plane.h"
#include "vehicle.h"
#include "circle_component.h"
#include "box_component.h"

Game::Game()
	: mShouldCloseWindow{ false },
	mShouldPause{ false },
	mRenderer{ nullptr },
	mPlayer{ nullptr }
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
	mPlayer = new Player{ this };
		
	CreateMap();
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
	std::vector<Actor*> deads;
	for (auto actor : mActors)
	{
		actor->Update();
		if (actor->GetState() == Actor::State::kDead)
			deads.emplace_back(actor);
	}

	for (auto actor : deads)
		delete actor;

	static int times = 0;
	for(auto vehicle: mVehicles)
		if(Intersects(mPlayer->GetBox()->GetWorldBox(), vehicle->GetBox()->GetWorldBox()))
				std::cout << "Player collides " << times++ << std::endl;
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

void Game::CreateMap()
{
	int start{ 0 };
	int end{ 2 };

	for (; start < end; ++start)
	{
		auto plane = new Plane{ this, Plane::Type::kGrass };
		plane->SetPosition(glm::vec3{ 0.0f, -0.1f, -2.0f * start });
	}
	
	end = 10;
	for (; start < end; ++start)
	{
		auto plane = new Plane{ this, Plane::Type::kRoad };
		plane->SetPosition(glm::vec3{ 0.0f, -0.1f, -2.0f * start });
	}
}