#include "SceneManager.h"

#include "SceneBase/SceneBase.h"
#include "Ttile/TitleScene.h"
#include "Game/GameScene.h"
#include "Result/ResultScene.h"

void SceneManager::PreUpdate()
{
	// �V�[���ؑ�
	if (m_curtSceneType != m_nextSceneType) { ChangeScene(m_nextSceneType); }

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

void SceneManager::ChangeScene(const SceneType& _type)
{
	KdSafeDelete(m_curtScene);

	// ���̃V�[�����쐬���A���݂̃V�[���ɂ���
	switch (_type)
	{
	case SceneType::Title:
		m_curtScene = new TitleScene();
		break;
	case SceneType::Game:
		m_curtScene = new GameScene();
		break;
	case SceneType::Result:
		m_curtScene = new ResultScene();
		break;
	}
	m_curtSceneType = _type;
}
