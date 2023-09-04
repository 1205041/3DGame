#include "WinScene.h"

// �L�����N�^�[
#include "../../../Object/Player/Player.h"

// �n�`
#include "../../../Object/Ground/Ground.h"
#include "../../../Object/SkySphere/SkySphere.h"

// �V�[��
#include "../../../Object/SceneText/WinScene/WinText.h"

// �J����
#include "../../../Object/Camera/Tracking/Tracking.h"

void WinScene::Event()
{
	// �V�[���ؑ�(Result��Title)
	if (GetAsyncKeyState('P') & 0x8000)
	{
		KdAudioManager::Instance().Play("Asset/Sounds/SE/PushButton.wav");
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
		KdAudioManager::Instance().StopAllSound();
	}
}

void WinScene::Init()
{
	KdAudioManager::Instance().Play("Asset/Sounds/SE/GameSet.wav");
	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Katidoki.wav", true);

	/* �I�u�W�F�N�g�̏����� */
	// �V�[��
	std::shared_ptr<WinText> spWin;
	spWin = std::make_shared<WinText>();
	m_objList.push_back(spWin);

	// �n�`
	std::shared_ptr<Ground> spGround;
	spGround = std::make_shared<Ground>();
	m_objList.push_back(spGround);

	std::shared_ptr<SkySphere> spSkySp;
	spSkySp = std::make_shared<SkySphere>();
	m_objList.push_back(spSkySp);

	// �L����
	std::shared_ptr<Player> spPlayer;
	spPlayer = std::make_shared<Player>();
	spPlayer->SetPos({ 0,0,0 });
	spPlayer->RegistHitObj(spGround);
	m_objList.push_back(spPlayer);

	// �J�����̏�����
	std::shared_ptr<Tracking> spTrac;
	spTrac = std::make_shared<Tracking>();
	spTrac->SetTarget(spWin);
	m_objList.push_back(spTrac);
}