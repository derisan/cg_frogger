#pragma once
#include "actor.h"
class Particle :
    public Actor
{
public:
    Particle(class Game* game, const glm::vec3 target);

    void UpdateActor() override;
    void Draw(class Shader* shader) override;

private:
    class Mesh* mMesh;

    const glm::vec3 mGravity;
    glm::vec3 mVelocity;
    float mLife;
};

