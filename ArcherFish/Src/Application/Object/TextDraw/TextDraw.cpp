#include "TextDraw.h"

void TextDraw::DrawSprite()
{
	KdShaderManager::GetInstance().m_spriteShader.DrawTex(&m_text, (int)m_textPixel.x, (int)m_textPixel.y, &m_textRectAng, &m_textColor, m_textPivot);
}
