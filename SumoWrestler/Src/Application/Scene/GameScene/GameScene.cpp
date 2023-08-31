#include "GameScene.h"

#include "../../Object/ObjBase.h"

// �L�����N�^
#include "../../Object/Player/Player.h"
#include "../../Object/Enemy/Enemy.h"

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
	KdAudioManager::Instance().Play("Asset/Sounds/SE/GameStart.wav");
	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Game.wav", true);

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
	std::shared_ptr<Enemy>	spEnemy;
	
	spPlayer = std::make_shared<Player>();
	spEnemy	 = std::make_shared<Enemy>();
	
	spPlayer->SetPos({ -2.0f,-4.0f,0 });
	spEnemy->SetPos({ 2.0f,-4.0f,0 });
	
	spPlayer->RegistHitObj(spGround);
	spEnemy->RegistHitObj(spGround);

	spPlayer->RegistHitObj(spEnemy);
	spEnemy->RegistHitObj(spPlayer);

	spEnemy->SetPlayer(spPlayer);

	m_objList.push_back(spPlayer);
	m_objList.push_back(spEnemy);
	
	// �J�����̏�����
	std::shared_ptr<TPS> spTps;
	spTps = std::make_shared<TPS>();
	spTps->SetTarget(spPlayer);
	spPlayer->SetCamera(spTps);
	m_objList.push_back(spTps);
}
