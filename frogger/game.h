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
	bool RemoveNearVehicle(class Actor* actor);

	void LaunchSkills();
	void SetPhongUniforms();
	bool IsWin();

	// Getters
	std::vector<class Vehicle*>& GetVehicles() { return mVehicles; }
	std::vector<class Vehicle*>& GetNearVehicles() { return mNearVehicles; }
	std::vector<class Plane*>& GetPlanes() { return mPlanes; }
	std::vector<class Tree*>& GetTrees() { return mTrees; }
	class Player* GetPlayer() { return mPlayer; }
	int GetCurStage() const { return mCurStage; }
	bool GetShouldCloseGame() const { return mShouldCloseGame; }
	int GetPlayerLives() const;

	// Setters
	void SetViewMatrix(const glm::mat4& view) { mView = view; }
	void SetCameraPosition(const glm::vec3& pos) { mCameraPos = pos; }
	void SetShouldCloseGame(bool value) { mShouldCloseGame = value; }

private:
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class Vehicle*> mVehicles;
	std::vector<class Vehicle*> mNearVehicles;
	std::vector<class Plane*> mPlanes;
	std::vector<class Tree*> mTrees;
	std::vector<int> mStage;

	class Scene* mScene;
	class Renderer* mRenderer;
	class Shader* mPhongShader;
	class Player* mPlayer;

	glm::mat4 mView;
	glm::vec3 mCameraPos;

	bool mIsUpdating;
	bool mShouldCloseGame;

	int mCurStage;
	float mSkillCooldown;

	const float dt{ 16.0f / 1000.0f };
};

