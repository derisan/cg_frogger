#pragma once
#include "actor.h"
class Earth :
    public Actor
{
public:
    Earth(class Game* game);

    void UpdateActor() override;
    void Draw(class Shader* shader);

private:
    class Mesh* mMesh;
 
    float mAngle;
};

