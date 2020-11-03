#pragma once
#include "actor.h"
class Plane :
    public Actor
{
public:
    enum class Type
    {
        kGrass,
        kRoad,
        kRail
    };

    Plane(class Game* game, Type type = Type::kGrass);
    
    void UpdateActor() override;
    void Draw(class Shader* shader) override;

    Type GetType() const { return mType; }

private:
    class Mesh* mMesh;

    Type mType;
};

