#include "plane.h"

#include <GL/glew.h>

#include "game.h"
#include "renderer.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "vertexarray.h"
#include "random.h"
#include "vehicle.h"

Plane::Plane(Game* game, Type type)
	: Actor{ game },
	mMesh{ nullptr },
	mCooldown{ Random::GetFloatRange(0.5f, 2.5f) },
	mType{ type }
{
	mMesh = new Mesh{};

	if (mType == Type::kGrass)
		mMesh = game->GetRenderer()->GetMesh("Assets/grass.gpmesh");
	else if (mType == Type::kRoad)
		mMesh = game->GetRenderer()->GetMesh("Assets/road.gpmesh");
	else
		mMesh = game->GetRenderer()->GetMesh("Assets/rail.gpmesh");
}

void Plane::UpdateActor()
{
	Actor::UpdateActor();

	mCooldown -= dt;
	if (mType != Type::kGrass && mCooldown < 0)
	{
		mCooldown = Random::GetFloatRange(1.5f, 3.0f);
		auto vehicle = new Vehicle{ mGame, Vehicle::Type::kCar };
		auto pos = GetPosition();
		vehicle->SetPosition(glm::vec3{ -15.0f, pos.y + 0.1f, pos.z });
		mGame->GetVehicles().emplace_back(vehicle);
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



