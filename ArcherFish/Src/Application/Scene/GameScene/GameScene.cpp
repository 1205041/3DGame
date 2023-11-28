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
#include "../../Object/TextDraw/TextDraw.h"
#include "../../Object/TextDraw/SceneUI/MaxWaterLv/MaxWaterLv.h"

// �J����
#include "../../Object/Camera/TPSCam/TPSCam.h"

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

	// �X�J�C�X�t�B�A
	std::shared_ptr<SphereGround> spGrndDm;
	spGrndDm = std::make_shared<SphereGround>();
	m_objList.push_back(spGrndDm);

	// �G�l�~�[
	std::shared_ptr<Enemy> spEnemy;
	spEnemy = std::make_shared<Enemy>();
	spEnemy->SetPos({ 0.0f,10.0f,3.0f });
	m_objList.push_back(spEnemy);

	// �L����
	std::shared_ptr<Player> spPlayer;	
	spPlayer = std::make_shared<Player>();
	m_objList.push_back(spPlayer);

	spEnemy->RegistHitObj(spPlayer);	/* spPlayer�Ƃ̓����蔻�� */
	spPlayer->RegistHitObj(spEnemy);	/* spEnemy�Ƃ̓����蔻�� */
	spGrndDm->RegistHitObj(spPlayer);	/* Player�Ƃ̓����蔻�� */
	spPlayer->RegistHitObj(spGrndDm);	/* SkySphere�Ƃ̓����蔻�� */

	// �Q�[��UI
	std::shared_ptr<TextDraw> spText;
	spText = std::make_shared<TextDraw>();
	spText->SetLoadText("Asset/Textures/SceneUI/Game/SightTP.png", { 0.0f,0.0f }, { 0,0,32, 32 });
	spText->SetColor({ 0.3f,0.3f,1.0f,0.7f });
	m_objList.push_back(spText);

	std::shared_ptr<MaxWaterLv> spWaterLv;
	spWaterLv = std::make_shared<MaxWaterLv>();
	m_objList.push_back(spWaterLv);

	spWaterLv->SetPlayer(spPlayer);

	// �J�����̏�����
	std::shared_ptr<TPSCam> spTps;
	spTps = std::make_shared<TPSCam>();
//	spTps->RegistHitObject(spGround);	/* Ground�Ƃ̓����蔻�� */
	m_objList.push_back(spTps);

	spTps->SetTarget(spPlayer);
	spPlayer->SetCamera(spTps);

	spTps->SetEnemy(spEnemy);
	spTps->RegistHitObj(spEnemy);	/* spEnemy�Ƃ̓����蔻�� */
}
