#include "TextBase.h"

void TextBase::PostUpdate()
{
	// ���W�s��쐬
	m_texMat = Math::Matrix::CreateTranslation(m_texPos);

	// �s�񍇐�(�r�q�s)
	m_mWorld = m_texMat;

	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
}

void TextBase::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, m_pixel.x, m_pixel.y, &m_rcAngle, &m_color);
}
