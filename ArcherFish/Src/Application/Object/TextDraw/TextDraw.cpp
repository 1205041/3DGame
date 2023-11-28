#include "TextDraw.h"

void TextDraw::SetLoadText(const std::string _tex, const Math::Rectangle _recAng)
{
	m_tex.Load(_tex);
	m_recAngle = _recAng;
}

void TextDraw::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, (int)m_pixel.x, (int)m_pixel.y, &m_recAngle, &m_color);
}
