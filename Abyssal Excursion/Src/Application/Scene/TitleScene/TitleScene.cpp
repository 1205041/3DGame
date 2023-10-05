#include "TitleScene.h"

// 背景＆タイトルロゴ
#include "../../Object/TextDraw/BackDrop/BackDrop.h"
#include "../../Object/TextDraw/SceneUI/Title/TitleLogo/TitleLogo.h"

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
	std::shared_ptr<BackDrop> spBD;
	spBD = std::make_shared<BackDrop>();
	m_objList.push_back(spBD);

	std::shared_ptr<TitleLogo> spTL;
	spTL = std::make_shared<TitleLogo>();
	m_objList.push_back(spTL);
}
