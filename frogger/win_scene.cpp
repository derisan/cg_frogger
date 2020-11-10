#include "win_scene.h"

#include <algorithm>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>

#include "gfw.h"
#include "renderer.h"
#include "shader.h"
#include "start_actor.h"
#include "sound_engine.h"
#include "sparticle.h"
#include "bunny.h"
#include "random.h"
#include "win_actor.h"

WinScene::WinScene(Gfw* gfw)
	: Scene{ gfw },
	mRenderer{ nullptr },
	mSpriteShader{ nullptr },
	mMeshShader{ nullptr },
	mCurParticleNum{ 0 }
{

}

void WinScene::Enter()
{
	mRenderer = Renderer::Get();
	mSpriteShader = mRenderer->GetShader("sprite");
	mMeshShader = mRenderer->GetShader("basicMesh");

	glm::mat4 view{ 1.0f };
	view = glm::lookAt(glm::vec3{ 0.0f, 1.0f, 10.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
	glm::mat4 proj{ 1.0f };
	proj = glm::perspective(45.0f, static_cast<float>(GetGfw()->GetScrWidth()) / GetGfw()->GetScrHeight(),
		0.1f, 100.0f);
	mMeshShader->SetActive();
	mMeshShader->SetMatrix4Uniform("uView", view);
	mMeshShader->SetMatrix4Uniform("uProj", proj);

	mCurParticleNum = 0;

	auto bunny = new Bunny{ this };
	bunny->SetPosition(glm::vec3{ -5.0f, 0.0f, -3.0f });
	bunny->SetRotation(Random::GetFloatRange(0.0f, 360.0f));
	bunny = new Bunny{ this };
	bunny->SetPosition(glm::vec3{ 5.0f, 0.0f, -3.0f });
	bunny->SetRotation(Random::GetFloatRange(0.0f, 360.0f));
	bunny = new Bunny{ this };
	bunny->SetPosition(glm::vec3{ 2.5f, 0.0f, 1.5f });
	bunny->SetRotation(Random::GetFloatRange(0.0f, 360.0f));
	bunny = new Bunny{ this };
	bunny->SetPosition(glm::vec3{ -2.5f, 0.0f, 1.5f });
	bunny->SetRotation(Random::GetFloatRange(0.0f, 360.0f));
	bunny = new Bunny{ this };

	auto youwin = new WinActor{ this };
	youwin->SetPosition(glm::vec3{ 5.0f, -3.0f, 0.0f });
	youwin->SetScale(5.0f);

	SoundEngine::Get()->Play("victory.mp3");
}

void WinScene::Exit()
{
	while (!mActors.empty())
		delete mActors.back();

	SoundEngine::Get()->Stop("victory.mp3");
}

void WinScene::ProcessInput(unsigned char key)
{
	if (key == 27)
		mGfw->PopScene();
	else if (key == 13) // enter
		mGfw->ChangeScene("main");			
}

void WinScene::Update()
{
	if (mCurParticleNum < 30)
	{
		auto particle = new SParticle{ this, glm::vec3{-7.0f, -5.0f, 0.0f}, SParticle::SParticleType::kCarrot };
		particle->SetVelocity(glm::vec3{ Random::GetFloatRange(6.0f, 7.0f), Random::GetFloatRange(6.0f, 8.0f), 0.0f });
		++mCurParticleNum;
	}

	std::vector<SceneActor*> deads;
	for (auto actor : mActors)
	{
		actor->Update();
		if (actor->GetState() == SceneActor::State::kDead)
			deads.emplace_back(actor);
	}

	for (auto actor : deads)
	{
		delete actor;
		--mCurParticleNum;
	}
}

void WinScene::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	mMeshShader->SetActive();
	for (auto actor : mActors)
		actor->Draw(mMeshShader);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	mSpriteShader->SetActive();

	glutSwapBuffers();
}

void WinScene::AddSceneActor(SceneActor* actor)
{
	mActors.emplace_back(actor);
}

void WinScene::RemoveSceneActor(SceneActor* actor)
{
	auto iter = std::find(std::begin(mActors), std::end(mActors), actor);
	if (iter != std::end(mActors))
		mActors.erase(iter);
}