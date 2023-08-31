#include "TitleScene.h"

// �n�`
#include "../../Object/SkySphere/SkySphere.h"

// �^�C�g���V�[��
#include "../../Object/SceneText/TitleScene/TitleText.h"

// �J����
#include "../../Object/Camera/Tracking/Tracking.h"

void TitleScene::Event()
{
	// �V�[���ؑ�(Title��Game)
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		KdAudioManager::Instance().Play("Asset/Sounds/SE/PushButton.wav");
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Game);
		KdAudioManager::Instance().StopAllSound();
	}
}

void TitleScene::Init()
{
	ShowCursor(false);

	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Title.wav", true);
	
	/* �I�u�W�F�N�g�̏����� */
	// �V�[��
	std::shared_ptr<TitleText> spTitle;
	spTitle = std::make_shared<TitleText>();
	m_objList.push_back(spTitle);

	// �n�`
	std::shared_ptr<SkySphere> spSkySp;
	spSkySp = std::make_shared<SkySphere>();
	m_objList.push_back(spSkySp);

	// �J�����̏�����
	std::shared_ptr<Tracking> spTrac;
	spTrac = std::make_shared<Tracking>();
	spTrac->SetTarget(spTitle);
	m_objList.push_back(spTrac);
}
