#include "ResultScene.h"
#include "../../Object/TextDraw/BackDrop/BackDrop.h"

void ResultScene::Event()
{
	// シーン切替(Result→Title)
	if (GetAsyncKeyState('O') & 0x8000)
	{
//		KdAudioManager::Instance().Play("Asset/Sounds/SE/PushButton.wav");
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
//		KdAudioManager::Instance().StopAllSound();
	}
}

void ResultScene::Init()
{
//	KdAudioManager::Instance().Play("Asset/Sounds/SE/GameSet.wav");
//	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Katidoki.wav", true);

	// マウスポインタ表示
	ShowCursor(true);

	/* オブジェクトの初期化 */
	// シーン
	std::shared_ptr<BackDrop> spBD;
	spBD = std::make_shared<BackDrop>();
	m_objList.push_back(spBD);
}