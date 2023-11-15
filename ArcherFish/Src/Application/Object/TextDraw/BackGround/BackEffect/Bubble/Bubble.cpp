#include "Bubble.h"

void Bubble::DrawSprite()
{
	// ‰~‚ð•`‰æ(–A)
	SetColor({ 0.2f,0.8f,0.8f,0.7f });
	KdShaderManager::Instance().m_spriteShader.DrawCircle(m_pos.x, m_pos.y, m_radius, &m_color, false);
	
}

void Bubble::Update()
{
}

void Bubble::PostUpdate()
{
}

void Bubble::Init()
{
}
