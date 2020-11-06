#include "loading_actor.h"

#include <GL/glew.h>

#include "renderer.h"
#include "vertexarray.h"
#include "texture.h"
#include "shader.h"

LoadingActor::LoadingActor(Scene* scene)
	: SceneActor{ scene },
	mAngle{ 0.0f },
	mVertexArray{ nullptr },
	mTexture{ nullptr }
{
	mVertexArray = Renderer::GetSpriteVertexArray();
	mTexture = Renderer::Get()->GetTexture("Assets/loading.png");

	SetScale(0.5f);
	SetAxis(glm::vec3{ 0.0f, 0.0f, 1.0f });
}

void LoadingActor::UpdateSceneActor()
{
	SceneActor::UpdateSceneActor();

	mAngle += dt;

	SetRotation(mAngle * 65);
}

void LoadingActor::Draw(Shader* shader)
{
	shader->SetMatrix4Uniform("uWorld", GetWorldTransform());
	mTexture->SetActive();
	mVertexArray->SetActive();
	glDrawElements(GL_TRIANGLES, mVertexArray->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}