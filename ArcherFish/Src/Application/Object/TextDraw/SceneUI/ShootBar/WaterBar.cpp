#include "WaterBar.h"

void WaterBar::Update()
{
	if (m_water < m_waterMax) { m_water += 2; }

	if (m_water > m_waterWarning) 
	{ 
		m_color = kBlueColor; 
	}
	else if (m_water > m_waterDanger) 
	{ 
		m_color = kYellowColor;
	}
	else 
	{ 
		m_color = kRedColor;
	}
}

void WaterBar::Init()
{
	m_tex.Load("Asset/Textures/SceneUI/Game/Bar/Bar.png");
	m_txt.Load("Asset/Textures/SceneUI/Game/Bar/fish.png");

	m_pixel = { -500.0f,-310.0f };
	m_rectAng = { 0,0,250 ,15 };
	m_color = kBlueColor;
	m_pivot = { 0.0f, 0.5f };
}

void WaterBar::DrawSprite()
{
	KdShaderManager::GetInstance().m_spriteShader.DrawTex(&m_txt, (int)m_txtPixel.x, (int)m_txtPixel.y, &m_txtRectAng);
	KdShaderManager::GetInstance().m_spriteShader.DrawTex(&m_tex, (int)m_pixel.x, (int)m_pixel.y, m_waterMax, m_height, &m_rectAng, &m_barColor, m_pivot);
	KdShaderManager::GetInstance().m_spriteShader.DrawTex(&m_tex, (int)m_pixel.x, (int)m_pixel.y, m_water, m_height, &m_rectAng, &m_color, m_pivot);
}