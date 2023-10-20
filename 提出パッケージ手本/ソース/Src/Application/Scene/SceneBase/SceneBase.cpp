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
	KdShaderManager::Instance().m_HD2DShader.BeginGenerateDepthMapFromLight();
	{
		for (auto& obj : m_objList) { obj->GenerateDepthMapFromLight(); }
	}
	KdShaderManager::Instance().m_HD2DShader.EndGenerateDepthMapFromLight();

	// ===== ===== ===== ===== ===== ===== ===== =====
	// �A�e�̂���I�u�W�F�N�g(�s�����ȕ��̂�2D�L����)
	// ��Begin��End�̊Ԃɂ܂Ƃ߂�Draw����
	// ===== ===== ===== ===== ===== ===== ===== =====
	KdShaderManager::Instance().m_HD2DShader.BeginLit();
	{
		for (auto& obj : m_objList) { obj->DrawLit(); }
	}
	KdShaderManager::Instance().m_HD2DShader.EndLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// �A�e�̂Ȃ��I�u�W�F�N�g(�����ȕ������܂ޕ��̂�G�t�F�N�g)
	// ��Begin��End�̊Ԃɂ܂Ƃ߂�Draw����
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	KdShaderManager::Instance().m_HD2DShader.BeginUnLit();
	{
		for (auto& obj : m_objList) { obj->DrawUnLit(); }
	}
	KdShaderManager::Instance().m_HD2DShader.EndUnLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== =====
	// �����I�u�W�F�N�g(�������I�u�W�F�N�g��G�t�F�N�g)
	// ��Begin��End�̊Ԃɂ܂Ƃ߂�Draw����
	// ===== ===== ===== ===== ===== ===== ===== ===== =====
	KdShaderManager::Instance().m_postProcessShader.BeginBright();
	{
		for (auto& obj : m_objList) { obj->DrawBright(); }
	}
	KdShaderManager::Instance().m_postProcessShader.EndBright();
}

void SceneBase::DrawDebug()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// �A�e�̂Ȃ��I�u�W�F�N�g(�����ȕ������܂ޕ��̂�G�t�F�N�g)
	// ��Begin��End�̊Ԃɂ܂Ƃ߂�Draw����
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	KdShaderManager::Instance().m_HD2DShader.BeginUnLit();
	{
		for (auto& obj : m_objList) { obj->DrawDebug(); }
	}
	KdShaderManager::Instance().m_HD2DShader.EndUnLit();
}

void SceneBase::DrawSprite()
{
	// ===== ===== ===== =====
	// 2D�̕`��͂��̊Ԃōs��
	// ===== ===== ===== =====
	KdShaderManager::Instance().m_spriteShader.Begin();
	{
		for (auto& obj : m_objList) { obj->DrawSprite(); }
	}
	KdShaderManager::Instance().m_spriteShader.End();
}
