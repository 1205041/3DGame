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
	// ��]�s��
	m_rotMat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(90.0f));

	// ���W�s��
	m_transMat = Math::Matrix::CreateTranslation({ 0,0.0f,0 });

	// �s�񍇐�(�r�q�s)
	m_mWorld = m_rotMat * m_transMat;
}

void WaterSurface::DrawLit()
{
	// ���ʕ\����L��
	KdShaderManager::Instance().m_HD2DShader.SetWaterEnable(true);
	KdShaderManager::Instance().m_HD2DShader.SetWaterUVOffset(m_offset);

	if (!m_spPolygon) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPolygon, m_mWorld);

	KdShaderManager::Instance().m_HD2DShader.SetWaterEnable(false);
}

void WaterSurface::Init()
{
	if (!m_spPolygon)
	{
		m_spPolygon = std::make_shared<KdSquarePolygon>();
		m_spPolygon->SetMaterial(KdAssets::Instance().m_textures.GetData(
			"Asset/Textures/WaterSurface/tp.png"));
		m_spPolygon->SetScale(10.0f);
		m_spPolygon->SetColor({ 0.5f,0.5f,0.5f,0.5f });
	}


	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("SkySpColl", m_spPolygon, KdCollider::TypeGround);

	// add�F�e�N�X�`���Ǎ���GPU�ɓ]��
	std::shared_ptr<KdTexture> spTex;
	spTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/WaterSurface/water.png");
	KdShaderManager::Instance().m_HD2DShader.SetWaterNomalText(*spTex);
}
