#pragma once

#include <unordered_map>

#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer(class Game* game);
	
	bool Init(int* argc, char** argv, int w = 1024, int h = 768);
	void Shutdown();

	void Draw();

	// Getters
	class Texture* GetTexture(const std::string& file);
	class Mesh* GetMesh(const std::string& file);

	// Setters
	void SetViewMatrix(const glm::mat4& view) { mView = view; }

private:
	bool LoadShaders();

	std::unordered_map<std::string, class Texture*> mTextures;
	std::unordered_map<std::string, class Mesh*> mMeshes;

	class Game* mGame;
	class Shader* mMeshShader;

	glm::mat4 mView;

	int mScrWidth;
	int mScrHeight;
};

