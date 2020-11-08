#pragma once
#include "scene_actor.h"
class Bunny :
    public SceneActor
{
public:
    Bunny(class Scene* scene);

    void UpdateSceneActor() override;
    void Draw(class Shader* shader) override;

private:
    class Mesh* mMesh;

    glm::vec3 mJumpVel;
    glm::vec3 mGravity;
};

