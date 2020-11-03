#include "Vehicle.h"

#include <GL/glew.h>

#include "game.h"
#include "renderer.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "vertexarray.h"

Vehicle::Vehicle(Game* game, Type type)
	: Actor{ game },
	mMesh{ nullptr },
	mType{ type }
{
	mMesh = new Mesh{};

	if (mType == Type::kCar)
		mMesh = game->GetRenderer()->GetMesh("Assets/car.gpmesh");
	else if (mType == Type::kTruck)
		mMesh = game->GetRenderer()->GetMesh("Assets/truck.gpmesh");
	else
		mMesh = game->GetRenderer()->GetMesh("Assets/train.gpmesh");
}

Vehicle::~Vehicle()
{

}

void Vehicle::UpdateActor()
{
	Actor::UpdateActor();
}

void Vehicle::Draw(Shader* shader)
{
	shader->SetMatrix4Uniform("uWorld", GetWorldTransform());
	mMesh->GetTexture()->SetActive();
	auto va = mMesh->GetVertexArray();
	va->SetActive();
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}



