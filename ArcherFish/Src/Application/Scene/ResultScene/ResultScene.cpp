#include "ResultScene.h"

#include "../../Object/TextDraw/TextDraw.h"
#include "../../Object/TextDraw/SceneUI/PKey/PKey.h"

void ResultScene::Event()
{
	// シーン切替(Result→Title)
	if (KdInputManager::Instance().GetButtonState("ChangeTitle"))
	{
		if (!m_SceneFlg)
		{
//			KdAudioManager::Instance().Play("Asset/Sounds/SE/PushButton.wav");
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
			KdAudioManager::Instance().StopAllSound();
			Sleep(300);
			m_SceneFlg = true;
		}
	}
	else { m_SceneFlg = false; }

	m_BGMSound->SetVolume(GetBGMVol());
	SetBGMVol(m_BGMVol);
}

void ResultScene::Init()
{
	// BGM・SE
//	m_SESound = KdAudioManager::Instance().Play("Asset/Sounds/SE/GameSet.wav");
	m_BGMSound = KdAudioManager::Instance().Play("Asset/Sounds/BGM/ResultCoast.wav", true);

	// マウスポインタ表示
	ShowCursor(true);

	/* オブジェクトの初期化 */
	// シーン
	std::shared_ptr<TextDraw> spText;
	spText = std::make_shared<TextDraw>();
	spText->SetLoadText("Asset/Textures/BackDrop/BDimg.png", { 0,0,1280,720 });
	m_objList.push_back(spText);

	spText = std::make_shared<TextDraw>();
	spText->SetLoadText("Asset/Textures/SceneUI/Logo/Logo.png", { 0,0,450, 90 });
	spText->SetTextPixel({ 0.0f,180.0f });
	m_objList.push_back(spText);

	spText = std::make_shared<TextDraw>();
	spText->SetLoadText("Asset/Textures/SceneUI/Score/Score.png", { 0,0,237,82 });
	spText->SetTextPixel({ -250.0f,0.0f });
	m_objList.push_back(spText);

	std::shared_ptr<PKey> spPKey;
	spPKey = std::make_shared<PKey>();
	spPKey->SetTextPixel({ 450.0f, -300.0f });
	m_objList.push_back(spPKey);
}
