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
#include "skill.h"

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
	mPhongShader{ nullptr },
	mPlayer{ nullptr },
	mView{ 1.0f },
	mIsUpdating{ false },
	mCurStage{ 0 },
	mSkillCooldown{ 0.0f }
{

}

bool Game::Init()
{
	mRenderer = Renderer::Get();

	glm::mat4 proj{ 1.0f };
	proj = glm::perspective(45.0f, static_cast<float>(mScene->GetGfw()->GetScrWidth()) / mScene->GetGfw()->GetScrHeight(),
		0.1f, 100.0f);
	
	mPhongShader = mRenderer->GetShader("phong");
	mPhongShader->SetActive();
	mPhongShader->SetMatrix4Uniform("uProj", proj);

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


	if (key == 32 && mSkillCooldown < 0.0f)
	{
		LaunchSkills();
		mSkillCooldown = 5.0f;
	}
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

	mSkillCooldown -= dt;

	CollisionCheck();
	CreateMap();
}

void Game::Draw()
{
	mPhongShader->SetActive();
	SetPhongUniforms();
	for (auto actor : mActors)
		actor->Draw(mPhongShader);
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
				mPlayer->HitByCar();
			}
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

bool Game::RemoveNearVehicle(Actor* actor)
{
	auto iter = std::find(std::begin(mNearVehicles), std::end(mNearVehicles), actor);
	if (iter != std::end(mNearVehicles))
	{
		mNearVehicles.erase(iter);
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

int Game::GetPlayerLives() const
{
	return mPlayer->GetLives();
}

void Game::SetPhongUniforms()
{
	mPhongShader->SetMatrix4Uniform("uView", mView);
	mPhongShader->SetVectorUniform("uViewPos", mCameraPos);
	mPhongShader->SetVectorUniform("uDirLight.direction", glm::vec3{ -12.0f, 5.0f, -0.1f });
	mPhongShader->SetVectorUniform("uDirLight.ambient", glm::vec3{ 0.1f });
	mPhongShader->SetVectorUniform("uDirLight.diffuse", glm::vec3{ 1.0f });
	mPhongShader->SetVectorUniform("uDirLight.specular", glm::vec3{ 1.0f });

	//mPhongShader->SetVectorUniform("uPointLights[0].position", glm::vec3{ 0.0f, 1.0f, -2.0f });
	//mPhongShader->SetVectorUniform("uPointLights[0].ambient", glm::vec3{ 0.05f });
	//mPhongShader->SetVectorUniform("uPointLights[0].diffuse", glm::vec3{ 0.8f });
	//mPhongShader->SetVectorUniform("uPointLights[0].specular", glm::vec3{ 1.0f });
	//mPhongShader->SetFloatUniform("uPointLights[0].constant", 1.0f);
	//mPhongShader->SetFloatUniform("uPointLights[0].linear", 0.07f);
	//mPhongShader->SetFloatUniform("uPointLights[0].quadratic", 0.017f);

	const auto& playerPos = mPlayer->GetPosition();
	for (auto lamp : mTrees)
	{	
		if(lamp->GetType() != Tree::TreeType::kLamp)
			continue;

		const auto& lampPos = lamp->GetPosition();
		if (fabs(lampPos.z - playerPos.z) > 14.0f)
			continue;

		std::string path = "uPointLights[0].";
		mPhongShader->SetVectorUniform(path + "position", lampPos);
		mPhongShader->SetVectorUniform(path + "ambient", glm::vec3{ 0.05f });
		mPhongShader->SetVectorUniform(path + "diffuse", glm::vec3{ 0.8f });
		mPhongShader->SetVectorUniform(path + "specular", glm::vec3{ 1.0f });
		mPhongShader->SetFloatUniform(path + "constant", 1.0f);
		mPhongShader->SetFloatUniform(path + "linear", 0.045f);
		mPhongShader->SetFloatUniform(path + "quadratic", 0.0075f);

		break;
	}

	mPhongShader->SetVectorUniform("uSpotLight.position", mPlayer->GetPosition());
	mPhongShader->SetVectorUniform("uSpotLight.direction", glm::vec3{ 0.0f, -0.1f, 1.0f });
	mPhongShader->SetVectorUniform("uSpotLight.ambient", glm::vec3{ 0.1f });
	mPhongShader->SetVectorUniform("uSpotLight.diffuse", glm::vec3{ 1.0f });
	mPhongShader->SetVectorUniform("uSpotLight.specular", glm::vec3{ 1.0f });
	mPhongShader->SetFloatUniform("uSpotLight.constant", 1.0f);
	mPhongShader->SetFloatUniform("uSpotLight.linear", 0.027f);
	mPhongShader->SetFloatUniform("uSpotLight.quadratic", 0.0028f);
	mPhongShader->SetFloatUniform("uSpotLight.cutOff", glm::cos(glm::radians(25.0f)));
	mPhongShader->SetFloatUniform("uSpotLight.outerCutOff", glm::cos(glm::radians(35.0f)));
}

void Game::LaunchSkills()
{
	mNearVehicles.clear();
	for (auto vehicle : mVehicles)
	{
		if (vehicle->GetState() != Actor::State::kActive || vehicle->GetType() == Vehicle::VehicleType::kLog)
			continue;

		const auto& vPos = vehicle->GetPosition();
		const auto& pPos = mPlayer->GetPosition();
		if (vPos.z <= pPos.z && fabs(vPos.z - pPos.z) <= 12.0f)
			mNearVehicles.emplace_back(vehicle);
	}
	
	for (auto vehicle : mNearVehicles)
	{
		vehicle->SetSpeed(0.0f);
		new Skill{ this, vehicle };
	}
}