#include "SkySpher.h"

void SkySphere::PostUpdate()
{
	Math::Matrix scaleMat;	// �g�k�s��
	scaleMat = Math::Matrix::CreateScale(1.0f, 1.0f, 1.0f);

	// �s�񍇐�
	m_mWorld = scaleMat;
}

void SkySphere::DrawLit()
{
	// �|��(�n��)
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void SkySphere::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/SkySphere/skysphere.gltf"));
}