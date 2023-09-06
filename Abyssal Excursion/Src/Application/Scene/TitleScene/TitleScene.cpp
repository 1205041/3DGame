#include "TitleScene.h"

// 地形
#include "../../Object/SkySphere/SkySphere.h"

// タイトルシーン
#include "../../Object/SceneText/TitleScene/TitleText.h"

// カメラ
#include "../../Object/Camera/TPS/TPS.h"

void TitleScene::Event()
{
	// シーン切替(Title→Game)
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		KdAudioManager::Instance().Play("Asset/Sounds/SE/PushButton.wav");
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Game);
		KdAudioManager::Instance().StopAllSound();
	}
}

void TitleScene::Init()
{
//	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Title.wav", true);
	
	/* オブジェクトの初期化 */
	// シーン
	std::shared_ptr<TitleText> spTitle;
	spTitle = std::make_shared<TitleText>();
	m_objList.push_back(spTitle);

	// 地形
	std::shared_ptr<SkySphere> spSkySp;
	spSkySp = std::make_shared<SkySphere>();
	m_objList.push_back(spSkySp);

	// カメラの初期化
	// カメラの初期化
	std::shared_ptr<TPS> spTps;
	spTps = std::make_shared<TPS>();
	spTps->SetTarget(spTitle);
	m_objList.push_back(spTps);
}
