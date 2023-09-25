#include "TitleScene.h"
#include "../../Object/2Ddraw/Title2D/Title2D.h"

void TitleScene::Event()
{
	// �V�[���ؑ�(Title��Game)
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
//		KdAudioManager::Instance().Play("Asset/Sounds/SE/PushButton.wav");
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Game);
//		KdAudioManager::Instance().StopAllSound();
	}
}

void TitleScene::Init()
{
//	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Title.wav", true);
	
	// �I�u�W�F�N�g�̏�����
	std::shared_ptr<Title2D> spTitle;
	spTitle = std::make_shared<Title2D>();
	m_objList.push_back(spTitle);
}
