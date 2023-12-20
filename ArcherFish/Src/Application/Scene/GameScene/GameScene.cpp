#include "GameScene.h"

#include "../../Object/ObjBase.h"

// キャラクタ
#include "../../Object/Player/Player.h"
#include "../../Object/Enemy/Enemy.h"

// 地形
#include "../../Object/Terrain/SphereDome/UnderWater/UnderWater.h"
#include "../../Object/Terrain/SphereDome/SphereGround.h"
#include "../../Object/Terrain/WaterSurface/WaterSurface.h"

// ゲームUI
#include "../../Object/TextDraw/TextDraw.h"
#include "../../Object/TextDraw/SceneUI/MaxWaterLv/MaxWaterLv.h"

// カメラ
#include "../../Object/Camera/TPSCam/TPSCam.h"

void GameScene::Event()
{
	// シーン切替(Game→Result)
	if (KdInputManager::Instance().GetButtonState("ChangeGame"))
	{
		if (!m_SceneFlg)
		{
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::Result);
			KdAudioManager::Instance().StopAllSound();
			Sleep(300); // 約0.3秒
			m_SceneFlg = true;
		}
	}
	else { m_SceneFlg = false; }	

	m_BGMSound->SetVolume(m_BGMVol);
	/* ※ この段階では更新されません ！！ */
}

void GameScene::Init()
{
	// BGM・SE
//	m_SESound = KdAudioManager::Instance().Play("Asset/Sounds/SE/GameStart.wav");
	m_BGMSound = KdAudioManager::Instance().Play("Asset/Sounds/BGM/GameUnderWater.wav", true);

	// マウスポインタ非表示
	ShowCursor(false);

	/* オブジェクトの初期化 */
	// 地形
	std::shared_ptr<UnderWater> spUnWater;
	spUnWater = std::make_shared<UnderWater>();
	m_objList.push_back(spUnWater);

	std::shared_ptr<SphereGround> spGrndDm;
	spGrndDm = std::make_shared<SphereGround>();
	m_objList.push_back(spGrndDm);

	std::shared_ptr<WaterSurface> spWaterSf;
	spWaterSf = std::make_shared<WaterSurface>();
	m_objList.push_back(spWaterSf);

	// キャラ
	std::shared_ptr<Player> spPlayer;
	spPlayer = std::make_shared<Player>();
	m_objList.push_back(spPlayer);

	/* === 地形の当たり判定 === */
	spUnWater->RegistHitObj(spPlayer); // Playerとの当たり判定
	spPlayer->RegistHitObj(spUnWater); // SkySphereとの当たり判定

	// エネミー
	std::shared_ptr<Enemy> spEnemy;
	spEnemy = std::make_shared<Enemy>();
	spEnemy->SetPos({ 0.0f,15.0f ,3.0f });
	m_objList.push_back(spEnemy);

	/* === 射撃の当たり判定 === */
	spEnemy->RegistHitObj(spPlayer); // spPlayerとの当たり判定
	spPlayer->RegistHitObj(spEnemy); // spEnemyとの当たり判定
	spPlayer->SetEnemy(spEnemy);

	/* === 地形の当たり判定 === */
//	spGrndDm->RegistHitObj(spEnemy); // spEnemyとの当たり判定
//	spEnemy->RegistHitObj(spGrndDm); // SkySphereとの当たり判定

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

void GameScene::ImGuiUpdate()
{
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(250, 100), ImGuiCond_Once);

	if (KdInputManager::Instance().GetButtonState("BGMVolUp"))
	{
		m_BGMVol += 0.01f;
		if (m_BGMVol >= 1.0f) { m_BGMVol = 1.0f; }
	}
	if (KdInputManager::Instance().GetButtonState("BGMVolDown"))
	{
		m_BGMVol -= 0.01f;
		if (m_BGMVol <= 0.1f) { m_BGMVol = 0.1f; }
	}

	// デバッグウィンドウ
	if (ImGui::Begin("GameScene : Debug Window"))
	{
		ImGui::SliderFloat("BGMVol", &m_BGMVol, 0.1f, 1.0f);
//		ImGui::SliderFloat("SEVol", &m_SEVol, 0.1f, 1.0f);
	}
	ImGui::End();
}
