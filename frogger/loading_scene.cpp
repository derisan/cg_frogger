#include "loading_scene.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <rapidjson/document.h>

#include "gfw.h"
#include "renderer.h"
#include "shader.h"
#include "sound_engine.h"
#include "loading_actor.h"

LoadingScene::LoadingScene(Gfw* gfw)
	: Scene{ gfw },
	mRenderer{ nullptr },
	mSpriteShader{ nullptr },
	mCurIdx{ 0 },
	mElapsed{ 0.0f }
{

}

void LoadingScene::Enter()
{
	mRenderer = Renderer::Get();
	mSpriteShader = mRenderer->GetShader("sprite");
	
	if (!ReadFromJson("assets.json"))
	{
		std::cout << "Failed to read assets from json" << std::endl;
		mGfw->PopScene();
	}

	new LoadingActor{ this };

	auto se = SoundEngine::Get();
	se->Create("Sounds/jump.wav", "jump");
	se->Create("Sounds/bgm.wav", "bgm", true);
}

void LoadingScene::Exit()
{
	while (!mActors.empty())
		delete mActors.back();
}

void LoadingScene::ProcessInput(unsigned char key)
{
	if (key == 27)
		mGfw->PopScene();
}

void LoadingScene::Update()
{
	mElapsed += dt;

	for (auto actor : mActors)
		actor->Update();

	if (mCurIdx < mMeshFiles.size())
	{
		mRenderer->GetMesh(mMeshFiles[mCurIdx]);
		std::cout << "Read mesh: " << mMeshFiles[mCurIdx++] << std::endl;
	}
	else if (mCurIdx == mMeshFiles.size() && mElapsed > 3.5f)
		mGfw->ChangeScene("start");
}

void LoadingScene::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	mSpriteShader->SetActive();
	for (auto actor : mActors)
		actor->Draw(mSpriteShader);

	glutSwapBuffers();
}

void LoadingScene::AddSceneActor(SceneActor* actor)
{
	mActors.emplace_back(actor);
}

void LoadingScene::RemoveSceneActor(SceneActor* actor)
{
	auto iter = std::find(std::begin(mActors), std::end(mActors), actor);
	if (iter != std::end(mActors))
		mActors.erase(iter);
}

bool LoadingScene::ReadFromJson(const std::string& file)
{
	std::ifstream f{ file };
	if (!f.is_open())
	{
		std::cout << "Failed to open file " << file << std::endl;
		return false;
	}

	std::stringstream fileStream;
	fileStream << f.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		std::cout << file << " is not valid json" << std::endl;
		return false;
	}

	const rapidjson::Value& meshes = doc["gpmesh"];
	if (!meshes.IsArray() || meshes.Size() < 1)
	{
		std::cout << file << " has no gpmesh." << std::endl;
		return false;
	}

	for (rapidjson::SizeType i = 0; i < meshes.Size(); i++)
	{
		std::string meshName = meshes[i].GetString();
		mMeshFiles.push_back("Assets/" + meshName);
	}

	return true;
}