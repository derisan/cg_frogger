#include "player.h"

#include <iostream>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "game.h"
#include "vertexarray.h"
#include "shader.h"
#include "texture.h"
#include "mesh.h"
#include "renderer.h"
#include "box_component.h"
#include "sound_engine.h"

Player::Player(Game* game)
    : Actor{ game },
    mMesh{ nullptr },
    mBox{ nullptr },
    mBorder{ 12.0f, 0.0f },
    mGravity{ 0.05f }
{
    mMesh = new Mesh{};
    mMesh = game->GetRenderer()->GetMesh("Assets/chr_sword.gpmesh");

    mBox = new BoxComponent{ this };
    mBox->SetObjectBox(mMesh->GetBox());
}

void Player::UpdateActor()
{
	Actor::UpdateActor();

    auto actorPos = GetPosition();
    auto cameraPos = glm::vec3{ actorPos.x, 15.0f, actorPos.z + 10.0f };
    auto cameraTarget = actorPos;
    auto cameraUp = glm::vec3{ 0.0f, 1.0f, 0.0f };

    glm::mat4 view = lookAt(cameraPos, cameraTarget, cameraUp);

    mGame->GetRenderer()->SetViewMatrix(view);

    auto pos = GetPosition();
    pos.y -= mGravity;

    // Need to check twice because of log(vehicle)
    if (pos.x > mBorder.x)
        pos.x = mBorder.x;
    else if (pos.x < -mBorder.x)
        pos.x = -mBorder.x;

    SetPosition(pos);
}

void Player::ActorInput(unsigned char key)
{
    auto pos = GetPosition();
    auto forward = GetForward();
    auto right = GetRight();

    switch (key)
    {
        case 'w': case 'W':
            PlayJumpSound();
            pos += forward * 2.0f;
            break;
        case 's': case 'S':
            PlayJumpSound();
            pos -= forward * 2.0f;
            break;
        case 'a': case 'A':
            PlayJumpSound();
            pos -= right * 2.0f;
            break;
        case 'd': case 'D':
            PlayJumpSound();
            pos += right * 2.0f;
            break;
    }

    if (pos.x > mBorder.x)
        pos.x = mBorder.x;
    else if (pos.x < -mBorder.x)
        pos.x = -mBorder.x;

    if (pos.z > mBorder.z)
        pos.z = mBorder.z;

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

void Player::PlayJumpSound()
{
    SoundEngine::Get()->Stop("jump");
    SoundEngine::Get()->Play("jump");
}