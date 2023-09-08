#include "WinScene.h"
#include "../../../Object/ObjBase.h"

void WinScene::Event()
{
	// シーン切替(Result→Title)
	if (GetAsyncKeyState('P') & 0x8000)
	{
		KdAudioManager::Instance().Play("Asset/Sounds/SE/PushButton.wav");
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
		KdAudioManager::Instance().StopAllSound();
	}
}

void WinScene::Init()
{
//	KdAudioManager::Instance().Play("Asset/Sounds/SE/GameSet.wav");
//	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Katidoki.wav", true);

	/* オブジェクトの初期化 */
	// シーン
	std::shared_ptr<ObjBase> spObjWin;
	spObjWin = std::make_shared<ObjBase>();
	spObjWin->SetTexture("Asset/Textures/Result/win.png");
	m_objList.push_back(spObjWin);
}