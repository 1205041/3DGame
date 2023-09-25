#include "Title2D.h"

void Title2D::PostUpdate()
{
	// ���W�s��쐬
	m_texMat = Math::Matrix::CreateTranslation(m_texPos);

	// �s�񍇐�(�r�q�s)
	m_mWorld = m_texMat;
}

void Title2D::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0, &m_rcAngle);
}

void Title2D::Init()
{
	m_tex.Load("Asset/Textures/Title/Title.png");
	m_rcAngle = { 0,0,1280,720 };

	m_texPos = Math::Vector3::Zero;
	m_texMat = Math::Matrix::Identity;
}
