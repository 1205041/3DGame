#include "GameScene.h"

#include "../../Object/ObjBase.h"

// �L�����N�^
#include "../../Object/Player/Player.h"

// �n�`
#include "../../Object/Terrain/Stage/Stage.h"
#include "../../Object/Terrain/Ground/Ground.h"
#include "../../Object/SkySphere/SkySphere.h"

// �Q�[��UI
#include "../../Object/2Ddraw/GameUI/Sight/Sight.h"

// �J����
#include "../../Object/Camera/TPS/TPS.h"

void GameScene::Event()
{
	// �V�[���ؑ�(Game��Title)
	if (GetAsyncKeyState('P') & 0x8000)
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
	}
	if (GetAsyncKeyState('O') & 0x8000)
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Result);
	}
	

	/* �� ���̒i�K�ł͍X�V����܂��� �I�I */
}

void GameScene::Init()
{
//	KdAudioManager::Instance().Play("Asset/Sounds/SE/GameStart.wav");
//	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Game.wav", true);

	/* �I�u�W�F�N�g�̏����� */
	// �n�`
	std::shared_ptr<Stage> spStage;
	spStage = std::make_shared<Stage>();
	m_objList.push_back(spStage);

	std::shared_ptr<SkySphere> spSkySp;
	spSkySp = std::make_shared<SkySphere>();
	m_objList.push_back(spSkySp);

	// �ʏ�̒n��
	std::shared_ptr<Ground> spGround;
	spGround = std::make_shared<Ground>();
	spGround->SetScale({ 10.0f, 3.0f, 10.0f });
	spGround->SetPos({ 0,-1.5f,0 });
	m_objList.push_back(spGround);

	spGround = std::make_shared<Ground>();
	spGround->SetScale({ 15.0f, 3.0f, 15.0f });
	spGround->SetPos({ -120.0f,-1.5f,0.0f });
	m_objList.push_back(spGround);

	spGround = std::make_shared<Ground>();
	spGround->SetScale({ 15.0f, 3.0f, 15.0f });
	spGround->SetPos({ 120.0f,-1.5f,0.0f });
	m_objList.push_back(spGround);

	spGround = std::make_shared<Ground>();
	spGround->SetScale({ 15.0f, 3.0f, 15.0f });
	spGround->SetPos({ 0.0f,-1.5f,120.0f });
	m_objList.push_back(spGround);

	spGround = std::make_shared<Ground>();
	spGround->SetScale({ 15.0f, 3.0f, 15.0f });
	spGround->SetPos({ 0.0f,-1.5f,-120.0f });
	m_objList.push_back(spGround);

	// �L����
	std::shared_ptr<Player> spPlayer;	
	spPlayer = std::make_shared<Player>();
	spPlayer->SetPos({ 0,5.0f,0 });
	spPlayer->RegistHitObj(spSkySp);	/* SkySphere�Ƃ̓����蔻�� */
	spPlayer->RegistHitObj(spStage);	/* Stage�Ƃ̓����蔻�� */
	m_objList.push_back(spPlayer);

	// �Q�[��UI
	std::shared_ptr<Sight> spSight;
	spSight = std::make_shared<Sight>();
	m_objList.push_back(spSight);
	
	// �J�����̏�����
	std::shared_ptr<TPS> spTps;
	spTps = std::make_shared<TPS>();
	spTps->SetTarget(spPlayer);
	spTps->RegistHitObject(spGround);	/* Ground�Ƃ̓����蔻�� */
	spPlayer->SetCamera(spTps);
	m_objList.push_back(spTps);
}
