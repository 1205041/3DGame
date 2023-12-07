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
	// 拡縮行列
	m_scaleMat = Math::Matrix::CreateScale({33.0f,1.0f,33.0f});

	// 座標行列
	m_transMat = Math::Matrix::CreateTranslation({ 0,1.3f,0 });

	// 行列合成(ＳＲＴ)
	m_mWorld = m_scaleMat * m_transMat;
}

void WaterSurface::DrawLit()
{
	KdShaderManager::Instance().m_HD2DShader.SetUVTiling({ 10.0f,10.0f });

	// 水面表現を有効
	KdShaderManager::Instance().m_HD2DShader.SetWaterEnable(true);
	KdShaderManager::Instance().m_HD2DShader.SetWaterUVOffset(m_offset);

	if (!m_spModelData) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModelData, m_mWorld);

	KdShaderManager::Instance().m_HD2DShader.SetWaterEnable(false);
}

void WaterSurface::Init()
{
	if (!m_spModelData)
	{
		m_spModelData = std::make_shared<KdModelData>();
		m_spModelData = KdAssets::Instance().m_modeldatas.GetData(
			"Asset/Models/Terrain/WaterSurface/WaterSurface.gltf");
	}

	// add：テクスチャ読込とGPUに転送
	std::shared_ptr<KdTexture> spTex;
	spTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/WaterSurface/water.png");
	KdShaderManager::Instance().m_HD2DShader.SetWaterNomalText(*spTex);
}
