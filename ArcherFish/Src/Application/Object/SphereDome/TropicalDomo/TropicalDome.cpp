#include "TropicalDome.h"

void TropicalDome::PostUpdate()
{
	// 拡縮行列
//	m_scaleMat = Math::Matrix::CreateScale(149.0f, 149.0f, 149.0f);
	m_scaleMat = Math::Matrix::CreateScale(1.0f, 1.0f, 1.0f);

	/* 拡縮メモ */
	// スカイスフィア：地面 = 149.0f：10.0fで丁度良いサイズ
	// なので地面のサイズの15倍がスカイスフィアのサイズ
	
	// 座標行列
	m_transMat = Math::Matrix::CreateTranslation({ 0,-6.0f,0 });

	// 行列合成(ＳＲＴ)
	m_mWorld = m_scaleMat * m_transMat;
}

void TropicalDome::DrawUnLit()
{
	if (!m_spModel) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void TropicalDome::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/SphereDome/SphereGround/SphereGround.gltf"));
	}

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("SkySpColl", m_spModel, KdCollider::TypeBump);
}
