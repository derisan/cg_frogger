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
	mTexture{ nullptr },
	mLives{ 3 }
{
	mVertexArray = Renderer::GetSpriteVertexArray();
	mTexture = Renderer::Get()->GetTexture("Assets/life.png");
	SetScale(0.3f);
	SetPosition(glm::vec3{ -0.7f, 0.7f, 0.0f });
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