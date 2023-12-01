#include "GameScene.h"

#include "../../Object/ObjBase.h"

// キャラクタ
#include "../../Object/Player/Player.h"
#include "../../Object/Enemy/Enemy.h"

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
		if (!m_pushAct)
		{
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::Result);
			Sleep(300); // 約0.3秒
			m_pushAct = true;
		}
	}
	else { m_pushAct = false; }

	/* ※ この段階では更新されません ！！ */
}

void GameScene::Init()
{
//	KdAudioManager::Instance().Play("Asset/Sounds/SE/GameStart.wav");
//	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Game.wav", true);

	// マウスポインタ非表示
	ShowCursor(false);

	/* オブジェクトの初期化 */
	// スカイスフィア
	std::shared_ptr<SphereGround> spGrndDm;
	spGrndDm = std::make_shared<SphereGround>();
	m_objList.push_back(spGrndDm);

	// キャラ
	std::shared_ptr<Player> spPlayer;
	spPlayer = std::make_shared<Player>();
	m_objList.push_back(spPlayer);

	spGrndDm->RegistHitObj(spPlayer);	/* Playerとの当たり判定 */
	spPlayer->RegistHitObj(spGrndDm);	/* SkySphereとの当たり判定 */

	// エネミー
	std::shared_ptr<Enemy> spEnemy;
//	for (int i = 1; i < 5; i++)
//	{
		spEnemy = std::make_shared<Enemy>();
//		spEnemy->SetPos({ 0.0f,15.0f ,3.0f * i });
		spEnemy->SetPos({ 0.0f,15.0f ,3.0f });
		m_objList.push_back(spEnemy);

		spEnemy->RegistHitObj(spPlayer);	/* spPlayerとの当たり判定 */
		spPlayer->RegistHitObj(spEnemy);	/* spEnemyとの当たり判定 */
		spPlayer->SetEnemy(spEnemy);
//	}

	// ゲームUI
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

	// カメラの初期化
	std::shared_ptr<TPSCam> spTps;
	spTps = std::make_shared<TPSCam>();
	m_objList.push_back(spTps);

	spTps->SetTarget(spPlayer);
	spPlayer->SetCamera(spTps);

	spTps->RegistHitObj(spEnemy);	/* spEnemyとの当たり判定 */
}
