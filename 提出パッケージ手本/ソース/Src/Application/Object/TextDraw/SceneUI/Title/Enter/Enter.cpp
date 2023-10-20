#include "Enter.h"

void Enter::Update()
{
	if (m_colorC <= 1.0f) { m_colorC += 0.02f; }
	else { m_colorC = 0.1f; }
}

void Enter::PostUpdate()
{
	SetColor({ 1.0f,1.0f,1.0f,m_colorC });
}

//void Enter::DrawSprite()
//{
//	KdShaderManager::Instance().m_spriteShader.DrawBox(m_pos.x, m_pos.y, m_half.x, m_half.y, &m_col, true);

//	TextDraw::DrawSprite();
//}

void Enter::Init()
{
	// テクスチャ表示
	SetText("Asset/Textures/SceneUI/Title/Key/Enter.png");
	SetPixel({ 0.0f,-150.0f });
	SetRecAng({ 0,0, 300, 60 });

//	m_pos = { 0.0f,-150.0f };
//	m_half = { 10.0f,10.0f };
//	m_col = { 0.0f,0.0f,0.0f,0.0f };
}
