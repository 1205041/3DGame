#include "WaterBar.h"

void WaterBar::Update()
{
//	if (KdInputManager::Instance().GetButtonState("WaterBarMax")) { m_water = 0; }
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
	m_text.Load("Asset/Textures/SceneUI/Game/Bar/Bar.png");
	m_fishText.Load("Asset/Textures/SceneUI/Game/Bar/fish.png");
}

void WaterBar::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_fishText, (int)m_fishPixel.x, (int)m_fishPixel.y, &m_fishRectAng);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_text, (int)m_pixel.x, (int)m_pixel.y, m_waterMax, m_height, &m_rectAng, &m_barColor, m_pivot);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_text, (int)m_pixel.x, (int)m_pixel.y, m_water, m_height, &m_rectAng, &m_color, m_pivot);
}