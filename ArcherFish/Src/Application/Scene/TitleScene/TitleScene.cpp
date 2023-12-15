#include "TitleScene.h"

// �w�i���^�C�g�����S
#include "../../Object/TextDraw/TextDraw.h"
#include "../../Object/TextDraw/SceneUI/Enter/Enter.h"

void TitleScene::Event()
{
	// �V�[���ؑ�(Title��Game)
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		if (!m_SceneFlg)
		{
//			KdAudioManager::Instance().Play("Asset/Sounds/SE/PushButton.wav");
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::Game);
			KdAudioManager::Instance().StopAllSound();
			Sleep(300);
			m_SceneFlg = true;
		}
	}
	else { m_SceneFlg = false; }

	m_BGMSound->SetVolume(m_BGMVol);
}

void TitleScene::Init()
{
	// BGM�ESE
	m_BGMSound = KdAudioManager::Instance().Play("Asset/Sounds/BGM/TitleRipple.wav", true);

	// �I�u�W�F�N�g�̏�����
	std::shared_ptr<TextDraw> spText;
	spText = std::make_shared<TextDraw>();
	spText->SetLoadText("Asset/Textures/BackDrop/BDimg.png", { 0,0,1280,720 });
	m_objList.push_back(spText);

	spText = std::make_shared<TextDraw>();
	spText->SetLoadText("Asset/Textures/SceneUI/Logo/Logo.png", { 0,0,450, 90 });
	m_objList.push_back(spText);

	std::shared_ptr<Enter> spEnter;
	spEnter = std::make_shared<Enter>();
	spEnter->SetTextPixel({ 0.0f, -150.0f });
	m_objList.push_back(spEnter);
}

void TitleScene::ImGuiUpdate()
{
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(250, 100), ImGuiCond_Once);

	// �f�o�b�O�E�B���h�E
	if (ImGui::Begin("TitleScene : Debug Window"))
	{
		ImGui::SliderFloat("BGMVol", &m_BGMVol, 0.1f, 1.0f);
	}
	ImGui::End();
}
