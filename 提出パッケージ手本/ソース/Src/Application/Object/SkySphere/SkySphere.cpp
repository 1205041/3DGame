#include "SkySphere.h"

void SkySphere::PostUpdate()
{
	// �g�k�s��
	m_scaleMat = Math::Matrix::CreateScale(149.0f, 149.0f, 149.0f);
	/* �g�k���� */
	// �X�J�C�X�t�B�A�F�n�� = 149.0f�F10.0f�Œ��x�ǂ��T�C�Y
	// �Ȃ̂Œn�ʂ̃T�C�Y��15�{���X�J�C�X�t�B�A�̃T�C�Y
	
	// ���W�s��
	m_transMat = Math::Matrix::CreateTranslation({ 0,-6.0f,0 });

	// �s�񍇐�(�r�q�s)
	m_mWorld = m_scaleMat * m_transMat;
}

void SkySphere::DrawBright()
{
	if (!m_spModel) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void SkySphere::DrawLit()
{
	if (!m_spModel) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void SkySphere::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/SkySphere/deepSea/DeepSea2.gltf"));
	}

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("SkySpColl", m_spModel, KdCollider::TypeBump);
}
