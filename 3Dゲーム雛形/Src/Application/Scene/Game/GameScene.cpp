#include "GameScene.h"

#include "../../Object/Ground/Ground.h"

void GameScene::Event()
{
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		ImGuiFlg = true;
	}
	else 
	{
		ImGuiFlg = false;
	}

	// シーン切替(Game→Title)
	if (GetAsyncKeyState('P') & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(SceneManager::SceneType::Title);
	}

	Math::Matrix transMat;
	transMat = Math::Matrix::CreateTranslation
	(Math::Vector3(0, 0, -5));
	m_camera->SetCameraMatrix(transMat);
	/* ※この段階では更新されません！！ */
}

void GameScene::Init()
{
	m_camera = std::make_unique<KdCamera>();
	m_camera->SetProjectionMatrix(60);

	// 地形
	std::shared_ptr<Ground> ground;
	ground = std::make_shared<Ground>();
	m_objList.push_back(ground);

	ImGuiFlg = false;
}

void GameScene::ImGuiUpdate()
{
	if (!ImGuiFlg) { return; }
	
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_Once);

	// ウィンドウ開始""の中の部分は他のウィンドウと被らないようにすること！
	if (ImGui::Begin("GameSceneDebugWindow"))
	{
		// ウィンドウを折りたたんでいない状態だと、この中が実行される

	}
	ImGui::End();
}
