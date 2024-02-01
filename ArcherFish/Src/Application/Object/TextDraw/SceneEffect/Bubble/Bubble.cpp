#include "Bubble.h"

void Bubble::Update()
{
	/*
	//i < pointNum , i % 2 == 0 , m_pos[i].x < 0 “™
	if (i % 2) { m_pos[i].x -= 1; }	//¶‚Ö
	else { m_pos[i].x += 1; }		//‰E‚Ö
	*/

	m_pixel.y += 2;
	if (m_pixel.y > 360) { m_pixel.y = -360; }
}

void Bubble::Init()
{
	m_pixel = Math::Vector2::Zero;
	m_color = kWhiteColor;
}

void Bubble::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawCircle((int)m_pixel.x, (int)m_pixel.y, m_radius, &m_color, m_fill);
}
