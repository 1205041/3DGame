#include "GameScene.h"

#include "../../Object/ObjBase.h"

// �L�����N�^
#include "../../Object/Player/Player.h"
#include "../../Object/Enemy/Enemy.h"

// �n�`
#include "../../Object/Terrain/Stage/Stage.h"

// �X�J�C�X�t�B�A
#include "../../Object/SphereDome/SphereGround.h"

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

	// �ʏ�̒n��
	std::shared_ptr<SphereGround> spGrndDm;
	spGrndDm = std::make_shared<SphereGround>();
	m_objList.push_back(spGrndDm);

	// �Q�[��UI
	std::shared_ptr<Sight> spSight;
	spSight = std::make_shared<Sight>();
	m_objList.push_back(spSight);

	// �L����
	std::shared_ptr<Player> spPlayer;	
	spPlayer = std::make_shared<Player>();
	spPlayer->RegistHitObj(spGrndDm);	/* SkySphere�Ƃ̓����蔻�� */
//	spPlayer->RegistHitObj(spStage);	/* Stage�Ƃ̓����蔻�� */
	m_objList.push_back(spPlayer);

	// �G�l�~�[
	std::shared_ptr<Enemy> spEnemy;
	spEnemy = std::make_shared<Enemy>();
	spEnemy->SetPos({ 0,0.0f,5.0f });
	m_objList.push_back(spEnemy);

	// �J�����̏�����
	std::shared_ptr<TPS> spTps;
	spTps = std::make_shared<TPS>();
//	spTps->RegistHitObject(spGround);	/* Ground�Ƃ̓����蔻�� */
	spTps->SetTarget(spPlayer);
	spPlayer->SetCamera(spTps);
	m_objList.push_back(spTps);
}
