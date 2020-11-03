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
    
    void UpdateActor() override;
    void Draw(class Shader* shader) override;

    // Getters
    Type GetType() const { return mType; }
    class BoxComponent* GetBox() const { return mBox; }

    // Setters
    void SetSpeed(float speed) { mSpeed = speed; }

private:
    class Mesh* mMesh;
    class BoxComponent* mBox;

    float mSpeed;

    Type mType;
};

