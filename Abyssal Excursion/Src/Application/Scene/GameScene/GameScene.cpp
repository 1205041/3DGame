#include "GameScene.h"

#include "../../Object/ObjBase.h"

// �L�����N�^
#include "../../Object/Player/Player.h"

// �n�`
#include "../../Object/Ground/Ground.h"
#include "../../Object/SkySphere/SkySphere.h"

// �J����
#include "../../Object/Camera/TPS/TPS.h"

void GameScene::Event()
{
	
	/* �� ���̒i�K�ł͍X�V����܂��� �I�I */
}

void GameScene::Init()
{
//	KdAudioManager::Instance().Play("Asset/Sounds/SE/GameStart.wav");
//	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Game.wav", true);

	/* �I�u�W�F�N�g�̏����� */
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
	spPlayer->RegistHitObj(spGround);
	m_objList.push_back(spPlayer);
	
	// �J�����̏�����
	std::shared_ptr<TPS> spTps;
	spTps = std::make_shared<TPS>();
	spTps->SetTarget(spPlayer);
	spTps->RegistHitObject(spGround);
	spPlayer->SetCamera(spTps);
	m_objList.push_back(spTps);
}
