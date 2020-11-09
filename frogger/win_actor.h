#pragma once
#include "scene_actor.h"
class WinActor :
    public SceneActor
{
public:
    WinActor(class Scene* scene);

    void Draw(class Shader* shader) override;

private:
    class VertexArray* mVertexArray;
    class Texture* mTexture;
};

