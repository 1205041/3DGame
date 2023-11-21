#include "SphereGround.h"

void SphereGround::PostUpdate()
{
	// ägèkçsóÒ
	m_scaleMat = Math::Matrix::CreateScale(30.0f, 30.0f, 30.0f);
	
	// ç¿ïWçsóÒ
	m_transMat = Math::Matrix::CreateTranslation({ 0,0.0f,0 });

	// çsóÒçáê¨(ÇrÇqÇs)
	m_mWorld = m_scaleMat * m_transMat;
}

void SphereGround::DrawUnLit()
{
	if (!m_spModelData) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModelData, m_mWorld);
}

void SphereGround::Init()
{
	if (!m_spModelData)
	{
		m_spModelData = std::make_shared<KdModelData>();
		m_spModelData->Load("Asset/Models/SphereDome/SphereGround/SphereGround.gltf");
	}

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("SkySpColl", m_spModelData, KdCollider::TypeGround);
}
