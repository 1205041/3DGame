#include "LoseScene.h"
#include "../../../Object/ObjBase.h"

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
	std::shared_ptr<ObjBase> spObjLose;
	spObjLose = std::make_shared<ObjBase>();
	spObjLose->SetTexture("Asset/Textures/Result/lose.png");
	m_objList.push_back(spObjLose);
}