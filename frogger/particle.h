#pragma once
#include "actor.h"
class Particle :
    public Actor
{
public:
    enum class ParticleType
    {
        kFireball,
        kCarrot
    };

    Particle(class Game* game, const glm::vec3 target, ParticleType type = ParticleType::kFireball);

    void UpdateActor() override;
    void Draw(class Shader* shader) override;

private:
    class Mesh* mMesh;

    const glm::vec3 mGravity;
    glm::vec3 mVelocity;
    float mLife;

    ParticleType mType;
};

