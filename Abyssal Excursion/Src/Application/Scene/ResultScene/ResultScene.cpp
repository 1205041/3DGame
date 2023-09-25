#include "ResultScene.h"
#include "../../Object/2Ddraw/Result2D/Result2D.h"

void ResultScene::Event()
{
	// シーン切替(Result→Title)
	if (GetAsyncKeyState('P') & 0x8000)
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

	/* オブジェクトの初期化 */
	// シーン
	std::shared_ptr<Result2D> spResult;
	spResult = std::make_shared<Result2D>();
	m_objList.push_back(spResult);
}