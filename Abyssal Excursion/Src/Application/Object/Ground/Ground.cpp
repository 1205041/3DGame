#include "Ground.h"

void Ground::GenerateDepthMapFromLight()
{
	if (!m_spModel) { return; }

	// モデル(地面)
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void Ground::DrawLit()
{
	// モデル(地面)
	if (!m_spModel) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void Ground::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Ground/Stage/Stage.gltf"));
	}
}
