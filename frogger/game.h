#pragma once

#include <vector>

#include <glm/glm.hpp>

class Game
{
public:
	Game(int w, int h);
	~Game() = default;

	bool Init(int* argc, char** argv);
	void LoadData();
	void Shutdown();

	void ProcessKeyboardInput(unsigned char key);
	void ProcessMouseInput(int button, int state, int x, int y) { };
	void Update();
	void Draw();

	void AddActor(class Actor* obj);
	void RemoveActor(class Actor* obj);

	// Getters
	bool GetShouldCloseWindow() const { return mShouldCloseWindow; }
	class Renderer* GetRenderer() { return mRenderer; }

	// Setters
	void SetShouldCloseWindow(bool value) { mShouldCloseWindow = value; }

private:
	std::vector<class Actor*> mActors;

	bool mShouldCloseWindow;
	bool mShouldPause;

	class Renderer* mRenderer;
};

