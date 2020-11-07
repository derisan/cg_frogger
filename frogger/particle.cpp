#include "particle.h"

#include <GL/glew.h>

#include "game.h"
#include "random.h"
#include "renderer.h"
#include "mesh.h"
#include "vertexarray.h"
#include "shader.h"
#include "texture.h"


Particle::Particle(Game* game, const glm::vec3 target, ParticleType type)
	: Actor{ game },
	mGravity{ 0.0f, -5.0f, 0.0f },
	mVelocity{ Random::GetFloatRange(-1.0f, 1.0f), Random::GetFloatRange(3.0f, 5.0f),
	Random::GetFloatRange(-1.0f, 1.0f) },
	mLife{ Random::GetFloatRange(0.5, 2.5f) },
	mType{ type }
{
	mMesh = new Mesh{};
	auto renderer = Renderer::Get();

	if (mType == ParticleType::kFireball)
	{
		mMesh = renderer->GetMesh("Assets/fireball.gpmesh");
		SetScale(0.075f);
	}
	else if (mType == ParticleType::kCarrot)
	{
		mMesh = renderer->GetMesh("Assets/carrot.gpmesh");
		SetScale(0.075f);
		SetRotation(Random::GetFloatRange(0.0f, 360.0f));
	}

	auto pos = target + Random::GetVec3(-0.5f, 0.5f);
	SetPosition(pos);
}

void Particle::UpdateActor()
{
	if (mLife < 0)
	{
		SetState(Actor::State::kDead);
		return;
	}

	mLife -= dt;
	mVelocity += mGravity * dt;
	auto pos = GetPosition() + mVelocity * dt;
	SetPosition(pos);
}

void Particle::Draw(Shader* shader)
{
	shader->SetMatrix4Uniform("uWorld", GetWorldTransform());
	mMesh->GetTexture()->SetActive();
	auto va = mMesh->GetVertexArray();
	va->SetActive();
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}