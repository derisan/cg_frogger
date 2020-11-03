#pragma once

#include "actor.h"

class Vehicle :
    public Actor
{
public:
    enum class Type
    {
        kCar,
        kTruck,
        kTrain
    };

    Vehicle(class Game* game, Type type);
    ~Vehicle();

    void UpdateActor() override;
    void Draw(class Shader* shader) override;

    Type GetType() const { return mType; }
    class BoxComponent* GetBox() const { return mBox; }

private:
    class Mesh* mMesh;
    class BoxComponent* mBox;

    Type mType;
};

