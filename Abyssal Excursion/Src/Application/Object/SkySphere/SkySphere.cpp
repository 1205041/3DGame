#include "SkySpher.h"

void SkySphere::PostUpdate()
{
	Math::Matrix scaleMat;	// 拡縮行列
	scaleMat = Math::Matrix::CreateScale(1.0f, 1.0f, 1.0f);

	// 行列合成
	m_mWorld = scaleMat;
}

void SkySphere::DrawLit()
{
	// 板ポリ(地面)
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void SkySphere::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/SkySphere/skysphere.gltf"));
}