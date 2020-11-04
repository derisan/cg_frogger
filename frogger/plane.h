#pragma once

#include "actor.h"
#include "vehicle.h"

class Plane :
    public Actor
{
public:
    enum class PlaneType
    {
        kGrass,
        kRoad,
        kRailroad
    };

    Plane(class Game* game, PlaneType type = PlaneType::kGrass);
    
    void UpdateActor() override;
    void Draw(class Shader* shader) override;

    // Getters
    class BoxComponent* GetBox() const { return mBox; }
    PlaneType GetType() const { return mType; }

private:
    class Mesh* mMesh;
    class BoxComponent* mBox;
    float mCooldown;

    PlaneType mType;
    Vehicle::VehicleType mVehicleType;
    
    int mLeftOrRight;
};

