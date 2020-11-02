#include "game.h"

#include <iostream>
#include <algorithm>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "random.h"
#include "cube.h"
#include "plane.h"

Game::Game(int w, int h)
	: mScrWidth{ w },
	mScrHeight{ h },
	mShouldCloseWindow{ false },
	mShouldPause{ false },
	mMeshShader{ nullptr },
	mPhongShader{ nullptr },
	mCamera{ nullptr }
{

}

bool Game::Init(int* argc, char** argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(mScrWidth, mScrHeight);
	glutCreateWindow("Frogger");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Unable to initialize GLEW" << std::endl;
		return false;
	}

	Random::Init();

	if (!LoadData())
	{
		std::cout << "Failed to load data" << std::endl;
		return false;
	}

	return true;
}

bool Game::LoadData()
{
	mMeshShader = new Shader{};
	mPhongShader = new Shader{};
	if (!mMeshShader->Load("Shaders/mesh.vert", "Shaders/mesh.frag")
		|| !mPhongShader->Load("Shaders/phong.vert", "Shaders/phong.frag"))
		return false;
	
	mCamera = new Camera{};
	mCamera->position = glm::vec3{ 0.0f, 1.0f, 6.0f };
	mCamera->target = glm::vec3{ 0.0f, 0.0f, -1.0f };
	mCamera->up = glm::vec3{ 0.0f, 1.0f, 0.0f };

	glm::mat4 proj{ 1.0f };
	proj = glm::perspective(45.0f, static_cast<float>(mScrWidth) / static_cast<float>(mScrHeight), 
		0.1f, 100.0f);

	// Projection matrix rarely changes
	mMeshShader->SetActive();
	mMeshShader->SetMatrix4Uniform("uProj", proj);
	mPhongShader->SetActive();
	mPhongShader->SetMatrix4Uniform("uProj", proj);

	Cube* cube{ new Cube{this} };
	cube->SetColor(glm::vec3{ 1.0f, 0.0f, 0.0f });
	cube->SetScale(glm::vec3{ 0.5f, 0.5f, 0.5f });

	Plane* plane{ new Plane{this} };
	plane->SetScale(glm::vec3{ 10.0f, 1.0f, 10.0f });
	plane->SetColor(glm::vec3{ 0.0f, 1.0f, 1.0f });
	
	
	return true;
}

void Game::Shutdown()
{
	delete mMeshShader;
	delete mPhongShader;
	delete mCamera;

	while (!mObjs.empty())
		delete mObjs.back();
}

void Game::ProcessKeyboardInput(unsigned char key)
{
	switch (key)
	{		
		case 'p': case 'P':
			mShouldPause = !mShouldPause;
			break;
	}
}

void Game::ProcessMouseInput(int button, int state, int x, int y)
{
	
}

void Game::Update()
{
	if (mShouldPause)
		return;

	std::vector<Object*> deads;
	for (auto obj : mObjs)
	{
		obj->Update();
		if (obj->GetState() == Object::State::kDead)
			deads.emplace_back(obj);
	}

	for (auto obj : deads)
		delete obj;
	deads.clear();
}

void Game::Draw()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm::mat4 view{ 1.0f };
	view = glm::lookAt(mCamera->position, mCamera->position + mCamera->target, mCamera->up);
	mMeshShader->SetActive();
	mMeshShader->SetMatrix4Uniform("uView", view);

	for (auto obj : mObjs)
		obj->Draw(mMeshShader);

	mPhongShader->SetActive();
	mPhongShader->SetMatrix4Uniform("uView", view);
	

	glutSwapBuffers();
}

void Game::AddObj(Object* obj)
{
	mObjs.emplace_back(obj);
}

void Game::RemoveObj(Object* obj)
{
	auto iter = std::find(std::begin(mObjs), std::end(mObjs), obj);
	if (iter != std::end(mObjs))
		mObjs.erase(iter);
}