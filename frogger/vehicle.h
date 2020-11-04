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
        kTrain,
        kLog
    };

    Vehicle(class Game* game, VehicleType type);
    
    void UpdateActor() override;
    void Draw(class Shader* shader) override;

    // Getters
    VehicleType GetType() const { return mType; }
    class BoxComponent* GetBox() const { return mBox; }
    float GetGenTerm() const { return mGenTerm; }
    
    // Setters
    void SetSpeed(float speed) { mSpeed = speed; }
    void SetGenTerm(float gen) { mGenTerm = gen; }
    
private:
    class Mesh* mMesh;
    class BoxComponent* mBox;

    float mSpeed;
    float mGenTerm;

    VehicleType mType;
};

