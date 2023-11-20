#include "GameScene.h"

#include "../../Object/ObjBase.h"

// キャラクタ
#include "../../Object/Player/Player.h"
#include "../../Object/Enemy/Enemy.h"

// 地形
#include "../../Object/Terrain/Stage/Stage.h"

// スカイスフィア
#include "../../Object/SphereDome/SphereGround.h"

// ゲームUI
#include "../../Object/TextDraw/TextDraw.h"

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

	// スカイスフィア
	std::shared_ptr<SphereGround> spGrndDm;
	spGrndDm = std::make_shared<SphereGround>();
	m_objList.push_back(spGrndDm);

	// ゲームUI
	std::shared_ptr<TextDraw> spText;
	spText = std::make_shared<TextDraw>();
	spText->SetLoadText("Asset/Textures/SceneUI/Game/SightTP.png", { -16.0f,16.0f }, { 0,0,32, 32 });
	spText->SetColor({ 1.0f,1.0f,1.0f,0.5f });
	m_objList.push_back(spText);

	// エネミー
	std::shared_ptr<Enemy> spEnemy;
	spEnemy = std::make_shared<Enemy>();
	spEnemy->SetPos({ 0.0f,5.0f,0.0f });
	m_objList.push_back(spEnemy);

	// キャラ
	std::shared_ptr<Player> spPlayer;	
	spPlayer = std::make_shared<Player>();
	spPlayer->RegistHitObj(spGrndDm);	/* SkySphereとの当たり判定 */
//	spPlayer->RegistHitObj(spStage);	/* Stageとの当たり判定 */
	spPlayer->SetEnemy(spEnemy);
	m_objList.push_back(spPlayer);

	// カメラの初期化
	std::shared_ptr<TPS> spTps;
	spTps = std::make_shared<TPS>();
//	spTps->RegistHitObject(spGround);	/* Groundとの当たり判定 */
	spTps->SetTarget(spPlayer);
	spPlayer->SetCamera(spTps);
	m_objList.push_back(spTps);
}
