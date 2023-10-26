#include "GameScene.h"

#include "../../Object/ObjBase.h"

// �L�����N�^
#include "../../Object/Player/Player.h"
#include "../../Object/Enemy/Enemy.h"

// �n�`
#include "../../Object/Terrain/Stage/Stage.h"
#include "../../Object/Terrain/Ground/Ground.h"

// �X�J�C�X�t�B�A
#include "../../Object/SphereDome/TropicalDomo/TropicalDome.h"
#include "../../Object/SphereDome/RiverSide/RiverSide.h"

// �Q�[��UI
#include "../../Object/TextDraw/SceneUI/Game/Sight/Sight.h"

// �J����
#include "../../Object/Camera/TPS/TPS.h"

void GameScene::Event()
{
	// �V�[���ؑ�(Game��Result)
	if (GetAsyncKeyState('P') & 0x8000)
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Result);
	}
	

	/* �� ���̒i�K�ł͍X�V����܂��� �I�I */
}

void GameScene::Init()
{
//	KdAudioManager::Instance().Play("Asset/Sounds/SE/GameStart.wav");
//	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Game.wav", true);

	// �}�E�X�|�C���^��\��
	ShowCursor(false);

	/* �I�u�W�F�N�g�̏����� */
	// �n�`
//	std::shared_ptr<Stage> spStage;
//	spStage = std::make_shared<Stage>();
//	m_objList.push_back(spStage);

	std::shared_ptr<TropicalDome> spTroDm;
	spTroDm = std::make_shared<TropicalDome>();
	m_objList.push_back(spTroDm);

	std::shared_ptr<RiverSide> spRvSd;
	spRvSd = std::make_shared<RiverSide>();
	m_objList.push_back(spRvSd);

	// �ʏ�̒n��
	std::shared_ptr<Ground> spGround;
	spGround = std::make_shared<Ground>();
	spGround->SetScale({ 1.0f, 1.0f, 1.0f });
 	spGround->SetPos({ 0,-1.5f,0 });
	m_objList.push_back(spGround);

	// �Q�[��UI
	std::shared_ptr<Sight> spSight;
	spSight = std::make_shared<Sight>();
	m_objList.push_back(spSight);

	// �L����
	std::shared_ptr<Player> spPlayer;	
	spPlayer = std::make_shared<Player>();
	spPlayer->SetPos({ 0,5.0f,0 });
//	spPlayer->RegistHitObj(spSkySp);	/* SkySphere�Ƃ̓����蔻�� */
//	spPlayer->RegistHitObj(spStage);	/* Stage�Ƃ̓����蔻�� */
	m_objList.push_back(spPlayer);

	// �G�l�~�[
	std::shared_ptr<Enemy> spEnemy;
	spEnemy = std::make_shared<Enemy>();
	spEnemy->SetPos({ 0,5.0f,10.0f });
	m_objList.push_back(spEnemy);

	// �J�����̏�����
	std::shared_ptr<TPS> spTps;
	spTps = std::make_shared<TPS>();
	spTps->RegistHitObject(spGround);	/* Ground�Ƃ̓����蔻�� */
	spTps->SetTarget(spPlayer);
	spPlayer->SetCamera(spTps);
	m_objList.push_back(spTps);
}
