#include "renderer.h"

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>

#include "game.h"
#include "shader.h"
#include "actor.h"
#include "texture.h"
#include "mesh.h"

Renderer::Renderer(Game* game)
	: mGame{ game },
	mMeshShader{ nullptr },
	mScrWidth{ 0 },
	mScrHeight{ 0 }
{

}

Renderer::~Renderer()
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
	delete mMeshShader;

	// Destroy textures
	for (auto item : mTextures)
		delete item.second;
	mTextures.clear();

	// Destroy meshes
	for (auto item : mMeshes)
		delete item.second;
	mMeshes.clear();
}

void Renderer::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	mMeshShader->SetActive();
	
	for (auto actor : mGame->GetActors())
		actor->Draw(mMeshShader);

	glutSwapBuffers();
}

bool Renderer::LoadShaders()
{
	mMeshShader = new Shader{};
	if (!mMeshShader->Load("Shaders/basicMesh.vert", "Shaders/basicMesh.frag"))
		return false;

	mMeshShader->SetActive();
	glm::mat4 view{ 1.0f };
	view = glm::lookAt(glm::vec3{ 0.0f, 0.5f, 3.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
	mMeshShader->SetMatrix4Uniform("uView", view);
	glm::mat4 proj{ 1.0f };
	proj = glm::perspective(45.0f, static_cast<float>(mScrWidth) / mScrHeight, 0.1f, 100.0f);
	mMeshShader->SetMatrix4Uniform("uProj", proj);

	return true;
}

Texture* Renderer::GetTexture(const std::string& file)
{
	Texture* tex = nullptr;
	auto iter = mTextures.find(file);
	if (iter != mTextures.end())
		tex = iter->second;
	else
	{
		tex = new Texture{};
		if (tex->Load(file))
			mTextures.emplace(file, tex);
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

Mesh* Renderer::GetMesh(const std::string& file)
{
	Mesh* m = nullptr;
	auto iter = mMeshes.find(file);
	if (iter != mMeshes.end())
		m = iter->second;
	else
	{
		m = new Mesh();
		if (m->Load(file, this))
			mMeshes.emplace(file, m);
		else
		{
			delete m;
			m = nullptr;
		}
	}
	return m;
}
