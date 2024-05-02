#include "SceneBase.h"

void SceneBase::PreUpdate()
{
	/* �I�u�W�F�N�g���X�g�̐��� */
	auto it = m_objList.begin();
	while (it != m_objList.end())
	{
		// �����؂�
		if ((*it)->IsExpired()) { it = m_objList.erase(it); }
		else { ++it; }
	}
}

void SceneBase::Update()
{
	for (auto& obj : m_objList) { obj->Update(); }

	Event();

	// imGui�\���ؑ�
	if (KdInputManager::GetInstance().GetButtonState("ImGuiOn")) { m_pushAct = true; }
	if (KdInputManager::GetInstance().GetButtonState("ImGuiOff")) { m_pushAct = false; }
}

void SceneBase::PostUpdate()
{
	for (auto& obj : m_objList) { obj->PostUpdate(); }
}

void SceneBase::PreDraw()
{
	for (auto& obj : m_objList) { obj->PreDraw(); }
}

void SceneBase::DrawLit()
{
	// ===== ===== ===== ===== ===== ===== ===== =====
	// �����Ղ�I�u�W�F�N�g(�s�����ȕ��̂�2D�L����)
	// ��Begin��End�̊Ԃɂ܂Ƃ߂�Draw����
	// ===== ===== ===== ===== ===== ===== ===== =====
	
	KdShaderManager::GetInstance().m_HD2DShader.BeginGenerateDepthMapFromLight();
	{
		for (auto& obj : m_objList) { obj->GenerateDepthMapFromLight(); }
	}
	KdShaderManager::GetInstance().m_HD2DShader.EndGenerateDepthMapFromLight();

	// ===== ===== ===== ===== ===== ===== ===== =====
	// �A�e�̂���I�u�W�F�N�g(�s�����ȕ��̂�2D�L����)
	// ��Begin��End�̊Ԃɂ܂Ƃ߂�Draw����
	// ===== ===== ===== ===== ===== ===== ===== =====
	KdShaderManager::GetInstance().m_HD2DShader.BeginLit();
	{
		for (auto& obj : m_objList) { obj->DrawLit(); }
	}
	KdShaderManager::GetInstance().m_HD2DShader.EndLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// �A�e�̂Ȃ��I�u�W�F�N�g(�����ȕ������܂ޕ��̂�G�t�F�N�g)
	// ��Begin��End�̊Ԃɂ܂Ƃ߂�Draw����
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	KdShaderManager::GetInstance().m_HD2DShader.BeginUnLit();
	{
		for (auto& obj : m_objList) { obj->DrawUnLit(); }
	}
	KdShaderManager::GetInstance().m_HD2DShader.EndUnLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== =====
	// �����I�u�W�F�N�g(�������I�u�W�F�N�g��G�t�F�N�g)
	// ��Begin��End�̊Ԃɂ܂Ƃ߂�Draw����
	// ===== ===== ===== ===== ===== ===== ===== ===== =====
	KdShaderManager::GetInstance().m_postProcessShader.BeginBright();
	{
		for (auto& obj : m_objList) { obj->DrawBright(); }
	}
	KdShaderManager::GetInstance().m_postProcessShader.EndBright();
}

void SceneBase::DrawDebug()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// �A�e�̂Ȃ��I�u�W�F�N�g(�����ȕ������܂ޕ��̂�G�t�F�N�g)
	// ��Begin��End�̊Ԃɂ܂Ƃ߂�Draw����
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	KdShaderManager::GetInstance().m_HD2DShader.BeginUnLit();
	{
		for (auto& obj : m_objList) { obj->DrawDebug(); }
	}
	KdShaderManager::GetInstance().m_HD2DShader.EndUnLit();
}

void SceneBase::DrawSprite()
{
	// ===== ===== ===== =====
	// 2D�̕`��͂��̊Ԃōs��
	// ===== ===== ===== =====
	KdShaderManager::GetInstance().m_spriteShader.Begin();
	{
		for (auto& obj : m_objList) { obj->DrawSprite(); }

		// ImGui����
		if (m_pushAct) { ImGuiUpdate(); }
	}
	KdShaderManager::GetInstance().m_spriteShader.End();
}

void SceneBase::ImGuiUpdate()
{
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(250, 100), ImGuiCond_Once);

	// �f�o�b�O�E�B���h�E
	if (ImGui::Begin("Scene : Debug Window"))
	{
		ImGui::SliderFloat("BGMVol", &m_BGMVol, 0.1f, 1.0f);
	}
	ImGui::End();
}
