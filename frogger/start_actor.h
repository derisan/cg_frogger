#pragma once
#include "scene_actor.h"
class StartActor:
    public SceneActor
{
public:
    StartActor(class Scene* scene);

    void Draw(class Shader* shader) override;

private:
    class VertexArray* mVertexArray;
    class Texture* mTexture;
};

