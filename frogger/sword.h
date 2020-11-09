#pragma once
#include "skill.h"
class Sword :
    public Skill
{
public:
    Sword(class Game* game);

    void UpdateActor() override;
    void Draw(class Shader* shader) override;

private:
    class Mesh* mMesh;
    class BoxComponent* mBox;

    float mSpeed;
    float mLifetime;
};

