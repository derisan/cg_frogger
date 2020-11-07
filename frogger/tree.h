#pragma once
#include "actor.h"

class Tree :
    public Actor
{
public:
    enum class TreeType
    {
        kBasic,
        kLamp
    };

    Tree(class Game* game, TreeType type);

    void UpdateActor() override;
    void Draw(class Shader* shader) override;

    // Getters
    TreeType GetType() const { return mType; }
    class BoxComponent* GetBox() const { return mBox; }

private:
    class Mesh* mMesh;
    class BoxComponent* mBox;

    TreeType mType;
};

