#include "skill.h"

#include <GL/glew.h>

#include "game.h"
#include "renderer.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "vertexarray.h"
#include "box_component.h"
#include "vehicle.h"
#include "particle.h"
#include "sound_engine.h"

Skill::Skill(Game* game, Vehicle* vehicle, SkillType type)
	: Actor{ game },
	mMesh{ nullptr },
	mBox{ nullptr },
	mTarget{ vehicle },
	mSpeed{ -7.0f },
	mType{ type },
	mCollides{ false }
{
	mMesh = new Mesh{};

	auto renderer = Renderer::Get();
	if (mType == SkillType::kPenguin)
	{
		mMesh = renderer->GetMesh("Assets/penguin.gpmesh");
		SetScale(0.1f);
	}
	
	mBox = new BoxComponent{ this };
	mBox->SetObjectBox(mMesh->GetBox());

	const auto& vPos = vehicle->GetPosition();
	SetPosition(glm::vec3{ vPos.x, 5.0f, vPos.z });

	SoundEngine::Get()->Play("penguin_drop.wav");
}

void Skill::UpdateActor()
{
	Actor::UpdateActor();

	auto pos = GetPosition();
	pos.y += mSpeed * dt;

	const auto& vehicleBox = mTarget->GetBox()->GetWorldBox();
	if (!mCollides && Intersects(mBox->GetWorldBox(), vehicleBox))
	{
		SoundEngine::Get()->Stop("penguin_drop.wav");
		SoundEngine::Get()->Play("penguin_boom.wav");
		
		mCollides = true;
		mTarget->SetState(State::kDead);
		for (int i = 0; i < 5; ++i)
			new Particle{ mGame, mTarget->GetPosition() };
	}

	if (pos.y < -1.0f)
		SetState(State::kDead);

	SetPosition(pos);
}

void Skill::Draw(Shader* shader)
{
	shader->SetMatrix4Uniform("uWorld", GetWorldTransform());
	mMesh->GetTexture()->SetActive();
	auto va = mMesh->GetVertexArray();
	va->SetActive();
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}