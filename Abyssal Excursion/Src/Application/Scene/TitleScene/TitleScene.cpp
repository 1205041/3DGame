#include "TitleScene.h"
#include "../../Object/2Ddraw/2Ddraw.h"

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
	std::shared_ptr<twoDdraw> spTitle;
	spTitle = std::make_shared<twoDdraw>();
	spTitle->SetTexture("Asset/Textures/Title/Title.png");
	m_objList.push_back(spTitle);
}
