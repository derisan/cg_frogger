#pragma once
#include "skill.h"
class Bomb :
    public Skill
{
public:
    Bomb(class Game* game, class Vehicle* vehicle);

    void UpdateActor() override;
    void Draw(class Shader* shader) override;

    class BoxComponent* GetBox() const { return mBox; }

private:
    class Mesh* mMesh;
    class BoxComponent* mBox;
    class Vehicle* mTarget;

    float mSpeed;
    bool mCollides;
};

