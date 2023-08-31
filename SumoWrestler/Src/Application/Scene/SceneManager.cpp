#include "SceneManager.h"

#include "SceneBase/SceneBase.h"
#include "TitleScene/TitleScene.h"
#include "GameScene/GameScene.h"
#include "ResultScene/Lose/LoseScene.h"
#include "ResultScene/Win/WinScene.h"

void SceneManager::PreUpdate()
{
	// �V�[���ؑ�
	if (m_curtSceneType != m_nextSceneType)
	{
		ChangeScene(m_nextSceneType);
	}

	m_curtScene->PreUpdate();
}

void SceneManager::Update()
{
	m_curtScene->Update();
}

void SceneManager::PostUpdate()
{
	m_curtScene->PostUpdate();
}

void SceneManager::PreDraw()
{
	m_curtScene->PreDraw();
}

void SceneManager::DrawLit()
{
	m_curtScene->DrawLit();
}

void SceneManager::DrawSprite()
{
	m_curtScene->DrawSprite();
}

void SceneManager::DrawDebug()
{
	m_curtScene->DrawDebug();
}

const std::list<std::shared_ptr<KdGameObject>>& SceneManager::GetObjList()
{
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�

	return m_curtScene->GetObjList();
}

void SceneManager::AddObject(const std::shared_ptr<KdGameObject>& _obj)
{
	m_curtScene->AddObject(_obj);
}

void SceneManager::ChangeScene(const SceneType& _sceneType)
{
	// ���݂̃V�[�����폜
	KdSafeDelete(m_curtScene);

	// ���̃V�[�����쐬���A���݂̃V�[���ɂ���
	switch (_sceneType)
	{
	case SceneType::Title:
		m_curtScene = new TitleScene();
		break;
	case SceneType::Game:
		m_curtScene = new GameScene();
		break;
	case SceneType::Win:
		m_curtScene = new WinScene();
		break;
	case SceneType::Lose:
		m_curtScene = new LoseScene();
		break;
	}
	m_curtSceneType = _sceneType;

}