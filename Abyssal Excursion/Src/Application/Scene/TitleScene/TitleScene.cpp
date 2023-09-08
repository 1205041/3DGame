#include "TitleScene.h"
#include "../../Object/2Ddraw/2Ddraw.h"

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
	std::shared_ptr<twoDdraw> spTitle;
	spTitle = std::make_shared<twoDdraw>();
	spTitle->SetTexture("Asset/Textures/Title/Title.png");
	m_objList.push_back(spTitle);
}
