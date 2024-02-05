#include "GameScene.h"

#include "../../Object/ObjBase.h"

// �L�����N�^
#include "../../Object/Player/Player.h"
#include "../../Object/Enemy/Enemy.h"

// �n�`
#include "../../Object/Terrain/SphereDome/UnderWater/UnderWater.h"
#include "../../Object/Terrain/SphereDome/SphereGround.h"
#include "../../Object/Terrain/WaterSurface/WaterSurface.h"

// �Q�[��UI
#include "../../Object/TextDraw/TextDraw.h"
#include "../../Object/TextDraw/SceneUI/ShootBar/WaterBar.h"
#include "../../Object/TextDraw/SceneUI/MaxWaterLv/MaxWaterLv.h"

// �J����
#include "../../Object/Camera/TPSCam/TPSCam.h"

void GameScene::Event()
{
	// �V�[���ؑ�(Game��Result)
	if (KdInputManager::Instance().GetButtonState("ChangeResult"))
	{
		if (!m_SceneFlg)
		{
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::Result);
			KdAudioManager::Instance().StopAllSound();
			Sleep(300); // ��0.3�b
			m_SceneFlg = true;
		}
	}
	else { m_SceneFlg = false; }

	m_BGMSound->SetVolume(m_BGMVol);

	/* �� ���̒i�K�ł͍X�V����܂��� �I�I */
}

void GameScene::Init()
{
	// �}�E�X�|�C���^��\��
	ShowCursor(false);

	// �����t�H�O
	KdShaderManager::Instance().WorkAmbientController().SetFogEnable(true, true);
 	// �����t�H�O�̐ݒ�
	KdShaderManager::Instance().WorkAmbientController().SetDistanceFog(m_disColor, m_density);
	// �����t�H�O�̐ݒ�
	KdShaderManager::Instance().WorkAmbientController().SetheightFog(m_heigtColor, m_topVal, m_bottomVal, m_distance);

	/* ���������� */
	// BGM�ESE
	std::shared_ptr<KdSoundInstance> m_SESound =
		KdAudioManager::Instance().Play("Asset/Sounds/SE/GameStart.wav", false);
	m_SESound->SetVolume(0.6f);

	m_BGMSound = KdAudioManager::Instance().Play("Asset/Sounds/BGM/GameUnderWater.wav", true);

	// �f�B���N�V���i�����C�g(���s��)
//	KdShaderManager::Instance().WorkAmbientController().SetDirLight({ 0,-1,0.3f }, { 3,3,3 });

	/* �I�u�W�F�N�g������ */
	/* === �n�` === */
	std::shared_ptr<UnderWater> spUnWater;
	spUnWater = std::make_shared<UnderWater>();
	m_objList.push_back(spUnWater);

	std::shared_ptr<SphereGround> spGrndDm;
	spGrndDm = std::make_shared<SphereGround>();
	m_objList.push_back(spGrndDm);

	std::shared_ptr<WaterSurface> spWaterSf;
	spWaterSf = std::make_shared<WaterSurface>();
	m_objList.push_back(spWaterSf);

	/* === �L���� === */
	std::shared_ptr<Player> spPlayer;
	spPlayer = std::make_shared<Player>();
	m_objList.push_back(spPlayer);

	// �n�`�̓����蔻��@
	spUnWater->RegistHitObj(spPlayer); // Player�Ƃ̓����蔻��
	spPlayer->RegistHitObj(spUnWater); // SkySphere�Ƃ̓����蔻��

	/* === �G�l�~�[ === */
	std::shared_ptr<Enemy> spEnemy;
	spEnemy = std::make_shared<Enemy>();
	spEnemy->SetPos({ 0.0f,15.0f ,3.0f });
	m_objList.push_back(spEnemy);

	// �ˌ��̓����蔻��
	spEnemy->RegistHitObj(spPlayer); // spPlayer�Ƃ̓����蔻��
	spPlayer->RegistHitObj(spEnemy); // spEnemy�Ƃ̓����蔻��
	spPlayer->SetEnemy(spEnemy);
	

	// �n�`�̓����蔻��A
//	spGrndDm->RegistHitObj(spEnemy); // spEnemy�Ƃ̓����蔻��
//	spEnemy->RegistHitObj(spGrndDm); // SkySphere�Ƃ̓����蔻��

	/* �Q�[��UI������ */
	std::shared_ptr<TextDraw> spText;
	spText = std::make_shared<TextDraw>();
	spText->SetLoadText("Asset/Textures/SceneUI/Game/SightTP.png");
	spText->SetColor({ 5.0f,0.0f,0.0f,0.7f });
	spText->SetRectAng({ 0,0,32, 32 });
	m_objList.push_back(spText);

	std::shared_ptr<WaterBar> spBar;
	spBar = std::make_shared<WaterBar>();
	m_objList.push_back(spBar);

	spPlayer->SetWaterBar(spBar);

	std::shared_ptr<MaxWaterLv> spWaterLv;
	spWaterLv = std::make_shared<MaxWaterLv>();
	spWaterLv->SetPixel({ 0.0f,180.0f });
	m_objList.push_back(spWaterLv);

	spWaterLv->SetPlayer(spPlayer);

	/* �J���������� */
	std::shared_ptr<TPSCam> spTps;
	spTps = std::make_shared<TPSCam>();
	m_objList.push_back(spTps);

	spTps->SetTarget(spPlayer);
	spPlayer->SetCamera(spTps);

	spTps->RegistHitObj(spEnemy);	/* spEnemy�Ƃ̓����蔻�� */
}
