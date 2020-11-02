#pragma once
#include "object.h"
class Plane :
    public Object
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

