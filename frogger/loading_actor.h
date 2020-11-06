#pragma once

#include "scene_actor.h"

class LoadingActor :
    public SceneActor
{
public:
    LoadingActor(class Scene* scene);

    void UpdateSceneActor() override;
    void Draw(class Shader* shader) override;

private:
    float mAngle;

    class VertexArray* mVertexArray;
    class Texture* mTexture;
};

