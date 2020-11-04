#pragma once

#include <vector>

class Game
{
public:
	Game();

	bool Init(int* argc, char** argv);
	bool LoadData();
	void CreateMap();
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
	std::vector<class Vehicle*>& GetVehicles() { return mVehicles; }
	class Renderer* GetRenderer() {	return mRenderer; }
	class Player* GetPlayer() { return mPlayer; }

	// Setters
	void SetShouldCloseWindow(bool value) { mShouldCloseWindow = value; }

private:
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class Vehicle*> mVehicles;
	std::vector<int> mStage;

	bool mShouldCloseWindow;
	bool mShouldPause;
	bool mIsUpdating;

	class Renderer* mRenderer;
	class Player* mPlayer;
};

