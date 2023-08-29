#include "GameScene.h"

// 地形
#include "../../Object/Ground/Ground.h"
#include "../../Object/SkySphere/SkySpher.h"

// カメラ
#include "../../Camera/TPSC/TPSC.h"

void GameScene::Event()
{
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (!switchFlg)
		{
			if (!ImGuiFlg) { ImGuiFlg = true; }
			else { ImGuiFlg = false; }
			switchFlg = true;
		}
	}
	else { switchFlg = false; }

	// シーン切替(Game→Title)
	if (GetAsyncKeyState('P') & 0x8000)
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
	}

	Math::Matrix transMat;
	transMat = Math::Matrix::CreateTranslation
	(Math::Vector3(0, 0, -5));
	m_camera->SetCameraMatrix(transMat);
	/* ※この段階では更新されません！！ */
}

void GameScene::Init()
{
	// スカイスフィア
	std::shared_ptr<SkySphere> skySphere;
	skySphere = std::make_shared<SkySphere>();
	m_objList.push_back(skySphere);

	// 地形
	std::shared_ptr<Ground> ground;
	ground = std::make_shared<Ground>();
	ground->SetScale({ 5.0f, 1.0f, 1.0f });
	ground->SetPos({ 0,-2.0f,0 });
	m_objList.push_back(ground);

	ground = std::make_shared<Ground>();
	ground->SetScale({ 5.0f, 1.0f, 1.0f });
	ground->SetPos({ 0,2.0f,0 });
	m_objList.push_back(ground);

	m_camera = std::make_unique<KdCamera>();
	m_camera->SetProjectionMatrix(60);

	// カメラの初期化
	std::shared_ptr<TPSC> spTps;
	spTps = std::make_shared<TPSC>();
//	spTps->SetTarget(spPlayer);
//	spPlayer->SetCamera(spTps);
	m_objList.push_back(spTps);

	ImGuiFlg = false;
	switchFlg = false;

}

void GameScene::ImGuiUpdate()
{
	if (!ImGuiFlg) { return; }
	
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Once);

	// ウィンドウ開始""の中の部分は他のウィンドウと被らないようにすること！
	if (ImGui::Begin("GameSceneDebugWindow"))
	{
		// ウィンドウを折りたたんでいない状態だと、この中が実行される

	}
	ImGui::End();
}
