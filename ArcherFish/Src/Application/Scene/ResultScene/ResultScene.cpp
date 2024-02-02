#include "ResultScene.h"

#include "../../Object/TextDraw/TextDraw.h"
#include "../../Object/TextDraw/SceneUI/PushKey/PushKey.h"
#include "../../Object/TextDraw/SceneEffect/Bubble/Bubble.h"

void ResultScene::Event()
{
	// �V�[���ؑ�(Result��Title)
	if (KdInputManager::Instance().GetButtonState("ChangeTitle"))
	{
		if (!m_SceneFlg)
		{
//			KdAudioManager::Instance().Play("Asset/Sounds/SE/PushButton.wav");
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
			KdAudioManager::Instance().StopAllSound();
			Sleep(300);
			m_SceneFlg = true;
		}
	}
	else { m_SceneFlg = false; }

	m_BGMSound->SetVolume(m_BGMVol);
}

void ResultScene::Init()
{
	// �}�E�X�|�C���^�\��
	ShowCursor(true);

	/* ���������� */
	// BGM�ESE
//	m_SESound = KdAudioManager::Instance().Play("Asset/Sounds/SE/GameSet.wav");
	m_BGMSound = KdAudioManager::Instance().Play("Asset/Sounds/BGM/ResultCoast.wav", true);

	/* �I�u�W�F�N�g������ */
	// �w�i�摜
	std::shared_ptr<TextDraw> spText;
	spText = std::make_shared<TextDraw>();
	spText->SetLoadText("Asset/Textures/BackDrop/BDimg.png");
	spText->SetRectAng({ 0,0,1280,720 });
	m_objList.push_back(spText);

	// �A
	std::shared_ptr<Bubble> spBub;
	for (int i = 0; i < spBub->GetBubbleNum(); i++)
	{
		spBub = std::make_shared<Bubble>();
		m_objList.push_back(spBub);
	}

	// ���S
	spText = std::make_shared<TextDraw>();
	spText->SetLoadText("Asset/Textures/SceneUI/Logo/Logo.png");
	spText->SetPixel({ 0.0f,180.0f });
	spText->SetRectAng({ 0,0,450, 90 });
	m_objList.push_back(spText);

	// �X�R�A
	spText = std::make_shared<TextDraw>();
	spText->SetLoadText("Asset/Textures/SceneUI/Score/Score.png");
	spText->SetPixel({ -250.0f,0.0f });
	spText->SetRectAng({ 0,0,237,82 });
	m_objList.push_back(spText);

	// �{�^��
	std::shared_ptr<PushKey> spPushKey;
	spPushKey = std::make_shared<PushKey>();
	spPushKey->SetLoadText("Asset/Textures/SceneUI/Key/PKey.png");
	spPushKey->SetPixel({ 450.0f, -300.0f });
	spPushKey->SetRectAng({ 0,0, 300, 60 });
	m_objList.push_back(spPushKey);
}
