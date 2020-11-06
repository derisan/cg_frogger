#pragma once

#include <vector>

#include <glm/glm.hpp>

class Game
{
public:
	Game(class Scene* scene);

	bool Init();
	void Shutdown();

	void ProcessInput(unsigned char key);
	void Update();
	void Draw();

	void CreateMap();
	void CollisionCheck();
	
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	bool RemoveVehicle(class Actor* actor);
	bool RemovePlane(class Actor* actor);
	bool RemoveTree(class Actor* actor);

	// Getters
	std::vector<class Vehicle*>& GetVehicles() { return mVehicles; }
	std::vector<class Plane*>& GetPlanes() { return mPlanes; }
	std::vector<class Tree*>& GetTrees() { return mTrees; }
	class Player* GetPlayer() { return mPlayer; }
	int GetCurStage() const { return mCurStage; }

	// Setters
	void SetViewMatrix(const glm::mat4& view) { mView = view; }

private:
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class Vehicle*> mVehicles;
	std::vector<class Plane*> mPlanes;
	std::vector<class Tree*> mTrees;
	std::vector<int> mStage;

	class Scene* mScene;
	class Renderer* mRenderer;
	class Shader* mMeshShader;
	class Player* mPlayer;

	glm::mat4 mView;

	bool mIsUpdating;
	int mCurStage;
};

