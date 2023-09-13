#include "GameScene.h"

#include "../../Object/ObjBase.h"

// キャラクタ
#include "../../Object/Player/Player.h"

// 地形
#include "../../Object/Terrain/Stage/Stage.h"
#include "../../Object/Terrain/Ground/Ground.h"
#include "../../Object/SkySphere/SkySphere.h"

// カメラ
#include "../../Object/Camera/TPS/TPS.h"

void GameScene::Event()
{
	// シーン切替(Game→Title)
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
	

	/* ※ この段階では更新されません ！！ */
}

void GameScene::Init()
{
//	KdAudioManager::Instance().Play("Asset/Sounds/SE/GameStart.wav");
//	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Game.wav", true);

	/* オブジェクトの初期化 */
	// 地形
	std::shared_ptr<Stage> spStage;
	spStage = std::make_shared<Stage>();
	m_objList.push_back(spStage);

	std::shared_ptr<SkySphere> spSkySp;
	spSkySp = std::make_shared<SkySphere>();
	m_objList.push_back(spSkySp);

	// 通常の地面
	std::shared_ptr<Ground> spGround;
	spGround = std::make_shared<Ground>();
	spGround->SetScale({ 10.0f, 5.0f, 50.0f });
	spGround->SetPos({ 0,1.0f,0 });
	m_objList.push_back(spGround);

	spGround = std::make_shared<Ground>();
	spGround->SetScale({ 10.0f, 5.0f, 10.0f });
	spGround->SetPos({ -135.0f,1.0f,0.0f });
	m_objList.push_back(spGround);

	spGround = std::make_shared<Ground>();
	spGround->SetScale({ 10.0f, 5.0f, 10.0f });
	spGround->SetPos({ 135.0f,1.0f,0.0f });
	m_objList.push_back(spGround);

	spGround = std::make_shared<Ground>();
	spGround->SetScale({ 10.0f, 5.0f, 10.0f });
	spGround->SetPos({ 270.0f,1.0f,0.0f });
	m_objList.push_back(spGround);

	// キャラ
	std::shared_ptr<Player> spPlayer;	
	spPlayer = std::make_shared<Player>();
	spPlayer->SetPos({ 0,6.0f,0 });
	spPlayer->RegistHitObj(spGround);	/* Groundとの当たり判定 */
	spPlayer->RegistHitObj(spSkySp);	/* SkySphereとの当たり判定 */
	spPlayer->RegistHitObj(spStage);	/* Stageとの当たり判定 */
	m_objList.push_back(spPlayer);
	
	// カメラの初期化
	std::shared_ptr<TPS> spTps;
	spTps = std::make_shared<TPS>();
	spTps->SetTarget(spPlayer);
	spTps->RegistHitObject(spGround);	/* Groundとの当たり判定 */
	spPlayer->SetCamera(spTps);
	m_objList.push_back(spTps);
}
