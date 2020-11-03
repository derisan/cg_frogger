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
	mType{ type }
{
	mMesh = new Mesh{};

	if (mType == VehicleType::kCar)
		mMesh = game->GetRenderer()->GetMesh("Assets/car.gpmesh");
	else if (mType == VehicleType::kTruck)
		mMesh = game->GetRenderer()->GetMesh("Assets/truck.gpmesh");
	else
		mMesh = game->GetRenderer()->GetMesh("Assets/train.gpmesh");

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



