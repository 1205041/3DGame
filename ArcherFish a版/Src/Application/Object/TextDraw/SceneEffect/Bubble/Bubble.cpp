#include "Bubble.h"

void Bubble::Update()
{
	m_pixel += m_move;

	if (m_pixel.x > 640.0f || m_pixel.x < -640.0f ||
		m_pixel.y > 360.0f || m_pixel.y < -360.0f)
	{
//		m_move *= -1.0f;
		m_pixel *= -1.0f;
	}
}

void Bubble::Init()
{
	std::mt19937 mt(std::random_device{}());
	std::uniform_real_distribution<float> pixelX(-640.0f, 640.0f), 
		pixelY(-360.0f, 360.0f), 
		radius(10.0f, 70.0f),
		color(0.0f, 1.0f), 
		move(-5.0f, 5.0f);

	m_radius = radius(mt);
	m_color = { color(mt),0.5f,1.0f,0.7f };
	m_pixel = { pixelX(mt) , pixelY(mt) };
	m_move = { move(mt),move(mt) };
}

void Bubble::DrawSprite()
{
	if (m_act) { KdShaderManager::Instance().m_spriteShader.DrawCircle((int)m_pixel.x, (int)m_pixel.y, (int)m_radius, &m_color, m_fill); }
}
