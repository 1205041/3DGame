#include "TextDraw.h"

void TextDraw::Update()
{
	if (m_AlphC <= 1.0f) { m_AlphC += 0.02f; }
	else { m_AlphC -= 1.0f; }
}

void TextDraw::PostUpdate()
{
	SetColor({ 1.0f,1.0f,1.0f,m_AlphC });
}

void TextDraw::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, m_pixel.x, m_pixel.y, &m_recAngle, &m_color);
}

void TextDraw::Init()
{
	SetColor({ 1.0f,1.0f,1.0f,1.0f });
}

void TextDraw::SetLoadText(const std::string _tex, const Math::Vector2 _pixel, const Math::Rectangle _recAng)
{
	m_tex.Load(_tex);
	m_pixel = _pixel;
	m_recAngle = _recAng;
}
