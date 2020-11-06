#include "start_actor.h"

#include <GL/glew.h>

#include "scene.h"
#include "vertexarray.h"
#include "texture.h"
#include "shader.h"
#include "renderer.h"

StartActor::StartActor(Scene* scene)
	: SceneActor{ scene },
	mVertexArray{ nullptr },
	mTexture{ nullptr }
{
	mVertexArray = Renderer::GetSpriteVertexArray();
	mTexture = Renderer::Get()->GetTexture("Assets/pressenter.jpg");
}

void StartActor::Draw(Shader* shader)
{
	shader->SetMatrix4Uniform("uWorld", GetWorldTransform());
	mTexture->SetActive();
	mVertexArray->SetActive();
	glDrawElements(GL_TRIANGLES, mVertexArray->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}