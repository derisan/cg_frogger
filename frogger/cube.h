#pragma once
#include "object.h"

class Cube :
    public Object
{
public:
    Cube(class Game* game);
    ~Cube();

    void Update() override;
    void Draw(class Shader* shader);
    void Load();

private:
    class VertexArray* mVertexArray;
};

