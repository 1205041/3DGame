#include "Sight.h"

void Sight::PostUpdate()
{
	// À•Ws—ñì¬
	m_texMat = Math::Matrix::CreateTranslation(m_texPos);

	// s—ñ‡¬(‚r‚q‚s)
	m_mWorld = m_texMat;
}

void Sight::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0, &m_rcAngle);
}

void Sight::Init()
{
	m_tex.Load("Asset/Textures/GameUI/SightTP.png");
	m_rcAngle = { 0, 0, 32, 32 };

	m_texPos = Math::Vector3::Zero;
	m_texMat = Math::Matrix::Identity;
}
