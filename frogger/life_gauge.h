#pragma once
#include "scene_actor.h"
class LifeGauge:
    public SceneActor
{
public:
    LifeGauge(class Scene* scene);

    void UpdateSceneActor() override;
    void Draw(class Shader* shader) override;

    // Getters
    int GetLives() const { return mLives; }

    // Setters
	void SetLives(int lives) { mLives = lives; }

private:
    class VertexArray* mVertexArray;
    class Texture* mTexture;

    int mLives;
};

