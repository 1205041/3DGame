#include "LoseScene.h"

// �L�����N�^�[
#include "../../../Object/Enemy/Enemy.h"

// �n�`
#include "../../../Object/Ground/Ground.h"
#include "../../../Object/SkySphere/SkySphere.h"

// �V�[��
#include "../../../Object/SceneText/LoseScene/LoseText.h"

// �J����
#include "../../../Object/Camera/Tracking/Tracking.h"

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
	KdAudioManager::Instance().Play("Asset/Sounds/SE/GameSet.wav");
	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Makeikusa.wav", true);

	/* �I�u�W�F�N�g�̏����� */
	// �V�[��
	std::shared_ptr<LoseText> spLose;
	spLose = std::make_shared<LoseText>();
	m_objList.push_back(spLose);

	// �n�`
	std::shared_ptr<Ground> spGround;
	spGround = std::make_shared<Ground>();
	m_objList.push_back(spGround);

	std::shared_ptr<SkySphere> spSkySp;
	spSkySp = std::make_shared<SkySphere>();
	m_objList.push_back(spSkySp);

	// �L����
	std::shared_ptr<Enemy> spEnemy;
	spEnemy = std::make_shared<Enemy>();
	spEnemy->SetPos({ 0,0,0 });
	spEnemy->RegistHitObj(spGround);
	m_objList.push_back(spEnemy);

	// �J�����̏�����
	std::shared_ptr<Tracking> spTrac;
	spTrac = std::make_shared<Tracking>();
	spTrac->SetTarget(spLose);
	m_objList.push_back(spTrac);
}