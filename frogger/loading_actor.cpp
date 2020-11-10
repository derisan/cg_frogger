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
	mTexture = Renderer::Get()->GetTexture("Assets/white.png");
}

void LoadingActor::UpdateSceneActor()
{
	SceneActor::UpdateSceneActor();
}

void LoadingActor::Draw(Shader* shader)
{
	shader->SetMatrix4Uniform("uWorld", GetWorldTransform());
	mTexture->SetActive();
	mVertexArray->SetActive();
	glDrawElements(GL_TRIANGLES, mVertexArray->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}