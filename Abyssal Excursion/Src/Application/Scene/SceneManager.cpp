#include "SceneManager.h"

#include "SceneBase/SceneBase.h"
#include "TitleScene/TitleScene.h"
#include "GameScene/GameScene.h"
#include "ResultScene/Lose/LoseScene.h"
#include "ResultScene/Win/WinScene.h"

void SceneManager::PreUpdate()
{
	// シーン切替
	if (m_curtSceneType != m_nextSceneType)
	{
		ChangeScene(m_nextSceneType);
	}

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
	m_spCurtScene = std::make_shared<SceneBase>();

	ChangeScene(SceneType::Game);
}

const std::list<std::shared_ptr<KdGameObject>>& SceneManager::GetObjList()
{
	// TODO: return ステートメントをここに挿入します

	return m_spCurtScene->GetObjList();
}

void SceneManager::AddObject(const std::shared_ptr<KdGameObject>& _obj)
{
	m_spCurtScene->AddObject(_obj);
}

void SceneManager::ChangeScene(const SceneType& _sceneType)
{
	std::shared_ptr<TitleScene> title;
	std::shared_ptr<GameScene> game;
	std::shared_ptr<WinScene> win;
	std::shared_ptr<LoseScene> lose;

	// 次のシーンを作成し、現在のシーンにする
	switch (_sceneType)
	{
	case SceneType::Title:
		title = std::make_shared<TitleScene>();
		m_spCurtScene = title;
		break;
	case SceneType::Game:
		game = std::make_shared<GameScene>();
		m_spCurtScene = game;
		break;
	case SceneType::Win:
		win = std::make_shared<WinScene>();
		m_spCurtScene = win;
		break;
	case SceneType::Lose:
		lose = std::make_shared<LoseScene>();
		m_spCurtScene = lose;
		break;
	}
	m_curtSceneType = _sceneType;

}