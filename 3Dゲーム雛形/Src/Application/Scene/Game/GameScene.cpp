#include "GameScene.h"

#include "../../Object/Ground/Ground.h"

void GameScene::Event()
{
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
}

void GameScene::ImGuiUpdate()
{
	return;

	// ウィンドウ開始"WindowName"の部分は他のウィンドウと被らないようにすること！
	if (ImGui::Begin("WindowName"))
	{
		// ウィンドウを折りたたんでいない状態だと、この中が実行される

	}
	ImGui::End();
}
