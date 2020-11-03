#pragma once

#include "actor.h"

class Vehicle :
    public Actor
{
public:
    enum class VehicleType
    {
        kCar,
        kTruck,
        kTrain
    };

    Vehicle(class Game* game, VehicleType type);
    
    void UpdateActor() override;
    void Draw(class Shader* shader) override;

    // Getters
    VehicleType GetType() const { return mType; }
    class BoxComponent* GetBox() const { return mBox; }

    // Setters
    void SetSpeed(float speed) { mSpeed = speed; }

private:
    class Mesh* mMesh;
    class BoxComponent* mBox;

    float mSpeed;

    VehicleType mType;
};

