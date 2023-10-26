#include "GameScene.h"

#include "../../Object/ObjBase.h"

// キャラクタ
#include "../../Object/Player/Player.h"
#include "../../Object/Enemy/Enemy.h"

// 地形
#include "../../Object/Terrain/Stage/Stage.h"
#include "../../Object/Terrain/Ground/Ground.h"

// スカイスフィア
#include "../../Object/SphereDome/TropicalDomo/TropicalDome.h"
#include "../../Object/SphereDome/RiverSide/RiverSide.h"

// ゲームUI
#include "../../Object/TextDraw/SceneUI/Game/Sight/Sight.h"

// カメラ
#include "../../Object/Camera/TPS/TPS.h"

void GameScene::Event()
{
	// シーン切替(Game→Result)
	if (GetAsyncKeyState('P') & 0x8000)
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Result);
	}
	

	/* ※ この段階では更新されません ！！ */
}

void GameScene::Init()
{
//	KdAudioManager::Instance().Play("Asset/Sounds/SE/GameStart.wav");
//	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Game.wav", true);

	// マウスポインタ非表示
	ShowCursor(false);

	/* オブジェクトの初期化 */
	// 地形
//	std::shared_ptr<Stage> spStage;
//	spStage = std::make_shared<Stage>();
//	m_objList.push_back(spStage);

	std::shared_ptr<TropicalDome> spTroDm;
	spTroDm = std::make_shared<TropicalDome>();
	m_objList.push_back(spTroDm);

	std::shared_ptr<RiverSide> spRvSd;
	spRvSd = std::make_shared<RiverSide>();
	m_objList.push_back(spRvSd);

	// 通常の地面
	std::shared_ptr<Ground> spGround;
	spGround = std::make_shared<Ground>();
	spGround->SetScale({ 1.0f, 1.0f, 1.0f });
 	spGround->SetPos({ 0,-1.5f,0 });
	m_objList.push_back(spGround);

	// ゲームUI
	std::shared_ptr<Sight> spSight;
	spSight = std::make_shared<Sight>();
	m_objList.push_back(spSight);

	// キャラ
	std::shared_ptr<Player> spPlayer;	
	spPlayer = std::make_shared<Player>();
	spPlayer->SetPos({ 0,5.0f,0 });
//	spPlayer->RegistHitObj(spSkySp);	/* SkySphereとの当たり判定 */
//	spPlayer->RegistHitObj(spStage);	/* Stageとの当たり判定 */
	m_objList.push_back(spPlayer);

	// エネミー
	std::shared_ptr<Enemy> spEnemy;
	spEnemy = std::make_shared<Enemy>();
	spEnemy->SetPos({ 0,5.0f,10.0f });
	m_objList.push_back(spEnemy);

	// カメラの初期化
	std::shared_ptr<TPS> spTps;
	spTps = std::make_shared<TPS>();
	spTps->RegistHitObject(spGround);	/* Groundとの当たり判定 */
	spTps->SetTarget(spPlayer);
	spPlayer->SetCamera(spTps);
	m_objList.push_back(spTps);
}
