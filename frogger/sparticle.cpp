#include "sParticle.h"

#include <GL/glew.h>

#include "scene.h"
#include "random.h"
#include "renderer.h"
#include "mesh.h"
#include "vertexarray.h"
#include "shader.h"
#include "texture.h"

SParticle::SParticle(Scene* scene, const glm::vec3 target, SParticleType type)
	: SceneActor{ scene},
	mGravity{ 0.0f, -5.0f, 0.0f },
	mVelocity{ Random::GetFloatRange(-1.0f, 1.0f), Random::GetFloatRange(3.0f, 5.0f),
	Random::GetFloatRange(-1.0f, 1.0f) },
	mLife{ Random::GetFloatRange(0.5, 2.5f) },
	mType{ type }
{
	mMesh = new Mesh{};
	auto renderer = Renderer::Get();

	if (mType == SParticleType::kFireball)
	{
		mMesh = renderer->GetMesh("Assets/fireball.gpmesh");
		SetScale(0.075f);
	}
	else if (mType == SParticleType::kCarrot)
	{
		mMesh = renderer->GetMesh("Assets/carrot.gpmesh");
		SetScale(0.075f);
		SetRotation(Random::GetFloatRange(0.0f, 360.0f));
	}

	auto pos = target + Random::GetVec3(-0.5f, 0.5f);
	SetPosition(pos);
}

void SParticle::UpdateSceneActor()
{
	if (mLife < 0)
	{
		SetState(SceneActor::State::kDead);
		return;
	}

	mLife -= dt;
	mVelocity += mGravity * dt;
	auto pos = GetPosition() + mVelocity * dt;
	SetPosition(pos);
}

void SParticle::Draw(Shader* shader)
{
	if (GetState() == State::kDead)
		return;


	shader->SetMatrix4Uniform("uWorld", GetWorldTransform());
	mMesh->GetTexture()->SetActive();
	auto va = mMesh->GetVertexArray();
	va->SetActive();
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}