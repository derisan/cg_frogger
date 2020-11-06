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
    void CollideWithLog(class Vehicle* log);
    void NotToFall();
    void HitByCar();
    void Die();

    // Getters
    class BoxComponent* GetBox() const { return mBox; }
    const glm::vec3& GetPrevMovement() const { return mPrevMovement; }
    int GetLives() const { return mLives; }

    // Setters
    void SetZBorder(float z) { mBorder.z = z; }
    void SetLives(int lives) { mLives = lives; }

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
    glm::vec3 mPrevMovement;
    int mLives;
    float mInvincible;
};

