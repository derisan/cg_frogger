#include "earth.h"

#include <GL/glew.h>

#include "game.h"
#include "random.h"
#include "renderer.h"
#include "mesh.h"
#include "vertexarray.h"
#include "shader.h"
#include "texture.h"
#include "player.h"

Earth::Earth(Game* game)
	: Actor{ game },
	mAngle{ 0.0f }
{
	mMesh = new Mesh{};
	auto renderer = Renderer::Get();

	mMesh = renderer->GetMesh("Assets/earth.gpmesh");

	SetScale(0.025f);
}

void Earth::UpdateActor()
{
	Actor::UpdateActor();

	mAngle += dt;

	auto pos = mGame->GetPlayer()->GetPosition();

	pos.x = -25.0f;
	pos.y = -10.0f;
	pos.z -= 8.0f;

	SetPosition(pos);
	SetRotation(mAngle * 7);
}

void Earth::Draw(Shader* shader)
{
	shader->SetMatrix4Uniform("uWorld", GetWorldTransform());
	mMesh->GetTexture()->SetActive();
	auto va = mMesh->GetVertexArray();
	va->SetActive();
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}