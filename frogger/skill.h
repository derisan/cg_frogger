#pragma once
#include "actor.h"
class Skill :
    public Actor
{
public:
    enum class SkillType
    {
        kPenguin,
        kSword
    };

    Skill(class Game* game);

    void UpdateActor() override { };
    void Draw(class Shader* shader) override { };

    // Getters
    SkillType GetType() const { return mType; }

private:
    SkillType mType;
};

