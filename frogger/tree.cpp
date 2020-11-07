#include "tree.h"

#include <GL/glew.h>

#include "game.h"
#include "renderer.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "vertexarray.h"
#include "box_component.h"
#include "player.h"

Tree::Tree(Game* game, TreeType type)
	: Actor{ game },
	mType{ type }
{
	game->GetTrees().emplace_back(this);

	mMesh = new Mesh{};
	auto renderer = Renderer::Get();
	if (mType == TreeType::kBasic)
	{
		mMesh = renderer->GetMesh("Assets/basic_tree.gpmesh");
		SetScale(0.1f);
	}
	else if (mType == TreeType::kLamp)
	{
		mMesh = renderer->GetMesh("Assets/pumpkin.gpmesh");
		SetScale(0.75f);
		SetRotation(180.0f);
	}

	mBox = new BoxComponent{ this };
	mBox->SetObjectBox(mMesh->GetBox());
}

void Tree::UpdateActor()
{
	Actor::UpdateActor();

	auto player = mGame->GetPlayer();
	if (player->GetPosition().z + 20.0f < GetPosition().z)
		SetState(State::kDead);
}

void Tree::Draw(Shader* shader)
{
	shader->SetMatrix4Uniform("uWorld", GetWorldTransform());
	mMesh->GetTexture()->SetActive();
	auto va = mMesh->GetVertexArray();
	va->SetActive();
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}