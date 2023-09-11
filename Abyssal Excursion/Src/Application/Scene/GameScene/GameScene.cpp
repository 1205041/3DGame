#include "GameScene.h"

#include "../../Object/ObjBase.h"

// �L�����N�^
#include "../../Object/Player/Player.h"

// �n�`
#include "../../Object/Terrain/Stage/Stage.h"
#include "../../Object/SkySphere/SkySphere.h"

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
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Win);
	}
	if (GetAsyncKeyState('I') & 0x8000)
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Lose);
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

	// �L����
	std::shared_ptr<Player> spPlayer;	
	spPlayer = std::make_shared<Player>();
	spPlayer->RegistHitObj(spStage);/* Stage�Ƃ̓����蔻�� */
	m_objList.push_back(spPlayer);
	
	// �J�����̏�����
	std::shared_ptr<TPS> spTps;
	spTps = std::make_shared<TPS>();
	spTps->SetTarget(spPlayer);
	spTps->RegistHitObject(spStage);/* Stage�Ƃ̓����蔻�� */
	spPlayer->SetCamera(spTps);
	m_objList.push_back(spTps);
}
