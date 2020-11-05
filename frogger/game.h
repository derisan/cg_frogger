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

	void CollisionCheck();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	bool RemoveVehicle(class Actor* actor);
	bool RemovePlane(class Actor* actor);
	bool RemoveTree(class Actor* actor);

	// Getters
	bool GetShouldCloseWindow() const { return mShouldCloseWindow; }
	const std::vector<class Actor*>& GetActors() const { return mActors; }
	std::vector<class Vehicle*>& GetVehicles() { return mVehicles; }
	std::vector<class Plane*>& GetPlanes() { return mPlanes; }
	std::vector<class Tree*>& GetTrees() { return mTrees; }
	class Renderer* GetRenderer() {	return mRenderer; }
	class Player* GetPlayer() { return mPlayer; }
	int GetCurStage() const { return mCurStage; }

	// Setters
	void SetShouldCloseWindow(bool value) { mShouldCloseWindow = value; }

private:
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class Vehicle*> mVehicles;
	std::vector<class Plane*> mPlanes;
	std::vector<class Tree*> mTrees;
	std::vector<int> mStage;

	bool mShouldCloseWindow;
	bool mShouldPause;
	bool mIsUpdating;

	class Renderer* mRenderer;
	class Player* mPlayer;
	class SoundEngine* mSoundEngine;

	int mCurStage;
};

