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

    class BoxComponent* GetBox() const { return mBox; }

private:
    class Mesh* mMesh;
    class BoxComponent* mBox;
};

