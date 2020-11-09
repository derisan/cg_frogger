#include "win_actor.h"

#include <GL/glew.h>

#include "scene.h"
#include "vertexarray.h"
#include "texture.h"
#include "shader.h"
#include "renderer.h"

WinActor::WinActor(Scene* scene)
	: SceneActor{ scene },
	mVertexArray{ nullptr },
	mTexture{ nullptr }
{
	auto renderer = Renderer::Get();

	mVertexArray = renderer->GetSpriteVertexArray();
	mTexture = renderer->GetTexture("Assets/youwin.png");
}

void WinActor::Draw(Shader* shader)
{
	shader->SetMatrix4Uniform("uWorld", GetWorldTransform());
	mTexture->SetActive();
	mVertexArray->SetActive();
	glDrawElements(GL_TRIANGLES, mVertexArray->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}