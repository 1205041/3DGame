#include "LoseScene.h"
#include "../../../Object/2Ddraw/2Ddraw.h"

void LoseScene::Event()
{
	// シーン切替(Result→Title)
	if (GetAsyncKeyState('P') & 0x8000)
	{
		KdAudioManager::Instance().Play("Asset/Sounds/SE/PushButton.wav");
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
		KdAudioManager::Instance().StopAllSound();
	}
}

void LoseScene::Init()
{
	//	KdAudioManager::Instance().Play("Asset/Sounds/SE/GameSet.wav");
	//	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Makeikusa.wav", true);

		/* オブジェクトの初期化 */
		// シーン
	std::shared_ptr<twoDdraw> spLose;
	spLose = std::make_shared<twoDdraw>();
	spLose->SetTexture("Asset/Textures/Result/lose.png");
	m_objList.push_back(spLose);
}