#include "WaterSurface.h"

void WaterSurface::Update()
{
	m_offset.x += 0.001f;
	if (m_offset.x > 1.0f) { m_offset.x -= 1.0f; }

	m_offset.y += 0.001f;
	if (m_offset.y > 1.0f) { m_offset.y -= 1.0f; }
}

void WaterSurface::PostUpdate()
{
	// gksñ
	m_scaleMat = Math::Matrix::CreateScale({33.0f,1.0f,33.0f});

	// ÀWsñ
	m_transMat = Math::Matrix::CreateTranslation({ 0.0f,1.3f,0.0f });

	// sñ¬(rqs)
	m_mWorld = m_scaleMat * m_transMat;
}

void WaterSurface::DrawLit()
{
	KdShaderManager::GetInstance().m_HD2DShader.SetUVTiling({ 10.0f,10.0f });

	// Ê\»ðLø
	KdShaderManager::GetInstance().m_HD2DShader.SetWaterEnable(true);
	KdShaderManager::GetInstance().m_HD2DShader.SetWaterUVOffset(m_offset);

	if (!m_spModelData) { return; }
	KdShaderManager::GetInstance().m_HD2DShader.DrawModel(*m_spModelData, m_mWorld);

	KdShaderManager::GetInstance().m_HD2DShader.SetWaterEnable(false);
}

void WaterSurface::Init()
{
	if (!m_spModelData)
	{
		m_spModelData = std::make_shared<KdModelData>();
		m_spModelData = KdAssets::GetInstance().m_modeldatas.GetData(
			"Asset/Models/Terrain/WaterSurface/WaterSurface.gltf");
	}

	// addFeNX`ÇÆGPUÉ]
	std::shared_ptr<KdTexture> spTex;
	spTex = KdAssets::GetInstance().m_textures.GetData("Asset/Textures/WaterSurface/water.png");
	KdShaderManager::GetInstance().m_HD2DShader.SetWaterNomalText(*spTex);
}
