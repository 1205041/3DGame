#include "GameScene.h"

#include "../../Object/ObjBase.h"

// �L�����N�^
#include "../../Object/Player/Player.h"
#include "../../Object/Enemy/Enemy.h"

// �X�J�C�X�t�B�A
#include "../../Object/Terrain/SphereDome/UnderWater/UnderWater.h"
#include "../../Object/Terrain/SphereDome/SphereGround.h"
#include "../../Object/Terrain/WaterSurface/WaterSurface.h"

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
		if (!m_pushAct)
		{
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::Result);
			Sleep(300); // ��0.3�b
			m_pushAct = true;
		}
	}
	else { m_pushAct = false; }

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
	std::shared_ptr<UnderWater> spUnWater;
	spUnWater = std::make_shared<UnderWater>();
	m_objList.push_back(spUnWater);

	std::shared_ptr<SphereGround> spGrndDm;
	spGrndDm = std::make_shared<SphereGround>();
	m_objList.push_back(spGrndDm);

	std::shared_ptr<WaterSurface> spWaterSf;
	spWaterSf = std::make_shared<WaterSurface>();
	m_objList.push_back(spWaterSf);

	// �L����
	std::shared_ptr<Player> spPlayer;
	spPlayer = std::make_shared<Player>();
	m_objList.push_back(spPlayer);

	/* === �n�`�̓����蔻�� === */
	// Player�Ƃ̓����蔻��
	spUnWater->RegistHitObj(spPlayer);
	spGrndDm->RegistHitObj(spPlayer);
	// SkySphere�Ƃ̓����蔻��
	spPlayer->RegistHitObj(spGrndDm);
	spPlayer->RegistHitObj(spUnWater);

	// �G�l�~�[
	std::shared_ptr<Enemy> spEnemy;
//	for (int i = 1; i < 5; i++)
//	{
		spEnemy = std::make_shared<Enemy>();
//		spEnemy->SetPos({ 0.0f,15.0f ,3.0f * i });
		spEnemy->SetPos({ 0.0f,15.0f ,3.0f });
		m_objList.push_back(spEnemy);

		spEnemy->RegistHitObj(spPlayer);	/* spPlayer�Ƃ̓����蔻�� */
		spPlayer->RegistHitObj(spEnemy);	/* spEnemy�Ƃ̓����蔻�� */
		spPlayer->SetEnemy(spEnemy);
//	}

	// �Q�[��UI
	std::shared_ptr<TextDraw> spText;
	spText = std::make_shared<TextDraw>();
	spText->SetLoadText("Asset/Textures/SceneUI/Game/SightTP.png", { 0,0,32, 32 });
	spText->SetColor({ 0.0f,1.0f,1.0f,0.8f });
	m_objList.push_back(spText);

	std::shared_ptr<MaxWaterLv> spWaterLv;
	spWaterLv = std::make_shared<MaxWaterLv>();
	spWaterLv->SetTextPixel({ 0.0f,180.0f });
	m_objList.push_back(spWaterLv);

	spWaterLv->SetPlayer(spPlayer);

	// �J�����̏�����
	std::shared_ptr<TPSCam> spTps;
	spTps = std::make_shared<TPSCam>();
	m_objList.push_back(spTps);

	spTps->SetTarget(spPlayer);
	spPlayer->SetCamera(spTps);

	spTps->RegistHitObj(spEnemy);	/* spEnemy�Ƃ̓����蔻�� */
}
