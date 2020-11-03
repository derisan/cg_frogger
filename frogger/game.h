#pragma once

#include <vector>

class Game
{
public:
	Game();
	~Game() = default;

	bool Init(int* argc, char** argv);
	void LoadData();
	void Shutdown();

	void ProcessKeyboardInput(unsigned char key);
	void ProcessMouseInput(int button, int state, int x, int y) { };
	void Update();
	void Draw();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	// Getters
	bool GetShouldCloseWindow() const { return mShouldCloseWindow; }
	const std::vector<class Actor*>& GetActors() const { return mActors; }
	class Renderer* GetRenderer() {	return mRenderer; }

	// Setters
	void SetShouldCloseWindow(bool value) { mShouldCloseWindow = value; }

private:
	std::vector<class Actor*> mActors;
	std::vector<class Vehicle*> mVehicles;

	bool mShouldCloseWindow;
	bool mShouldPause;

	class Renderer* mRenderer;
	class Player* mPlayer;
};

