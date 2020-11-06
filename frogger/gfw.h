#pragma once

#include <stack>
#include <unordered_map>
#include <string>

class Gfw
{
public:
	Gfw();

	bool Init(int* argc, char** argv, int w = 1024, int h = 768);

	void Run(unsigned char key);
	void Shutdown();

	void ChangeScene(const std::string& scene);
	void PushScene(const std::string& scene);
	void PopScene();

	// Getters
	bool GetShouldClose() const { return mShouldClose; }
	int GetScrWidth() const { return mScrWidth; }
	int GetScrHeight() const { return mScrHeight; }

	// Setters
	void SetShouldClose(bool value) { mShouldClose = value; }

private:
	void ProcessInput(unsigned char key);
	void Update();
	void Draw();

	std::stack<class Scene*> mScenes;
	std::unordered_map<std::string, class Scene*> mScenesMap;

	int mScrWidth;
	int mScrHeight;

	bool mShouldClose;
};