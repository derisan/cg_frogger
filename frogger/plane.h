#pragma once
#include "object.h"
class Plane :
    public Actor
{
public:
    Plane(class Game* game);
    ~Plane();

    void UpdateObject() override;
    void Draw(class Shader* shader);
    void Load();

private:
    class VertexArray* mVertexArray;
};

