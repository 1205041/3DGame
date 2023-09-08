#include "TitleScene.h"
#include "../../Object/ObjBase.h"

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
	
	/* オブジェクトの初期化 */
	// シーン
	std::shared_ptr<ObjBase> spObjTitle;
	spObjTitle = std::make_shared<ObjBase>();
	spObjTitle->SetTexture("Asset/Textures/Title/Title.png");
	m_objList.push_back(spObjTitle);
}
