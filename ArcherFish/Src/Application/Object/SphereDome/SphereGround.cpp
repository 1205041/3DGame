#include "SphereGround.h"

void SphereGround::Update()
{
	m_offset.x += 0.001f;
	if (m_offset.x > 1.0f) { m_offset.x -= 1.0f; }

	m_offset.y += 0.001f;
	if (m_offset.y > 1.0f) { m_offset.y -= 1.0f; }
}

void SphereGround::PostUpdate()
{
	// �g�k�s��
	m_scaleMat = Math::Matrix::CreateScale(18.0f, 18.0f, 18.0f);
	
	// ���W�s��
	m_transMat = Math::Matrix::CreateTranslation({ 0,0.0f,0 });

	// �s�񍇐�(�r�q�s)
	m_mWorld = m_scaleMat * m_transMat;
}

void SphereGround::DrawLit()
{
	// ���ʕ\����L��
	KdShaderManager::Instance().m_HD2DShader.SetWaterEnable(true);
	KdShaderManager::Instance().m_HD2DShader.SetWaterUVOffset(m_offset);

	if (!m_spModelData) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModelData, m_mWorld);

	KdShaderManager::Instance().m_HD2DShader.SetWaterEnable(false);
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

	// add�F�e�N�X�`���Ǎ���GPU�ɓ]��
	std::shared_ptr<KdTexture> spTex;
	spTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/WaterSurface/water.png");
	KdShaderManager::Instance().m_HD2DShader.SetWaterNomalText(*spTex);
}
