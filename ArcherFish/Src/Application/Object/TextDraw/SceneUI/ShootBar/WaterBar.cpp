#include "WaterBar.h"

void WaterBar::Update()
{
	if (KdInputManager::Instance().GetButtonState("WaterBarMax")) { m_water = m_waterMax; }
//	if (m_water < m_waterMax) { m_water += 2; }
	if (m_water > 0) { m_water -= 2; }

	if (m_water > m_waterWarning) { color = blue; }
	else if (m_water > m_waterDanger) { color = yellow; }
	else { color = red; }
}

void WaterBar::Init()
{
}

void WaterBar::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawBox((int)pixel.x, (int)pixel.y, m_waterMax, height, &barColor, true);
	KdShaderManager::Instance().m_spriteShader.DrawBox((int)pixel.x, (int)pixel.y, m_water, height, &color, true);
}