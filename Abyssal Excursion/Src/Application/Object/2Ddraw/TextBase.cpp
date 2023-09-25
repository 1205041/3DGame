#include "TextBase.h"

void TextBase::PostUpdate()
{
	// À•Ws—ñì¬
	m_texMat = Math::Matrix::CreateTranslation(m_texPos);

	// s—ñ‡¬(‚r‚q‚s)
	m_mWorld = m_texMat;

	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
}

void TextBase::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0, &m_rcAngle, &m_color);
}
