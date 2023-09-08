#include "LoseScene.h"
#include "../../../Object/2Ddraw/2Ddraw.h"

void LoseScene::Event()
{
	// �V�[���ؑ�(Result��Title)
	if (GetAsyncKeyState('P') & 0x8000)
	{
		KdAudioManager::Instance().Play("Asset/Sounds/SE/PushButton.wav");
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
		KdAudioManager::Instance().StopAllSound();
	}
}

void LoseScene::Init()
{
	//	KdAudioManager::Instance().Play("Asset/Sounds/SE/GameSet.wav");
	//	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Makeikusa.wav", true);

		/* �I�u�W�F�N�g�̏����� */
		// �V�[��
	std::shared_ptr<twoDdraw> spLose;
	spLose = std::make_shared<twoDdraw>();
	spLose->SetTexture("Asset/Textures/Result/lose.png");
	m_objList.push_back(spLose);
}