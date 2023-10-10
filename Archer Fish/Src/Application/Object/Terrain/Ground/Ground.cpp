#include "Ground.h"

void Ground::DrawLit()
{
	if (!m_spModel) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void Ground::Init()
{
	if (!m_spModel)
	{
		//	ƒ‚ƒfƒ‹(’n–Ê)‰Šú‰»
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Terrain/Ground/NGround/Ground.gltf"));
	}

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("GroundColl", m_spModel, KdCollider::TypeGround);
}
