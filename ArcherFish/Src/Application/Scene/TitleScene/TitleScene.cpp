#include "TitleScene.h"

// 2D表示
#include "../../Object/TextDraw/TextDraw.h"
#include "../../Object/TextDraw/SceneUI/PushKey/PushKey.h"
#include "../../Object/TextDraw/SceneEffect/Bubble/Bubble.h"

void TitleScene::Event()
{
	// シーン切替(Title→Game)
	if (KdInputManager::Instance().GetButtonState("ChangeGame"))
	{
		if (!m_SceneFlg)
		{
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
	/* 音響初期化 */
	// BGM
	m_BGMSound = KdAudioManager::Instance().Play("Asset/Sounds/BGM/TitleRipple.wav", true);

	/* オブジェクト初期化 */
	// 背景画像
	std::shared_ptr<TextDraw> spText;
	spText = std::make_shared<TextDraw>();
	spText->SetLoadText("Asset/Textures/BackDrop/BDimg.png");
	spText->SetRectAng({ 0,0,1280,720 });
	m_objList.push_back(spText);

	// 泡
	std::shared_ptr<Bubble> spBub;
	for (int i = 0; i < spBub->GetBubblei(); i++)
	{
		spBub = std::make_shared<Bubble>();
		m_objList.push_back(spBub);
	}

	// ロゴ
	spText = std::make_shared<TextDraw>();
	spText->SetLoadText("Asset/Textures/SceneUI/Logo/Logo.png");
	spText->SetPixel({ 0.0f,80.0f });
	spText->SetRectAng({ 0,0,450, 90 });
	m_objList.push_back(spText);

	// ボタン
	std::shared_ptr<PushKey> spPushKey;
	spPushKey = std::make_shared<PushKey>();
	spPushKey->SetLoadText("Asset/Textures/SceneUI/Key/Enter.png");
	spPushKey->SetPixel({ 0.0f, -140.0f });
	spPushKey->SetRectAng({ 0, 0, 400, 60 });
	m_objList.push_back(spPushKey);
}
