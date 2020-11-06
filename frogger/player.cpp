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
#include "tree.h"
#include "vehicle.h"

Player::Player(Game* game)
    : Actor{ game },
    mMesh{ nullptr },
    mBox{ nullptr },
    mBorder{ 12.0f, 0.0f },
    mGravity{ 0.05f },
    mPrevMovement{ 0.0f },
    mLives{ 3 },
    mInvincible{ 0.0f }
{
    mMesh = new Mesh{};

    auto renderer = Renderer::Get();
    mMesh = renderer->GetMesh("Assets/bird.gpmesh");
    SetScale(0.1f);
    
    mBox = new BoxComponent{ this };
    mBox->SetObjectBox(mMesh->GetBox());
}

void Player::UpdateActor()
{
	Actor::UpdateActor();

    mInvincible -= dt;

    const auto& actorPos = GetPosition();
    auto cameraPos = glm::vec3{ actorPos.x + 3.0f, 15.0f, actorPos.z + 10.0f };
    const auto& cameraTarget = actorPos;
    auto cameraUp = glm::vec3{ 0.0f, 1.0f, 0.0f };

    glm::mat4 view = lookAt(cameraPos, cameraTarget, cameraUp);

    mGame->SetViewMatrix(view);

    auto pos = GetPosition();
    pos.y -= mGravity;

    if (pos.y < -0.5f)
        Die();

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

    mPrevMovement = glm::vec3{ 0.0f };
    switch (key)
    {
        case 'w': case 'W':
            PlayJumpSound();
            pos += forward * 2.0f;
            mPrevMovement = -(forward * 2.0f);
            break;
        case 's': case 'S':
            PlayJumpSound();
            pos -= forward * 2.0f;
            mPrevMovement = (forward * 2.0f);
            break;
        case 'a': case 'A':
            PlayJumpSound();
            pos -= right * 1.2f;
            mPrevMovement = (right * 1.2f);
            break;
        case 'd': case 'D':
            PlayJumpSound();
            pos += right * 1.2f;
            mPrevMovement = -(right * 1.2f);
            break;
    }

    if (pos.x > mBorder.x)
        pos.x = mBorder.x;
    else if (pos.x < -mBorder.x)
        pos.x = -mBorder.x;

    if (pos.z > mBorder.z)
        pos.z = mBorder.z;

    SetPosition(pos);

    // This is not a proper way. But there's no other way.
    mBox->OnUpdateWorldTransform();
    const auto& playerBox = mBox->GetWorldBox();
    for (auto tree : mGame->GetTrees())
    {
        if (tree->GetState() != Actor::State::kActive)
            continue;

        const auto& treeBox = tree->GetBox()->GetWorldBox();
        if (Intersects(playerBox, treeBox))
        {
            auto pos = GetPosition() + mPrevMovement;
            SetPosition(pos);
        }
    }
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
    SoundEngine::Get()->Stop("jump.wav");
    SoundEngine::Get()->Play("jump.wav", 10.0f);
}

void Player::CollideWithLog(Vehicle* log)
{
    auto pos = GetPosition();
    pos.x = log->GetPosition().x;
    pos.y = 0.0f;
    SetPosition(pos);
}

void Player::NotToFall()
{
    auto pos = GetPosition();
    pos.y = 0.0f;
    SetPosition(pos);
}

void Player::HitByCar()
{
    if (mInvincible < 0)
    {
        --mLives;
        mInvincible = 1.0f;
        SoundEngine::Get()->Play("DragonHawkDeath1.wav");
    }

    if (mLives == 0)
        Die();
}

void Player::Die()
{
    mGame->SetShouldCloseGame(true);
}