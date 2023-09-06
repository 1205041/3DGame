#include "LoseScene.h"

// 地形
#include "../../../Object/Ground/Ground.h"
#include "../../../Object/SkySphere/SkySphere.h"

// シーン
#include "../../../Object/SceneText/LoseScene/LoseText.h"

// カメラ
#include "../../../Object/Camera/TPS/TPS.h"

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
	std::shared_ptr<LoseText> spLose;
	spLose = std::make_shared<LoseText>();
	m_objList.push_back(spLose);

	// 地形
	std::shared_ptr<Ground> spGround;
	spGround = std::make_shared<Ground>();
	m_objList.push_back(spGround);

	std::shared_ptr<SkySphere> spSkySp;
	spSkySp = std::make_shared<SkySphere>();
	m_objList.push_back(spSkySp);

	// カメラの初期化
	std::shared_ptr<TPS> spTps;
	spTps = std::make_shared<TPS>();
	spTps->SetTarget(spLose);
	m_objList.push_back(spTps);
}