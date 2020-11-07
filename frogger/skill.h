#pragma once
#include "actor.h"
class Skill :
    public Actor
{
public:
    enum class SkillType
    {
        kPenguin
    };

    Skill(class Game* game, class Vehicle* vehicle, SkillType type = SkillType::kPenguin);

    void UpdateActor() override;
    void Draw(class Shader* shader) override;

    // Getters
    SkillType GetType() const { return mType; }
    class BoxComponent* GetBox() const { return mBox; }

    // Setters
    void SetSpeed(float speed) { mSpeed = speed; }

private:
    class Mesh* mMesh;
    class BoxComponent* mBox;
    class Vehicle* mTarget;

    float mSpeed;
    SkillType mType;

    bool mCollides;
};

