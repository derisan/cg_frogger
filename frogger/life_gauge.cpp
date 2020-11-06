#include "life_gauge.h"

#include <GL/glew.h>

#include "renderer.h"
#include "vertexarray.h"
#include "texture.h"
#include "shader.h"
#include "scene.h"

LifeGauge::LifeGauge(Scene* scene)
	: SceneActor{ scene },
	mVertexArray{ nullptr },
	mTexture{ nullptr }
{
	mVertexArray = Renderer::GetSpriteVertexArray();
	mTexture = Renderer::Get()->GetTexture("Assets/life.png");
	SetScale(0.2f);
}

void LifeGauge::UpdateSceneActor()
{
	SceneActor::UpdateSceneActor();


}

void LifeGauge::Draw(Shader* shader)
{
	shader->SetMatrix4Uniform("uWorld", GetWorldTransform());
	mTexture->SetActive();
	mVertexArray->SetActive();
	glDrawElements(GL_TRIANGLES, mVertexArray->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}