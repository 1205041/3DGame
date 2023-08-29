#include "GameScene.h"

// �n�`
#include "../../Object/Ground/Ground.h"
#include "../../Object/SkySphere/SkySpher.h"

// �J����
#include "../../Camera/TPSC/TPSC.h"

void GameScene::Event()
{
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (!switchFlg)
		{
			if (!ImGuiFlg) { ImGuiFlg = true; }
			else { ImGuiFlg = false; }
			switchFlg = true;
		}
	}
	else { switchFlg = false; }

	// �V�[���ؑ�(Game��Title)
	if (GetAsyncKeyState('P') & 0x8000)
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
	}

	Math::Matrix transMat;
	transMat = Math::Matrix::CreateTranslation
	(Math::Vector3(0, 0, -5));
	m_camera->SetCameraMatrix(transMat);
	/* �����̒i�K�ł͍X�V����܂���I�I */
}

void GameScene::Init()
{
	// �X�J�C�X�t�B�A
	std::shared_ptr<SkySphere> skySphere;
	skySphere = std::make_shared<SkySphere>();
	m_objList.push_back(skySphere);

	// �n�`
	std::shared_ptr<Ground> ground;
	ground = std::make_shared<Ground>();
	ground->SetScale({ 5.0f, 1.0f, 1.0f });
	ground->SetPos({ 0,-2.0f,0 });
	m_objList.push_back(ground);

	ground = std::make_shared<Ground>();
	ground->SetScale({ 5.0f, 1.0f, 1.0f });
	ground->SetPos({ 0,2.0f,0 });
	m_objList.push_back(ground);

	m_camera = std::make_unique<KdCamera>();
	m_camera->SetProjectionMatrix(60);

	// �J�����̏�����
	std::shared_ptr<TPSC> spTps;
	spTps = std::make_shared<TPSC>();
//	spTps->SetTarget(spPlayer);
//	spPlayer->SetCamera(spTps);
	m_objList.push_back(spTps);

	ImGuiFlg = false;
	switchFlg = false;

}

void GameScene::ImGuiUpdate()
{
	if (!ImGuiFlg) { return; }
	
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Once);

	// �E�B���h�E�J�n""�̒��̕����͑��̃E�B���h�E�Ɣ��Ȃ��悤�ɂ��邱�ƁI
	if (ImGui::Begin("GameSceneDebugWindow"))
	{
		// �E�B���h�E��܂肽����ł��Ȃ���Ԃ��ƁA���̒������s�����

	}
	ImGui::End();
}
