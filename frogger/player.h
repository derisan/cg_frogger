#pragma once
#include "actor.h"

class Player :
    public Actor
{
public:
    Player(class Game* game);

    void UpdateActor() override;
    void ActorInput(unsigned char key) override;
    void Draw(class Shader* shader) override;

    void PlayJumpSound();

    // Getters
    class BoxComponent* GetBox() const { return mBox; }

    // Setters
    void SetZBorder(float z) { mBorder.z = z; }

private:
    class Mesh* mMesh;
    class BoxComponent* mBox;

    struct Border
    {
        const float x;
        float z;
    };
    Border mBorder;

    const float mGravity;
};

