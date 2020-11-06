#pragma once
#include "scene_actor.h"
class LifeGauge:
    public SceneActor
{
public:
    LifeGauge(class Scene* scene);

    void UpdateSceneActor() override;
    void Draw(class Shader* shader) override;

private:
    class VertexArray* mVertexArray;
    class Texture* mTexture;
};

