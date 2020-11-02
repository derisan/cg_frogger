#include "renderer.h"

#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "shader.h"

Renderer::Renderer(Game* game)
	: mGame{ game },
	mLightCubeShader{ nullptr },
	mMeshShader{ nullptr },
	mScrWidth{ 0 },
	mScrHeight{ 0 }
{

}

bool Renderer::Init(int* argc, char** argv, int w, int h)
{
	mScrWidth = w;
	mScrHeight = h;

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

	if (!LoadShaders())
	{
		std::cout << "Failed to load shaders" << std::endl;
		return false;
	}

	return true;
}

void Renderer::Shutdown()
{

}

void Renderer::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


	glutSwapBuffers();
}

bool Renderer::LoadShaders()
{
	mLightCubeShader = new Shader{};
	if(!mLightCubeShader->Load("Shaders/light_cube.vert", "Shaders/light_cube.frag"))
		return false;

	mMeshShader = new Shader{};
	if (!mMeshShader->Load("Shaders/mesh.vert", "Shaders/mesh.frag"))
		return false;

	return true;
}