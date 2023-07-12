#include "GameScene.h"

#include "../../Object/Ground/Ground.h"

void GameScene::Event()
{
	// �V�[���ؑ�(Game��Title)
	if (GetAsyncKeyState('P') & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(SceneManager::SceneType::Title);
	}

	Math::Matrix transMat;
	transMat = Math::Matrix::CreateTranslation
	(Math::Vector3(0, 0, -5));
	m_camera->SetCameraMatrix(transMat);
	/* �����̒i�K�ł͍X�V����܂���I�I */
}

void GameScene::Init()
{
	m_camera = std::make_unique<KdCamera>();
	m_camera->SetProjectionMatrix(60);

	// �n�`
	std::shared_ptr<Ground> ground;
	ground = std::make_shared<Ground>();
	m_objList.push_back(ground);
}

void GameScene::ImGuiUpdate()
{
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) 
	{ 
		if (!ImGuiFlg) 
		{ 
			ImGuiFlg = true;
			return;
		}
		else
		{
			ImGuiFlg = false;
		}
	}
	
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_Once);

	// �E�B���h�E�J�n""�̒��̕����͑��̃E�B���h�E�Ɣ��Ȃ��悤�ɂ��邱�ƁI
	if (ImGui::Begin("GameSceneDebugWindow"))
	{
		// �E�B���h�E��܂肽����ł��Ȃ���Ԃ��ƁA���̒������s�����

	}
	ImGui::End();
}
