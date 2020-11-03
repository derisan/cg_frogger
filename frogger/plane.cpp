#include "plane.h"

#include <GL/glew.h>

#include "game.h"
#include "renderer.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "vertexarray.h"

Plane::Plane(Game* game, Type type)
	: Actor{ game },
	mMesh{ nullptr },
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

Plane::~Plane()
{

}

void Plane::UpdateActor()
{
	Actor::UpdateActor();
}

void Plane::Draw(Shader* shader)
{
	shader->SetMatrix4Uniform("uWorld", GetWorldTransform());
	mMesh->GetTexture()->SetActive();
	auto va = mMesh->GetVertexArray();
	va->SetActive();
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}



