#include "TextDraw.h"

void TextDraw::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, (int)m_pixel.x, (int)m_pixel.y, &m_rectAng, &m_color, m_pivot);
}
