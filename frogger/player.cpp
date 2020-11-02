#include "player.h"

#include <iostream>

#include <GL/glew.h>

#include "game.h"
#include "vertexarray.h"
#include "shader.h"
#include "texture.h"
#include "mesh.h"

Player::Player(Game* game)
    : Actor{ game },
    mVertexArray{ nullptr },
    mMesh{ nullptr }
{
    mMesh = new Mesh{};
    mMesh->Load("Assets/chr_knight.gpmesh", game->GetRenderer());
}

Player::~Player()
{
	delete mVertexArray;
}

void Player::UpdateActor()
{
	Actor::UpdateActor();
}

void Player::ActorInput(unsigned char key)
{
    auto pos = GetPosition();

    switch (key)
    {
        case 'w': case 'W':
            pos.z -= 0.2f;
            break;
        case 's': case 'S':
            pos.z += 0.2f;
            break;
        case 'a': case 'A':
            pos.x -= 0.2f;
            break;
        case 'd': case 'D':
            pos.x += 0.2f;
            break;
    }

    SetPosition(pos);
}

void Player::Draw(Shader* shader)
{
	shader->SetMatrix4Uniform("uWorld", GetWorldTransform());
    //mTexture->SetActive();
    mMesh->GetTexture()->SetActive();
    mVertexArray = mMesh->GetVertexArray();
    mVertexArray->SetActive();
    glDrawElements(GL_TRIANGLES, mVertexArray->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}