#include "player.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "game.h"
#include "vertexarray.h"
#include "shader.h"
#include "texture.h"
#include "mesh.h"
#include "renderer.h"

Player::Player(Game* game)
    : Actor{ game },
    mMesh{ nullptr }
{
    mMesh = new Mesh{};
    mMesh = game->GetRenderer()->GetMesh("Assets/chr_knight.gpmesh");
}

Player::~Player()
{

}

void Player::UpdateActor()
{
	Actor::UpdateActor();

    auto actorPos = GetPosition();
    auto cameraPos = glm::vec3{ actorPos.x , actorPos.y + 1.0f, actorPos.z + 3.0f };
    auto cameraTarget = GetForward() * 2.0f;
    auto cameraUp = glm::vec3{ 0.0f, 1.0f, 0.0f };

    glm::mat4 view = lookAt(cameraPos, cameraPos + cameraTarget, cameraUp);

    mGame->GetRenderer()->SetViewMatrix(view);
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