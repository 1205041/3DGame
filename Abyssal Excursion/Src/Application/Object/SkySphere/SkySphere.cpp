#include "SkySpher.h"

void SkySphere::PostUpdate()
{
//	Math::Matrix scaleMat;	// �g�k�s��
//	scaleMat = Math::Matrix::CreateScale(1.0f, 1.0f, 1.0f);

	// �s�񍇐�
//	m_mWorld = scaleMat;
}

void SkySphere::DrawLit()
{
	// ���f��(�w�i)
	if (!m_spModel) { return; }
//	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void SkySphere::Init()
{
	if (!m_spModel)
	{
//		m_spModel = std::make_shared<KdModelWork>();
//		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/SkySphere/deepSea/DeepSea.gltf"));
	}
}