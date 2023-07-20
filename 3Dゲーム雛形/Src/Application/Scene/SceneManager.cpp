#include "SceneManager.h"

#include "SceneBase/SceneBase.h"
#include "Ttile/TitleScene.h"
#include "Game/GameScene.h"
#include "Result/ResultScene.h"

void SceneManager::PreUpdate()
{
	// シーン切替
	if (m_curtSceneType != m_nextSceneType) { ChangeScene(m_nextSceneType); }

	m_spCurtScene->PreUpdate();
}

void SceneManager::Update()
{
	m_spCurtScene->Update();
}

void SceneManager::PostUpdate()
{
	m_spCurtScene->PostUpdate();
}

void SceneManager::PreDraw()
{
	m_spCurtScene->PreDraw();
}

void SceneManager::DrawLit()
{
	m_spCurtScene->DrawLit();
}

void SceneManager::DrawSprite()
{
	m_spCurtScene->DrawSprite();
}

void SceneManager::DrawDebug()
{
	m_spCurtScene->DrawDebug();
}

void SceneManager::Init()
{
	if (!m_spCurtScene) { m_spCurtScene = std::make_shared<SceneBase>(); }

	ChangeScene(SceneType::Game);
}

const std::list<std::shared_ptr<KdGameObject>>& SceneManager::GetObjList() const
{
	return m_spCurtScene->GetObjList();
}

void SceneManager::AddObject(const std::shared_ptr<KdGameObject>& _obj)
{
	m_spCurtScene->AddObject(_obj);	
}

void SceneManager::ChangeScene(const SceneType& _type)
{
	// 次のシーンを作成し、現在のシーンにする
	switch (_type)
	{
	case SceneType::Title:
		if (!title) { title = std::make_shared<TitleScene>(); }
		m_spCurtScene = title;
		break;
	case SceneType::Game:
		if (!game) { game = std::make_shared<GameScene>(); }
		m_spCurtScene = game;
		break;
	case SceneType::Result:
		if (!result) { result = std::make_shared<ResultScene>(); }
		m_spCurtScene = result;
		break;
	}
	m_curtSceneType = _type;
}
