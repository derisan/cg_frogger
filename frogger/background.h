#pragma once

#include "scene_actor.h"

#include <string>

class Background
{
public:
    Background(const std::string& file);
    ~Background();

    void Draw();
    void Load();

private:
    class Texture* mTexture;
    unsigned int mVertexArray;
    unsigned int mVertexBuffer;
    unsigned int mIndexBuffer;
    class Shader* mShader;
};

