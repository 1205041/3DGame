#include "SkySphere.h"

void SkySphere::PostUpdate()
{
	// 拡縮行列
	m_scaleMat = Math::Matrix::CreateScale(450.0f, 450.0f, 450.0f);
	/* 拡縮メモ */
	// スカイスフィア：地面 = 150.0f：10.0fで丁度良いサイズ
	// なので地面のサイズの15倍がスカイスフィアのサイズ
	
	// 座標行列
	m_transMat = Math::Matrix::CreateTranslation({ 0,-5.0f,0 });

	// 行列合成(ＳＲＴ)
	m_mWorld = m_scaleMat * m_transMat;
}

void SkySphere::DrawLit()
{
	if (!m_spModel) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void SkySphere::DrawBright()
{
	if (!m_spModel) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void SkySphere::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/SkySphere/deepSea/DeepSea.gltf"));
	}
}
