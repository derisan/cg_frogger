#pragma once

#include <vector>

#include <glm/glm.hpp>

class Game
{
public:
	Game(int w, int h);
	~Game() = default;

	bool Init(int* argc, char** argv);
	bool LoadData();
	void Shutdown();

	void ProcessKeyboardInput(unsigned char key);
	void ProcessMouseInput(int button, int state, int x, int y);
	void Update();
	void Draw();

	void AddObj(class Object* obj);
	void RemoveObj(class Object* obj);

	// Getters
	bool GetShouldCloseWindow() const { return mShouldCloseWindow; }

	// Setters
	void SetShouldCloseWindow(bool value) { mShouldCloseWindow = value; }

private:
	std::vector<class Object*> mObjs;

	const int mScrWidth;
	const int mScrHeight;

	bool mShouldCloseWindow;
	bool mShouldPause;

	class Shader* mMeshShader;
	class Shader* mPhongShader;

	struct Camera
	{
		glm::vec3 position;
		glm::vec3 target;
		glm::vec3 up;
	};
	Camera* mCamera;
};

