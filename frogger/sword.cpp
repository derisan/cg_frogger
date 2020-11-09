#include "sword.h"

#include <GL/glew.h>

#include "game.h"
#include "renderer.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "vertexarray.h"
#include "box_component.h"
#include "player.h"
#include "particle.h"
#include "sound_engine.h"
#include "vehicle.h"

Sword::Sword(Game* game)
	: Skill{ game },
	mMesh{ nullptr },
	mSpeed{ -10.0f },
	mLifetime{ 0.0f }
{
	mMesh = new Mesh{};

	auto renderer = Renderer::Get();
	mMesh = renderer->GetMesh("Assets/sword.gpmesh");
	SetScale(0.5f);

	mBox = new BoxComponent{ this };
	mBox->SetObjectBox(mMesh->GetBox());


	const auto& player = game->GetPlayer();
	auto pos = player->GetPosition();
	pos += player->GetForward() * 2.0f;
	pos.x -= 3.0f;
	pos.y = 1.0f;

	SetPosition(pos);
	SetRotation(-45.0f);
}

void Sword::UpdateActor()
{
	Skill::UpdateActor();

	mLifetime += dt;
	if (mLifetime > 3.0f)
	{
		SetState(State::kDead);
		return;
	}

	const auto& swordBox = mBox->GetWorldBox();
	for (auto vehicle : mGame->GetVehicles())
	{
		if (vehicle->GetType() == Vehicle::VehicleType::kLog || vehicle->GetState() == State::kDead)
			continue;

		const auto& vehicleBox = vehicle->GetBox()->GetWorldBox();
		if (Intersects(swordBox, vehicleBox))
		{
			SoundEngine::Get()->Play("slashing.wav");

			vehicle->SetState(State::kDead);
			for (int i = 0; i < 5; ++i)
				new Particle{ mGame, vehicle->GetPosition() };
		}
	}
	auto pos = GetPosition();
	pos.z += mSpeed * dt;

	SetPosition(pos);
}

void Sword::Draw(Shader* shader)
{
	shader->SetMatrix4Uniform("uWorld", GetWorldTransform());
	mMesh->GetTexture()->SetActive();
	auto va = mMesh->GetVertexArray();
	va->SetActive();
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}

