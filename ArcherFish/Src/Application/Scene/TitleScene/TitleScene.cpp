#include "TitleScene.h"

// 背景＆タイトルロゴ
#include "../../Object/TextDraw/TextDraw.h"
#include "../../Object/TextDraw/BackGround/BackEffect/Bubble/Bubble.h"
#include "../../Object/TextDraw/SceneUI/Enter/Enter.h"

void TitleScene::Event()
{
	// シーン切替(Title→Game)
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
//		KdAudioManager::Instance().Play("Asset/Sounds/SE/PushButton.wav");
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Game);
//		KdAudioManager::Instance().StopAllSound();
	}
}

void TitleScene::Init()
{
//	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Title.wav", true);
	
	// オブジェクトの初期化
//	std::shared_ptr<TextDraw> spText;
//	spText = std::make_shared<TextDraw>();
//	spText->SetLoadText("Asset/Textures/BackDrop/BDimg.png", { 0.0f,0.0f }, { 0,0,1280,720 });
//	m_objList.push_back(spText);

//	spText = std::make_shared<TextDraw>();
//	spText->SetLoadText("Asset/Textures/SceneUI/Title/Logo/TitleLogo.png", { 0.0f,0.0f }, { 0,0,450, 90 });
//	m_objList.push_back(spText);

//	std::shared_ptr<Enter> spEnter;
//	spEnter = std::make_shared<Enter>();
//	m_objList.push_back(spEnter);

	std::shared_ptr<Bubble> spBublle;
	spBublle = std::make_shared<Bubble>();
	m_objList.push_back(spBublle);
}
