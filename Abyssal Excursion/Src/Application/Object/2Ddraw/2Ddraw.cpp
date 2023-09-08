#include "2Ddraw.h"

void twoDdraw::PostUpdate()
{
	// À•Ws—ñì¬
	m_texMat = Math::Matrix::CreateTranslation(m_texPos);

	// s—ñ‡¬(‚r‚q‚s)
	m_mWorld = m_texMat;
}

void twoDdraw::DrawSprite()
{
	Math::Rectangle rc = { 0,0,1280,720 };
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0, &rc);
}

void twoDdraw::Init()
{
	Math::Vector3 m_texPos = Math::Vector3::Zero;
	Math::Matrix m_texMat = Math::Matrix::Identity;
}
