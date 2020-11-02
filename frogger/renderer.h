#pragma once
class Renderer
{
public:
	Renderer(class Game* game);
	~Renderer() = default;

	bool Init(int* argc, char** argv, int w = 1024, int h = 768);
	void Shutdown();

	void Draw();

	// Getters 
	int GetScreenWidth() const { return mScrWidth; }
	int GetScreenHeight() const { return mScrHeight; }
private:
	bool LoadShaders();

	class Game* mGame;
	class Shader* mLightCubeShader;
	class Shader* mMeshShader;

	int mScrWidth;
	int mScrHeight;
};

