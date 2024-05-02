#include "UnderWater.h"

void UnderWater::Update()
{
	m_offset.x += 0.001f;
	if (m_offset.x > 1.0f) { m_offset.x -= 1.0f; }

	m_offset.y += 0.001f;
	if (m_offset.y > 1.0f) { m_offset.y -= 1.0f; }
}

void UnderWater::PostUpdate()
{
	// �g�k�s��
	m_scaleMat = Math::Matrix::CreateScale(18.0f, 18.0f, 18.0f);

	// ���W�s��
	m_transMat = Math::Matrix::CreateTranslation({ 0,0.0f,0 });

	// �s�񍇐�(�r�q�s)
	m_mWorld = m_scaleMat * m_transMat;
}

void UnderWater::DrawLit()
{
	// ���ʕ\����L��
	KdShaderManager::GetInstance().m_HD2DShader.SetWaterEnable(true);
	KdShaderManager::GetInstance().m_HD2DShader.SetWaterUVOffset(m_offset);

	if (!m_spModelData) { return; }
	KdShaderManager::GetInstance().m_HD2DShader.DrawModel(*m_spModelData, m_mWorld);
	
	KdShaderManager::GetInstance().m_HD2DShader.SetWaterEnable(false);
}

void UnderWater::Init()
{
	if (!m_spModelData)
	{
		m_spModelData = std::make_shared<KdModelData>();
		m_spModelData = KdAssets::GetInstance().m_modeldatas.GetData(
			"Asset/Models/Terrain/UnderWater/UnderWater.gltf");
	}

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("SkySpColl", m_spModelData, KdCollider::TypeGround);
}
