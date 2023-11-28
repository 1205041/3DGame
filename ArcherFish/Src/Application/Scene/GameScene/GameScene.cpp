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
#include "../../Object/TextDraw/SceneUI/MaxWaterLv/MaxWaterLv.h"

// カメラ
#include "../../Object/Camera/TPSCam/TPSCam.h"

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

	// エネミー
	std::shared_ptr<Enemy> spEnemy;
	spEnemy = std::make_shared<Enemy>();
	spEnemy->SetPos({ 0.0f,10.0f,3.0f });
	m_objList.push_back(spEnemy);

	// キャラ
	std::shared_ptr<Player> spPlayer;	
	spPlayer = std::make_shared<Player>();
	m_objList.push_back(spPlayer);

	spEnemy->RegistHitObj(spPlayer);	/* spPlayerとの当たり判定 */
	spPlayer->RegistHitObj(spEnemy);	/* spEnemyとの当たり判定 */
	spGrndDm->RegistHitObj(spPlayer);	/* Playerとの当たり判定 */
	spPlayer->RegistHitObj(spGrndDm);	/* SkySphereとの当たり判定 */

	// ゲームUI
	std::shared_ptr<TextDraw> spText;
	spText = std::make_shared<TextDraw>();
	spText->SetLoadText("Asset/Textures/SceneUI/Game/SightTP.png", { 0.0f,0.0f }, { 0,0,32, 32 });
	spText->SetColor({ 0.3f,0.3f,1.0f,0.7f });
	m_objList.push_back(spText);

	std::shared_ptr<MaxWaterLv> spWaterLv;
	spWaterLv = std::make_shared<MaxWaterLv>();
	m_objList.push_back(spWaterLv);

	spWaterLv->SetPlayer(spPlayer);

	// カメラの初期化
	std::shared_ptr<TPSCam> spTps;
	spTps = std::make_shared<TPSCam>();
//	spTps->RegistHitObject(spGround);	/* Groundとの当たり判定 */
	m_objList.push_back(spTps);

	spTps->SetTarget(spPlayer);
	spPlayer->SetCamera(spTps);

	spTps->SetEnemy(spEnemy);
	spTps->RegistHitObj(spEnemy);	/* spEnemyとの当たり判定 */
}
