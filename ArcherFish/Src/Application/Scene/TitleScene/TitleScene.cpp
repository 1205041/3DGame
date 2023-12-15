#include "TitleScene.h"

// 背景＆タイトルロゴ
#include "../../Object/TextDraw/TextDraw.h"
#include "../../Object/TextDraw/SceneUI/Enter/Enter.h"

void TitleScene::Event()
{
	// シーン切替(Title→Game)
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		if (!m_SceneFlg)
		{
//			KdAudioManager::Instance().Play("Asset/Sounds/SE/PushButton.wav");
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::Game);
			KdAudioManager::Instance().StopAllSound();
			Sleep(300);
			m_SceneFlg = true;
		}
	}
	else { m_SceneFlg = false; }

	m_BGMSound->SetVolume(m_BGMVol);
}

void TitleScene::Init()
{
	// BGM・SE
	m_BGMSound = KdAudioManager::Instance().Play("Asset/Sounds/BGM/TitleRipple.wav", true);

	// オブジェクトの初期化
	std::shared_ptr<TextDraw> spText;
	spText = std::make_shared<TextDraw>();
	spText->SetLoadText("Asset/Textures/BackDrop/BDimg.png", { 0,0,1280,720 });
	m_objList.push_back(spText);

	spText = std::make_shared<TextDraw>();
	spText->SetLoadText("Asset/Textures/SceneUI/Logo/Logo.png", { 0,0,450, 90 });
	m_objList.push_back(spText);

	std::shared_ptr<Enter> spEnter;
	spEnter = std::make_shared<Enter>();
	spEnter->SetTextPixel({ 0.0f, -150.0f });
	m_objList.push_back(spEnter);
}

void TitleScene::ImGuiUpdate()
{
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(250, 100), ImGuiCond_Once);

	// デバッグウィンドウ
	if (ImGui::Begin("TitleScene : Debug Window"))
	{
		ImGui::SliderFloat("BGMVol", &m_BGMVol, 0.1f, 1.0f);
	}
	ImGui::End();
}
