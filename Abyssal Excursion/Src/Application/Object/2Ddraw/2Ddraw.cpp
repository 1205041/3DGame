#include "2Ddraw.h"

void twoDdraw::PostUpdate()
{
	// ���W�s��쐬
	m_texMat = Math::Matrix::CreateTranslation(m_texPos);

	// �s�񍇐�(�r�q�s)
	m_mWorld = m_texMat;
}

void twoDdraw::DrawSprite()
{
	
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0, &m_rcAngle);
}

void twoDdraw::Init()
{
	m_texPos = Math::Vector3::Zero;
	m_texMat = Math::Matrix::Identity;
}
