#pragma once
#include "scene_actor.h"
class SParticle :
    public SceneActor
{
public:
    enum class SParticleType
    {
        kFireball,
        kCarrot
    };

    SParticle(class Scene* scene, const glm::vec3 target, SParticleType type = SParticleType::kFireball);

    void UpdateSceneActor() override;
    void Draw(class Shader* shader) override;

private:
    class Mesh* mMesh;

    const glm::vec3 mGravity;
    glm::vec3 mVelocity;
    float mLife;

    SParticleType mType;
};

