#pragma once

#include <unordered_map>

class Renderer
{
public:
	Renderer(class Game* game);
	~Renderer();

	bool Init(int* argc, char** argv, int w = 1024, int h = 768);
	void Shutdown();

	void Draw();

	class Texture* GetTexture(const std::string& file);
	class Mesh* GetMesh(const std::string& file);

private:
	bool LoadShaders();

	std::unordered_map<std::string, class Texture*> mTextures;
	std::unordered_map<std::string, class Mesh*> mMeshes;

	class Game* mGame;
	class Shader* mMeshShader;

	int mScrWidth;
	int mScrHeight;
};

