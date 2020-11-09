#include "plane.h"

#include <iostream>

#include <GL/glew.h>

#include "game.h"
#include "renderer.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "vertexarray.h"
#include "random.h"
#include "player.h"
#include "box_component.h"
#include "tree.h"

Plane::Plane(Game* game, PlaneType type)
	: Actor{ game },
	mMesh{ nullptr },
	mBox{ nullptr },
	mCooldown{ 0.0f },
	mType{ type },
	mVehicleType{ Vehicle::VehicleType::kCar },
	mLeftOrRight{ Random::GetChoice(-1, 1) }
{
	game->GetPlanes().emplace_back(this);
	mMesh = new Mesh{};

	auto renderer = Renderer::Get();
	auto yOffset{ -0.1f };
	if (mType == PlaneType::kGrass)
	{
		mMesh = renderer->GetMesh("Assets/grass.gpmesh");
		yOffset -= 0.1f;
	}
	else if (mType == PlaneType::kRoad)
	{
		mMesh = renderer->GetMesh("Assets/road.gpmesh");
		mVehicleType = static_cast<Vehicle::VehicleType>(Random::GetIntRange(0, 1));
	}
	else if(mType == PlaneType::kRailroad)
	{
		mMesh = renderer->GetMesh("Assets/railroad.gpmesh");
		mVehicleType = Vehicle::VehicleType::kBus;
		yOffset -= 0.1f;
	}
	else
	{
		mMesh = renderer->GetMesh("Assets/water.gpmesh");
		mVehicleType = Vehicle::VehicleType::kLog;
	}

	SetPosition(glm::vec3{ 0.0f, yOffset, -2.0f * game->GetCurStage() });
	GenerateTree();

	mBox = new BoxComponent{ this };
	mBox->SetObjectBox(mMesh->GetBox());
}

void Plane::UpdateActor()
{
	Actor::UpdateActor();

	mCooldown -= dt;
	if (mCooldown < 0)
		GenerateVehicle();

	auto player = mGame->GetPlayer();
	if (player->GetPosition().z + 20.0f < GetPosition().z)
	{
		SetState(State::kDead);
		player->SetZBorder(GetPosition().z - 10.0f);
	}
}

void Plane::Draw(Shader* shader)
{
	shader->SetMatrix4Uniform("uWorld", GetWorldTransform());
	mMesh->GetTexture()->SetActive();
	auto va = mMesh->GetVertexArray();
	va->SetActive();
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}

void Plane::GenerateVehicle()
{
	if (mType != PlaneType::kGrass)
	{
		auto vehicle = new Vehicle{ mGame, mVehicleType };
		const auto& pos = GetPosition();
		vehicle->SetPosition(glm::vec3{ mLeftOrRight * 15.0f, pos.y + 0.1f, pos.z });
		vehicle->SetSpeed(-5.0f);
		if (mLeftOrRight == -1)
			vehicle->SetRotation(180.0f);
		mCooldown = Random::GetFloatRange(1.0f, 3.0f) + vehicle->GetGenTerm();
	}
}

void Plane::GenerateTree()
{
	const auto& pos = GetPosition();
	if (mType == PlaneType::kGrass)
	{
		auto treeNum = Random::GetIntRange(3, 5);
		auto xPos = Random::GetShuffledArray(-6, 6, 0);
		
		for (int i = 0; i < treeNum; ++i)
		{
			auto tree = new Tree{ mGame, Tree::TreeType::kBasic };
			tree->SetPosition(glm::vec3{ xPos[i] * 2.0f, pos.y + 0.2f, pos.z });
		}
	}

	--sLampStride;
	if (sLampStride < 0)
	{
		sLampStride = 8;
		auto tree = new Tree{ mGame, Tree::TreeType::kLamp };
		tree->SetPosition(glm::vec3{ 0.0f, 2.0f, pos.z });
	}
}

int Plane::sLampStride{ 0 };