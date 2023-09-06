#include "TitleScene.h"

// �n�`
#include "../../Object/SkySphere/SkySphere.h"

// �^�C�g���V�[��
#include "../../Object/SceneText/TitleScene/TitleText.h"

// �J����
#include "../../Object/Camera/TPS/TPS.h"

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
//	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Title.wav", true);
	
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
	// �J�����̏�����
	std::shared_ptr<TPS> spTps;
	spTps = std::make_shared<TPS>();
	spTps->SetTarget(spTitle);
	m_objList.push_back(spTps);
}
