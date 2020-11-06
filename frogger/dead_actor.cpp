#include "dead_actor.h"

#include <GL/glew.h>

#include "scene.h"
#include "vertexarray.h"
#include "texture.h"
#include "shader.h"
#include "renderer.h"

DeadActor::DeadActor(Scene* scene)
	: SceneActor{ scene },
	mVertexArray{ nullptr },
	mTexture{ nullptr }
{
	mVertexArray = Renderer::GetSpriteVertexArray();
	mTexture = Renderer::Get()->GetTexture("Assets/deadscreen.png");
}

void DeadActor::UpdateSceneActor()
{

}

void DeadActor::Draw(Shader* shader)
{
	shader->SetMatrix4Uniform("uWorld", GetWorldTransform());
	mTexture->SetActive();
	mVertexArray->SetActive();
	glDrawElements(GL_TRIANGLES, mVertexArray->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}