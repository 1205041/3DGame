#include "SceneBase.h"

void SceneBase::PreUpdate()
{
	/* オブジェクトリストの整理 */
	auto it = m_objList.begin();
	while (it != m_objList.end())
	{
		// 期限切れ
		if ((*it)->IsExpired()) { it = m_objList.erase(it); }
		else { ++it; }
	}
}

void SceneBase::Update()
{
	for (auto& obj : m_objList) { obj->Update(); }

	Event();

	// imGui表示切替
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
	// 光を遮るオブジェクト(不透明な物体や2Dキャラ)
	// はBeginとEndの間にまとめてDrawする
	// ===== ===== ===== ===== ===== ===== ===== =====
	
	KdShaderManager::GetInstance().m_HD2DShader.BeginGenerateDepthMapFromLight();
	{
		for (auto& obj : m_objList) { obj->GenerateDepthMapFromLight(); }
	}
	KdShaderManager::GetInstance().m_HD2DShader.EndGenerateDepthMapFromLight();

	// ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のあるオブジェクト(不透明な物体や2Dキャラ)
	// はBeginとEndの間にまとめてDrawする
	// ===== ===== ===== ===== ===== ===== ===== =====
	KdShaderManager::GetInstance().m_HD2DShader.BeginLit();
	{
		for (auto& obj : m_objList) { obj->DrawLit(); }
	}
	KdShaderManager::GetInstance().m_HD2DShader.EndLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のないオブジェクト(透明な部分を含む物体やエフェクト)
	// はBeginとEndの間にまとめてDrawする
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	KdShaderManager::GetInstance().m_HD2DShader.BeginUnLit();
	{
		for (auto& obj : m_objList) { obj->DrawUnLit(); }
	}
	KdShaderManager::GetInstance().m_HD2DShader.EndUnLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光源オブジェクト(自ら光るオブジェクトやエフェクト)
	// はBeginとEndの間にまとめてDrawする
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
	// 陰影のないオブジェクト(透明な部分を含む物体やエフェクト)
	// はBeginとEndの間にまとめてDrawする
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
	// 2Dの描画はこの間で行う
	// ===== ===== ===== =====
	KdShaderManager::GetInstance().m_spriteShader.Begin();
	{
		for (auto& obj : m_objList) { obj->DrawSprite(); }

		// ImGui処理
		if (m_pushAct) { ImGuiUpdate(); }
	}
	KdShaderManager::GetInstance().m_spriteShader.End();
}

void SceneBase::ImGuiUpdate()
{
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(250, 100), ImGuiCond_Once);

	// デバッグウィンドウ
	if (ImGui::Begin("Scene : Debug Window"))
	{
		ImGui::SliderFloat("BGMVol", &m_BGMVol, 0.1f, 1.0f);
	}
	ImGui::End();
}
