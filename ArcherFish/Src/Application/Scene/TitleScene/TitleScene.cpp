#include "TitleScene.h"

// 背景＆タイトルロゴ
#include "../../Object/TextDraw/TextDraw.h"
#include "../../Object/TextDraw/SceneUI/Enter/Enter.h"

void TitleScene::Event()
{
	// シーン切替(Title→Game)
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		if (!m_pushAct)
		{
//			KdAudioManager::Instance().Play("Asset/Sounds/SE/PushButton.wav");
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::Game);
//			KdAudioManager::Instance().StopAllSound();
			m_pushAct = true;
		}
	}
	else { m_pushAct = false; }
}

void TitleScene::Init()
{
	//	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Title.wav", true);

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
