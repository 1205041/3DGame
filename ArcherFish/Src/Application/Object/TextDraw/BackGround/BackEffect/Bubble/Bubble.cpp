#include "Bubble.h"

void Bubble::DrawSprite()
{
	// 円を描画(泡)
//	for (int i = 0; i < babbleNum; i++)
//	{
	//{ 0.2f,0.8f,0.8f,0.7f }
		m_color = { 1.0f,1.0f,1.0f,1.0f };
		KdShaderManager::Instance().m_spriteShader.DrawCircle(m_pos.x, m_pos.y, m_radius, &m_color, false);
//	}
}

void Bubble::Update()
{
//	for (int i = 0; i < babbleNum; i++)
//	{
		m_pos.y += 0.2f;

		// 上端のチェックで上へワープ
		if (m_pos.y > 180) { m_pos.y = -181; }
//	}
}

void Bubble::PostUpdate()
{
}

void Bubble::Init()
{

}
