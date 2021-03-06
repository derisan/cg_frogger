#include "Vehicle.h"

#include <GL/glew.h>

#include "game.h"
#include "renderer.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "vertexarray.h"
#include "box_component.h"

Vehicle::Vehicle(Game* game, VehicleType type)
	: Actor{ game },
	mMesh{ nullptr },
	mBox{ nullptr },
	mSpeed{ 0.0f },
	mGenTerm{ 0.0f },
	mType{ type }
{
	game->GetVehicles().emplace_back(this);

	mMesh = new Mesh{};

	auto renderer = Renderer::Get();
	if (mType == VehicleType::kCar)
	{
		mMesh = renderer->GetMesh("Assets/car.gpmesh");
		SetScale(0.1f);
		mGenTerm = 0.25f;
	}
	else if (mType == VehicleType::kTruck)
	{
		mMesh = renderer->GetMesh("Assets/truck.gpmesh");
		SetScale(0.15f);
		mGenTerm = 0.5f;
	}
	else if (mType == VehicleType::kBus)
	{
		mMesh = renderer->GetMesh("Assets/bus.gpmesh");
		SetScale(0.1f);
		mGenTerm = 3.0f;
	}
	else
	{
		mMesh = renderer->GetMesh("Assets/log.gpmesh");
		mGenTerm = 0.75f;
	}

	mBox = new BoxComponent{ this };
	mBox->SetObjectBox(mMesh->GetBox());
}
	
void Vehicle::UpdateActor()
{
	Actor::UpdateActor();

	auto right = GetRight();

	auto pos = GetPosition();
	pos += right * mSpeed * dt;

	if (pos.x > 15.0f || pos.x < -15.0f)
		SetState(State::kDead);

	SetPosition(pos);
}

void Vehicle::Draw(Shader* shader)
{
	shader->SetMatrix4Uniform("uWorld", GetWorldTransform());
	mMesh->GetTexture()->SetActive();
	auto va = mMesh->GetVertexArray();
	va->SetActive();
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}



