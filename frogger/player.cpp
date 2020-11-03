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
    mMesh{ nullptr }
{
    mMesh = new Mesh{};
    mMesh->Load("Assets/chr_knight.gpmesh", game->GetRenderer());
}

Player::~Player()
{

}

void Player::UpdateActor()
{
	Actor::UpdateActor();

}

void Player::ActorInput(unsigned char key)
{
    auto pos = GetPosition();
    auto forward = GetForward();
    auto right = GetRight();

    switch (key)
    {
        case 'w': case 'W':
            pos += forward * 0.1f;
            break;
        case 's': case 'S':
            pos -= forward * 0.1f;
            break;
        case 'a': case 'A':
            pos -= right * 0.1f;
            break;
        case 'd': case 'D':
            pos += right * 0.1f;
            break;
    }
    SetPosition(pos);
}

void Player::Draw(Shader* shader)
{
    shader->SetMatrix4Uniform("uWorld", GetWorldTransform());
    mMesh->GetTexture()->SetActive();
    auto va = mMesh->GetVertexArray();
    va->SetActive();
    glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}