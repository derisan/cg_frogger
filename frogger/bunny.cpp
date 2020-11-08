#include "bunny.h"

#include <GL/glew.h>

#include "game.h"
#include "random.h"
#include "renderer.h"
#include "mesh.h"
#include "vertexarray.h"
#include "shader.h"
#include "texture.h"

Bunny::Bunny(Scene* scene)
	: SceneActor{ scene },
	mMesh{ nullptr },
	mJumpVel{ 0.0f, 10.0f, 0.0f },
	mGravity{ 0.0f, -9.8f, 0.0f }
{
	mMesh = new Mesh{};
	auto renderer = Renderer::Get();

	mMesh = renderer->GetMesh("Assets/bunny.gpmesh");
	SetRotation(-90.0f);
}

void Bunny::UpdateSceneActor()
{
	mJumpVel += mGravity * dt;
	auto pos = GetPosition() + mJumpVel * dt;

	if (pos.y < 0.0f && mJumpVel.y < 0)
	{
		pos.y = 0.0f;
		mJumpVel.y += 20.0f;
	}

	SetPosition(pos);
}

void Bunny::Draw(class Shader* shader)
{
	shader->SetMatrix4Uniform("uWorld", GetWorldTransform());
	mMesh->GetTexture()->SetActive();
	auto va = mMesh->GetVertexArray();
	va->SetActive();
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}