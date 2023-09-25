#include "Result2D.h"

void Result2D::PostUpdate()
{
	// À•Ws—ñì¬
	m_texMat = Math::Matrix::CreateTranslation(m_texPos);

	// s—ñ‡¬(‚r‚q‚s)
	m_mWorld = m_texMat;
}

void Result2D::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0, &m_rcAngle);
}

void Result2D::Init()
{
	m_tex.Load("Asset/Textures/Result/win.png");
	m_rcAngle = { 0, 0, 1280, 720 };

	m_texPos = Math::Vector3::Zero;
	m_texMat = Math::Matrix::Identity;
}
