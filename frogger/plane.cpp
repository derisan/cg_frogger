#include "plane.h"

#include <GL/glew.h>

#include "game.h"
#include "renderer.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "vertexarray.h"
#include "random.h"
#include "player.h"

Plane::Plane(Game* game, PlaneType type)
	: Actor{ game },
	mMesh{ nullptr },
	mCooldown{ Random::GetFloatRange(0.0f, 0.5f) },
	mType{ type },
	mVehicleType{ Vehicle::VehicleType::kCar },
	mLeftOrRight{ Random::GetChoice(-1, 1) }
{
	mMesh = new Mesh{};

	if (mType == PlaneType::kGrass)
		mMesh = game->GetRenderer()->GetMesh("Assets/grass.gpmesh");
	else if (mType == PlaneType::kRoad)
		mMesh = game->GetRenderer()->GetMesh("Assets/road.gpmesh");
	else
		mMesh = game->GetRenderer()->GetMesh("Assets/railroad.gpmesh");

	if (mType == PlaneType::kRoad)
		mVehicleType = static_cast<Vehicle::VehicleType>(Random::GetIntRange(0, 1));
	else if (mType == PlaneType::kRailroad)
		mVehicleType = Vehicle::VehicleType::kTrain;
}

void Plane::UpdateActor()
{
	Actor::UpdateActor();

	mCooldown -= dt;
	if (mType != PlaneType::kGrass && mCooldown + static_cast<int>(mVehicleType) < 0)
	{
		mCooldown = Random::GetFloatRange(1.0f, 2.0f);
		auto vehicle = new Vehicle{ mGame, mVehicleType };
		auto pos = GetPosition();
		vehicle->SetPosition(glm::vec3{ mLeftOrRight * 15.0f, pos.y + 0.1f, pos.z });
		vehicle->SetSpeed(mLeftOrRight * -5.0f);
		mGame->GetVehicles().emplace_back(vehicle);
	}

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



