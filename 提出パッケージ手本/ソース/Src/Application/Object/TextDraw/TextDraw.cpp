#include "TextDraw.h"

void TextDraw::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, m_pixel.x, m_pixel.y, &m_recAngle, &m_color);
}
