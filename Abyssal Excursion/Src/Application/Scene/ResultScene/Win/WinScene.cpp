#include "WinScene.h"
#include "../../../Object/2Ddraw/2Ddraw.h"

void WinScene::Event()
{
	// シーン切替(Result→Title)
	if (GetAsyncKeyState('P') & 0x8000)
	{
//		KdAudioManager::Instance().Play("Asset/Sounds/SE/PushButton.wav");
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
//		KdAudioManager::Instance().StopAllSound();
	}
}

void WinScene::Init()
{
//	KdAudioManager::Instance().Play("Asset/Sounds/SE/GameSet.wav");
//	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Katidoki.wav", true);

	/* オブジェクトの初期化 */
	// シーン
	std::shared_ptr<twoDdraw> spWin;
	spWin = std::make_shared<twoDdraw>();
	spWin->SetTexture("Asset/Textures/Result/win.png");
	spWin->SetRectAngle({ 0, 0, 1280, 720 });
	m_objList.push_back(spWin);
}