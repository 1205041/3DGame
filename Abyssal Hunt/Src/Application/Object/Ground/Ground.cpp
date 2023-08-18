#include "Ground.h"

void Ground::PostUpdate()
{
	Math::Matrix scaleMat;	// �g�k�s��
	scaleMat = Math::Matrix::CreateScale(5.0f, 1.0f, 1.0f);

	Math::Matrix transMat;	// ���W�s��
	transMat = Math::Matrix::CreateTranslation({ 0,-2.0f,0 });

	// �s�񍇐�(�r�q�s)
	m_mWorld = scaleMat * transMat;
}

void Ground::GenerateDepthMapFromLight()
{
	// �|��(�n��)
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Ground::DrawLit()
{
	// �|��(�n��)
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Ground::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Ground/Stage/Stage.gltf"));
}
