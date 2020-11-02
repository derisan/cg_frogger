#pragma once
#include "actor.h"

class Player :
    public Actor
{
public:
    Player(class Game* game);
    ~Player();

    void UpdateActor() override;
    void ActorInput(unsigned char key) override;
    void Draw(class Shader* shader) override;

private:
    class VertexArray* mVertexArray;
    class Mesh* mMesh;
};

