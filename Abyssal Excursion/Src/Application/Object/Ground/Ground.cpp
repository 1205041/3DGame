#include "Ground.h"

void Ground::PostUpdate()
{
	Math::Matrix scaleMat;	// Šgks—ñ
	scaleMat = Math::Matrix::CreateScale(5.0f, 1.0f, 1.0f);

	Math::Matrix transMat;	// À•Ws—ñ
	transMat = Math::Matrix::CreateTranslation({ 0,-2.0f,0 });

	// s—ñ‡¬(‚r‚q‚s)
	m_mWorld = scaleMat * transMat;
}

void Ground::GenerateDepthMapFromLight()
{
	// ”Âƒ|ƒŠ(’n–Ê)
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Ground::DrawLit()
{
	// ”Âƒ|ƒŠ(’n–Ê)
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Ground::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Ground/Stage/Stage.gltf"));
}
